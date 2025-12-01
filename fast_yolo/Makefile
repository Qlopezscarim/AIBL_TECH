all:
	g++ -o yolo_cam yolo_cam.cpp     -I/home/beagle/ncnn/build/install/include/ncnn     -L/home/beagle/ncnn/build/install/lib     -lncnn -lpthread -fopenmp     -march=armv7-a -mfpu=neon -mfloat-abi=hard     -O3
lib:
	g++ -o yolo_cam yolo_cam_thread.cpp     -I/home/beagle/ncnn/build/install/include/ncnn     -L/home/beagle/ncnn/build/install/lib     -lncnn -lpthread -fopenmp     -march=armv7-a -mfpu=neon -mfloat-abi=hard     -O3
