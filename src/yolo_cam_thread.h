#pragma once
#include <atomic>
#include <thread>
#include <cstddef>

class CAMERA
{
public:
    // Spawns camera detection thread
    // bbox_area_threshold: minimum bounding box area (width * height) to trigger exit
    // conf_threshold: confidence threshold for detections (default 0.2)
    static void camera_thread(size_t sampling_rate, 
                             float bbox_area_threshold,
                             float conf_threshold = 0.2f);
    
    // Call this to signal the thread to stop cleanly
    static void stop_thread();
    
private:
    static std::atomic<bool> should_stop;
};
