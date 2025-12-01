CXX = g++

# Base flags for normal code
BASE_CPPFLAGS = -O3 -DUSE_VOLK -I../libs/servos

# Aggressive flags for YOLO camera thread (NEON optimizations)
YOLO_CPPFLAGS = -O3 -march=armv7-a -mfpu=neon -mfloat-abi=hard -fopenmp \
                -I/home/beagle/ncnn/build/install/include/ncnn

# Libraries
BASE_LDFLAGS = -lboost_program_options
YOLO_LDFLAGS = -L/home/beagle/ncnn/build/install/lib -lncnn -lpthread -fopenmp

# Combined flags
LDFLAGS = $(BASE_LDFLAGS) $(YOLO_LDFLAGS)

OBJDIR := ../build
common_hdrs = Adc.h joystick_thread.h pressure_sensor_thread.h yolo_cam_thread.h

# Object files - added yolo_cam_thread.o
OBJS = $(OBJDIR)/main.o \
       $(OBJDIR)/Adc.o \
       $(OBJDIR)/joystick_thread.o \
       $(OBJDIR)/pressure_sensor_thread.o \
       $(OBJDIR)/yolo_cam_thread.o \
       ../libs/servos/PCA9685.o

all: AIBL

AIBL: $(common_hdrs) $(OBJDIR) $(OBJS)
	$(CXX) -o AIBL $(OBJS) $(LDFLAGS)

$(OBJDIR):
	mkdir -p $@

# Normal compilation for most files
$(OBJDIR)/main.o: main.cpp $(common_hdrs)
	$(CXX) $(BASE_CPPFLAGS) -c $< -o $@

$(OBJDIR)/Adc.o: Adc.cpp $(common_hdrs)
	$(CXX) $(BASE_CPPFLAGS) -c $< -o $@

$(OBJDIR)/joystick_thread.o: joystick_thread.cpp $(common_hdrs)
	$(CXX) $(BASE_CPPFLAGS) -c $< -o $@

$(OBJDIR)/pressure_sensor_thread.o: pressure_sensor_thread.cpp $(common_hdrs)
	$(CXX) $(BASE_CPPFLAGS) -c $< -o $@

# Aggressive NEON compilation for camera thread
$(OBJDIR)/yolo_cam_thread.o: yolo_cam_thread.cpp $(common_hdrs)
	$(CXX) $(YOLO_CPPFLAGS) -c $< -o $@

clean:
	rm -rf AIBL $(OBJDIR)

.PHONY: all clean
