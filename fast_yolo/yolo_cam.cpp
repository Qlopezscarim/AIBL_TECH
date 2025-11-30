#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "net.h"

#define CAM_W 160
#define CAM_H 120
#define FB_W 128
#define FB_H 160
#define MODEL_SIZE 320

// COCO class names (80 classes)
const char* COCO_CLASSES[] = {
    "person","bicycle","car","motorcycle","airplane","bus","train","truck","boat","traffic light",
    "fire hydrant","stop sign","parking meter","bench","bird","cat","dog","horse","sheep","cow",
    "elephant","bear","zebra","giraffe","backpack","umbrella","handbag","tie","suitcase","frisbee",
    "skis","snowboard","sports ball","kite","baseball bat","baseball glove","skateboard","surfboard",
    "tennis racket","bottle","wine glass","cup","fork","knife","spoon","bowl","banana","apple",
    "sandwich","orange","broccoli","carrot","hot dog","pizza","donut","cake","chair","couch",
    "potted plant","bed","dining table","toilet","tv","laptop","mouse","remote","keyboard",
    "cell phone","microwave","oven","toaster","sink","refrigerator","book","clock","vase","scissors",
    "teddy bear","hair drier","toothbrush"
};

// Target classes: skateboard=36, cup=41, mouse=63
const int TARGET_CLASSES[] = {36, 41, 63};
const int NUM_TARGET_CLASSES = 3;

// YOLO anchors for YOLO-Fastest
const float ANCHORS[6][2] = {
    {12, 18}, {37, 49}, {52, 132},  // For 20x20 feature map
    {115, 73}, {119, 199}, {242, 238}  // For 10x10 feature map
};

const char* get_class_name(int class_idx) {
    if (class_idx >= 0 && class_idx < 80) {
        return COCO_CLASSES[class_idx];
    }
    return "unknown";
}

#define COLOR_YELLOW 0xFFE0

struct Detection {
    float x, y, w, h;
    float confidence;
    int class_id;
};

static inline float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

static inline uint16_t yuyv_to_rgb565(uint8_t y, int u, int v)
{
    int r = y + ((351 * v) >> 8);
    int g = y - ((179 * v + 86 * u) >> 8);
    int b = y + ((443 * u) >> 8);

    r = (r < 0) ? 0 : (r > 255) ? 255 : r;
    g = (g < 0) ? 0 : (g > 255) ? 255 : g;
    b = (b < 0) ? 0 : (b > 255) ? 255 : b;

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static inline void draw_h_line(uint16_t *fb, int x1, int x2, int y, uint16_t color)
{
    if (y < 0 || y >= FB_H) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= FB_W) x2 = FB_W - 1;
    for (int x = x1; x <= x2; x++) {
        fb[y * FB_W + x] = color;
    }
}

static inline void draw_v_line(uint16_t *fb, int x, int y1, int y2, uint16_t color)
{
    if (x < 0 || x >= FB_W) return;
    if (y1 < 0) y1 = 0;
    if (y2 >= FB_H) y2 = FB_H - 1;
    for (int y = y1; y <= y2; y++) {
        fb[y * FB_W + x] = color;
    }
}

void draw_bbox(uint16_t *fb, int x1, int y1, int x2, int y2, uint16_t color)
{
    draw_h_line(fb, x1, x2, y1, color);
    draw_h_line(fb, x1, x2, y1+1, color);
    draw_h_line(fb, x1, x2, y2, color);
    draw_h_line(fb, x1, x2, y2-1, color);

    draw_v_line(fb, x1, y1, y2, color);
    draw_v_line(fb, x1+1, y1, y2, color);
    draw_v_line(fb, x2, y1, y2, color);
    draw_v_line(fb, x2-1, y1, y2, color);
}

// Parse YOLO output layer - OPTIMIZED VERSION
void parse_yolo_layer(const ncnn::Mat& out, int stride, int anchor_start,
                     std::vector<Detection>& detections, float conf_thresh)
{
    int grid_h = out.h;
    int grid_w = out.w;

    for (int h = 0; h < grid_h; h++) {
        for (int w = 0; w < grid_w; w++) {
            for (int a = 0; a < 3; a++) {
                int base_channel = a * 85;

                float obj_conf = sigmoid(out.channel(base_channel + 4).row(h)[w]);

                if (obj_conf < conf_thresh) continue;

                // OPTIMIZED: Only check our 3 target classes instead of all 80
                int best_class = -1;
                float best_class_score = 0;
                for (int i = 0; i < NUM_TARGET_CLASSES; i++) {
                    int c = TARGET_CLASSES[i];
                    float score = sigmoid(out.channel(base_channel + 5 + c).row(h)[w]);
                    if (score > best_class_score) {
                        best_class_score = score;
                        best_class = c;
                    }
                }

                if (best_class < 0) continue;  // No target class had significant score

                float confidence = obj_conf * best_class_score;
                if (confidence < conf_thresh) continue;

                // Decode bounding box
                float tx = out.channel(base_channel + 0).row(h)[w];
                float ty = out.channel(base_channel + 1).row(h)[w];
                float tw = out.channel(base_channel + 2).row(h)[w];
                float th = out.channel(base_channel + 3).row(h)[w];

                float anchor_w = ANCHORS[anchor_start + a][0];
                float anchor_h = ANCHORS[anchor_start + a][1];

                // Decode according to YOLOv3 formula
                float bx = (sigmoid(tx) + w) * stride;
                float by = (sigmoid(ty) + h) * stride;
                float bw = anchor_w * expf(tw);
                float bh = anchor_h * expf(th);

                Detection det;
                det.x = bx;
                det.y = by;
                det.w = bw;
                det.h = bh;
                det.confidence = confidence;
                det.class_id = best_class;

                detections.push_back(det);
            }
        }
    }
}

// Simple NMS
void nms(std::vector<Detection>& detections, float iou_thresh)
{
    std::sort(detections.begin(), detections.end(),
              [](const Detection& a, const Detection& b) {
                  return a.confidence > b.confidence;
              });

    std::vector<bool> suppressed(detections.size(), false);

    for (size_t i = 0; i < detections.size(); i++) {
        if (suppressed[i]) continue;

        Detection& a = detections[i];
        float x1a = a.x - a.w / 2;
        float y1a = a.y - a.h / 2;
        float x2a = a.x + a.w / 2;
        float y2a = a.y + a.h / 2;
        float area_a = a.w * a.h;

        for (size_t j = i + 1; j < detections.size(); j++) {
            if (suppressed[j]) continue;
            if (detections[j].class_id != a.class_id) continue;

            Detection& b = detections[j];
            float x1b = b.x - b.w / 2;
            float y1b = b.y - b.h / 2;
            float x2b = b.x + b.w / 2;
            float y2b = b.y + b.h / 2;
            float area_b = b.w * b.h;

            float inter_x1 = std::max(x1a, x1b);
            float inter_y1 = std::max(y1a, y1b);
            float inter_x2 = std::min(x2a, x2b);
            float inter_y2 = std::min(y2a, y2b);

            float inter_w = std::max(0.0f, inter_x2 - inter_x1);
            float inter_h = std::max(0.0f, inter_y2 - inter_y1);
            float inter_area = inter_w * inter_h;

            float iou = inter_area / (area_a + area_b - inter_area);

            if (iou > iou_thresh) {
                suppressed[j] = true;
            }
        }
    }

    std::vector<Detection> kept;
    for (size_t i = 0; i < detections.size(); i++) {
        if (!suppressed[i]) {
            kept.push_back(detections[i]);
        }
    }
    detections = kept;
}

int main()
{
    // Open camera
    int cam = open("/dev/video0", O_RDWR);
    if (cam < 0) { perror("open video0"); return 1; }

    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width  = CAM_W;
    fmt.fmt.pix.height = CAM_H;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(cam, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT");
        return 1;
    }

    struct v4l2_requestbuffers req = {0};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(cam, VIDIOC_REQBUFS, &req) < 0) {
        perror("REQBUFS");
        return 1;
    }

    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    if (ioctl(cam, VIDIOC_QUERYBUF, &buf) < 0) {
        perror("QUERYBUF");
        return 1;
    }

    void *map = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                     MAP_SHARED, cam, buf.m.offset);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Open framebuffer
    int fb = open("/dev/fb0", O_RDWR);
    if (fb < 0) { perror("open fb0"); return 1; }

    // Load YOLO-Fastest model
    ncnn::Net yolo;
    yolo.opt.use_vulkan_compute = false;
    yolo.opt.num_threads = 2;

    printf("Loading YOLO-Fastest-1.1 model...\n");
    if (yolo.load_param("/home/beagle/Yolo-Fastest/sample/ncnn/model/yolo-fastest-1.1.param")) {
        fprintf(stderr, "Failed to load param\n");
        return 1;
    }
    if (yolo.load_model("/home/beagle/Yolo-Fastest/sample/ncnn/model/yolo-fastest-1.1.bin")) {
        fprintf(stderr, "Failed to load model\n");
        return 1;
    }

    // Start streaming
    if (ioctl(cam, VIDIOC_STREAMON, &buf.type) < 0) {
        perror("STREAMON");
        return 1;
    }

    printf("Streaming with YOLO-Fastest detection...\n");
    printf("Detecting: cup, skateboard, mouse\n");

    uint16_t fb_pixels[FB_W * FB_H];
    std::vector<Detection> detections;
    int detection_persist_counter = 0;

    // Letterbox calculations for landscape camera on portrait display
    int scaled_h = (CAM_H * FB_W) / CAM_W;  // 96
    int offset_y = (FB_H - scaled_h) / 2;    // 32

    int frame_count = 0;

    while (1)
    {
        if (ioctl(cam, VIDIOC_QBUF, &buf) < 0) {
            perror("QBUF");
            return 1;
        }

        if (ioctl(cam, VIDIOC_DQBUF, &buf) < 0) {
            perror("DQBUF");
            return 1;
        }

        uint8_t *yuyv = (uint8_t*)map;

        // Convert camera to framebuffer (no rotation - letterbox it)
        memset(fb_pixels, 0, sizeof(fb_pixels));

        // Camera is 160x120 (4:3 landscape), FB is 128x160 (portrait)
        // To display landscape on portrait without rotation, we letterbox
        // Scale to fit width: 160 -> 128, so 120 -> 96
        // Center vertically: (160-96)/2 = 32 pixel borders top/bottom

        for (int fb_y = 0; fb_y < scaled_h; fb_y++) {
            for (int fb_x = 0; fb_x < FB_W; fb_x++) {
                int cam_x = fb_x * CAM_W / FB_W;
                int cam_y = fb_y * CAM_H / scaled_h;

                if (cam_x >= CAM_W || cam_y >= CAM_H)
                    continue;

                int cam_x_even = cam_x & ~1;
                int yuyv_idx = (cam_y * CAM_W + cam_x_even) * 2;

                uint8_t y_val = yuyv[yuyv_idx + (cam_x & 1) * 2];
                int u = (int)yuyv[yuyv_idx + 1] - 128;
                int v = (int)yuyv[yuyv_idx + 3] - 128;

                fb_pixels[(fb_y + offset_y) * FB_W + fb_x] = yuyv_to_rgb565(y_val, u, v);
            }
        }

        // Run detection every 3 frames
        if (frame_count % 3 == 0) {
            detections.clear();
            detection_persist_counter = 0;

            // Convert YUYV to RGB for NCNN (no rotation)
            unsigned char* rgb_data = new unsigned char[CAM_W * CAM_H * 3];
            for (int y = 0; y < CAM_H; y++) {
                for (int x = 0; x < CAM_W; x += 2) {
                    int idx = (y * CAM_W + x) * 2;
                    uint8_t y1 = yuyv[idx];
                    int u = (int)yuyv[idx+1] - 128;
                    uint8_t y2 = yuyv[idx+2];
                    int v = (int)yuyv[idx+3] - 128;

                    // Pixel 1
                    int r1 = y1 + ((351 * v) >> 8);
                    int g1 = y1 - ((179 * v + 86 * u) >> 8);
                    int b1 = y1 + ((443 * u) >> 8);
                    r1 = (r1 < 0) ? 0 : (r1 > 255) ? 255 : r1;
                    g1 = (g1 < 0) ? 0 : (g1 > 255) ? 255 : g1;
                    b1 = (b1 < 0) ? 0 : (b1 > 255) ? 255 : b1;

                    int rgb_idx1 = (y * CAM_W + x) * 3;
                    rgb_data[rgb_idx1] = r1;
                    rgb_data[rgb_idx1 + 1] = g1;
                    rgb_data[rgb_idx1 + 2] = b1;

                    // Pixel 2
                    int r2 = y2 + ((351 * v) >> 8);
                    int g2 = y2 - ((179 * v + 86 * u) >> 8);
                    int b2 = y2 + ((443 * u) >> 8);
                    r2 = (r2 < 0) ? 0 : (r2 > 255) ? 255 : r2;
                    g2 = (g2 < 0) ? 0 : (g2 > 255) ? 255 : g2;
                    b2 = (b2 < 0) ? 0 : (b2 > 255) ? 255 : b2;

                    int rgb_idx2 = (y * CAM_W + x + 1) * 3;
                    rgb_data[rgb_idx2] = r2;
                    rgb_data[rgb_idx2 + 1] = g2;
                    rgb_data[rgb_idx2 + 2] = b2;
                }
            }

            // Feed landscape image to model
            ncnn::Mat in = ncnn::Mat::from_pixels_resize(
                rgb_data, ncnn::Mat::PIXEL_RGB, CAM_W, CAM_H, MODEL_SIZE, MODEL_SIZE);

            delete[] rgb_data;

            const float mean_vals[3] = {0.f, 0.f, 0.f};
            const float norm_vals[3] = {1/255.f, 1/255.f, 1/255.f};
            in.substract_mean_normalize(mean_vals, norm_vals);

            // Run inference - extract raw YOLO outputs
            ncnn::Extractor ex = yolo.create_extractor();
            ex.input("data", in);

            ncnn::Mat out_large, out_small;
            ex.extract("120_854", out_large);  // 10x10 for large objects
            ex.extract("129_922", out_small);  // 20x20 for small objects

            // Parse both outputs
            std::vector<Detection> all_detections;
            parse_yolo_layer(out_large, 32, 3, all_detections, 0.2f);  // stride=32, anchors 3-5
            parse_yolo_layer(out_small, 16, 0, all_detections, 0.2f);  // stride=16, anchors 0-2

            // Apply NMS
            nms(all_detections, 0.45f);

            detections = all_detections;

            if (!detections.empty()) {
                printf("Frame %d: %zu detections:\n", frame_count, detections.size());
                for (const auto& det : detections) {
                    printf("  - %s (%.2f) at [%.1f, %.1f] size [%.1f x %.1f]\n",
                           get_class_name(det.class_id), det.confidence,
                           det.x, det.y, det.w, det.h);
                }
            }
        } else {
            detection_persist_counter++;
            if (detection_persist_counter >= 6) {
                detections.clear();
            }
        }

        // Draw bounding boxes (letterboxed display)
        for (const auto& det : detections) {
            // Detection coordinates are in model space (320x320)
            // Scale to camera space (160x120)
            float cam_x = det.x * CAM_W / MODEL_SIZE;
            float cam_y = det.y * CAM_H / MODEL_SIZE;
            float cam_w = det.w * CAM_W / MODEL_SIZE;
            float cam_h = det.h * CAM_H / MODEL_SIZE;

            // Scale to framebuffer letterbox area
            int fb_x1 = (int)((cam_x - cam_w/2) * FB_W / CAM_W);
            int fb_x2 = (int)((cam_x + cam_w/2) * FB_W / CAM_W);
            int fb_y1 = (int)((cam_y - cam_h/2) * scaled_h / CAM_H) + offset_y;
            int fb_y2 = (int)((cam_y + cam_h/2) * scaled_h / CAM_H) + offset_y;

            if (fb_x1 < 0) fb_x1 = 0;
            if (fb_x2 >= FB_W) fb_x2 = FB_W - 1;
            if (fb_y1 < 0) fb_y1 = 0;
            if (fb_y2 >= FB_H) fb_y2 = FB_H - 1;

            draw_bbox(fb_pixels, fb_x1, fb_y1, fb_x2, fb_y2, COLOR_YELLOW);
        }

        // Write to framebuffer
        lseek(fb, 0, SEEK_SET);
        write(fb, fb_pixels, sizeof(fb_pixels));

        frame_count++;
    }

    munmap(map, buf.length);
    close(cam);
    close(fb);

    return 0;
}
