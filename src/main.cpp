/*
//      Code written from reference 2.4.8 txrx_loopback_to_file.cpp (RX)
*/
#include <boost/program_options.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <csignal>
#include <sstream>
#include <csignal>
#include <iostream>
#include "Adc.h"
#include <cstdlib>
#include "joystick_thread.h"
#include "pressure_sensor_thread.h"
#include "PCA9685.h"
#include "yolo_cam_thread.h"

#define LEFT_JOYSTICK_THRESHOLD (0.05f)
#define RIGHT_JOYSTICK_THRESHOLD (1.46f)


void motor_control_loop(size_t channel,size_t goal, double press_thresh, double& press_value, size_t* curr_position, PCA9685& pwm) 
//grossly passing a reference to a global variable (This may blow up)
{
	size_t current_angle = curr_position[channel];

	if(current_angle < goal)
	{
		while(current_angle < goal )//&& press_value > press_thresh)
		{
			current_angle = current_angle + 2;
			pwm.setServoAngle(channel, current_angle);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}
	else if (current_angle > goal)
	{
		while(current_angle > goal )//&& press_value > press_thresh)
                {
                        current_angle = current_angle - 2;
                        pwm.setServoAngle(channel, current_angle);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
	}
	curr_position[channel] = current_angle;
}



namespace po = boost::program_options;

//Tracks if first packet - lets us know if we want to store it to the output file
bool first_packet = true;

double current_joy_reading = 0.62989f;

double current_sensor_one_reading = 1.8f;
double current_sensor_two_reading = 1.8f;
double current_sensor_three_reading = 1.8f;
double current_sensor_four_reading = 1.8f;
double current_sensor_five_reading = 1.8f;

//Motor current positions!
size_t motor_positions[6] = {0, 0, 0, 0, 0, 0};

// Add global variables at the top with your other sensor readings
bool bbox_threshold_met = false;
float last_detection_area = 0.0f;
int last_detection_class = -1;


// Interrupt signal handler
bool stop_signal_called = false;
void sig_int_handler(int) {
    stop_signal_called = true;
}

int main(int argc, char *argv[]) {
    //INPUT HANDLING WITH BOOST
    bool motor_1, motor_2, motor_3, motor_4, motor_5, motor_6;

    // setup the program options
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help", "help message")
        ("motor1", po::value<bool>(&motor_1)->default_value(false), "If motor 1 is on or off")
        ("motor2", po::value<bool>(&motor_2)->default_value(false), "If motor 2 is on or off")
        ("motor3", po::value<bool>(&motor_3)->default_value(false), "If motor 3 is on or off")
        ("motor4", po::value<bool>(&motor_4)->default_value(false), "If motor 4 is on or off")
        ("motor5", po::value<bool>(&motor_5)->default_value(false), "If motor 5 is on or off")
	("motor6", po::value<bool>(&motor_6)->default_value(false), "If motor 6 is on or off")
    ;
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    //END OF INPUT HANDLING WITH BOOST

    //Print out help message if neccessary/asked for
    if (vm.count("help")) {
        std::cout << "AIBL_MAIN " << desc << std::endl;
        return ~0;
    }

    // Set the signal handler
    std::signal(SIGINT, &sig_int_handler);
    // Keep running until CTRL-C issued
    size_t count = 0;

/*Handling global setup for the kernel - MUST BE RUN" */

    //int ret = std::system("../libs/INIT/INIT");  // Runs the INIT program
    //if (ret != 0) {
    //    std::cout << "Couldn't find initialization for this kernel instance; please ensure it exists" << std::endl;
    //}

	// These lines cause our Joystick and pressure sensors to actually start being used
	size_t sampling_rate_joystick = 250;
	//need to reset so no action is detected
	current_joy_reading = 0.62989f;

	std::thread joystick_sampler(JOYSTICK::joystick_thread,
	std::ref(sampling_rate_joystick)
	);



	size_t sampling_rate_pressure_sensor = 10;
	//need to reset so no action is detected
	current_sensor_one_reading = 1.8f;
        current_sensor_two_reading = 1.8f;
	current_sensor_three_reading = 1.8f;
	current_sensor_four_reading = 1.8f;
	current_sensor_five_reading = 1.8f;

	std::thread pressure_sampler(PRESSURE_SENSOR::pressure_sensor_thread,
	std::ref(sampling_rate_pressure_sensor)
	);
	// END OF THREAD SPAWNS



	// Create PCA9685 instance on I2C bus 2 (P9_19/P9_20) at default address 0x40
        PCA9685 pwm("/dev/i2c-2", 0x40);

        std::cout << "Initializing PCA9685..." << std::endl;
        pwm.init();
        std::cout << "PCA9685 initialized successfully!" << std::endl;


//try {
        // Create PCA9685 instance on I2C bus 2 (P9_19/P9_20) at default address 0x40
        //PCA9685 pwm("/dev/i2c-2", 0x40);

        //std::cout << "Initializing PCA9685..." << std::endl;
        //pwm.init();
        //std::cout << "PCA9685 initialized successfully!" << std::endl;

        // Example 1: Test servo control on channel 0
	size_t init_one 	=	150;
	size_t init_two 	=	150;
	size_t init_three 	=	150;
	size_t init_four 	=	150;
	size_t init_five 	=	150;
	size_t init_six 	=	150;

	motor_positions[0] = init_one;
	motor_positions[1] = init_two;
	motor_positions[2] = init_three;
	motor_positions[3] = init_four;
	motor_positions[4] = init_five;
	motor_positions[5] = init_six;


        std::cout << "\n=== Setting all servo to zero ===" << std::endl;
        std::cout << "Moving ALL servo to 0 degrees..." << std::endl;
        pwm.setServoAngle(0, init_one);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	pwm.setServoAngle(1, init_two);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	pwm.setServoAngle(2, init_three);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	pwm.setServoAngle(3, init_four);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	pwm.setServoAngle(4, init_five);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	pwm.setServoAngle(5, init_six);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
        std::this_thread::sleep_for(std::chrono::seconds(1));
/*
        std::cout << "Moving servo to 90 degrees..." << std::endl;
        pwm.setServoAngle(0, 90);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Moving servo to 180 degrees..." << std::endl;
        pwm.setServoAngle(0, 180);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Moving servo back to 90 degrees..." << std::endl;
        pwm.setServoAngle(0, 90);
        std::this_thread::sleep_for(std::chrono::seconds(1));



        // Example 2: Test servo control on channel 0
        std::cout << "\n=== Testing Servo on Channel 1 ===" << std::endl;
        std::cout << "Moving servo to 0 degrees..." << std::endl;
        pwm.setServoAngle(1, 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Moving servo to 90 degrees..." << std::endl;
        pwm.setServoAngle(1, 90);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Moving servo to 180 degrees..." << std::endl;
        pwm.setServoAngle(1, 180);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Moving servo back to 90 degrees..." << std::endl;
        pwm.setServoAngle(1, 90);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Example 3: Test raw PWM control on channel 2
        std::cout << "\n=== Testing Raw PWM on Channel 2 ===" << std::endl;
        std::cout << "PWM value 0 (off)..." << std::endl;
        pwm.setChannelPWM(2, 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "PWM value 1024 (25%)..." << std::endl;
        pwm.setChannelPWM(2, 1024);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "PWM value 2048 (50%)..." << std::endl;
        pwm.setChannelPWM(2, 2048);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "PWM value 4095 (100%)..." << std::endl;
        pwm.setChannelPWM(2, 4095);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Turn off all channels
        std::cout << "\nTurning off all channels..." << std::endl;
        pwm.allOff();

        std::cout << "\nTest complete!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
*/
/*
    // Spawn the camera thread
    size_t sampling_rate_camera = 100;  // 100ms between frames (10 FPS)
    float bbox_area_threshold = 5000.0f;  // Minimum bbox area (width*height) in model space
    float confidence_threshold = 0.2f;  // Detection confidence threshold

    std::thread camera_sampler(CAMERA::camera_thread,
        sampling_rate_camera,
        bbox_area_threshold,
        confidence_threshold
    );

    // Later, to wait for detection or force stop
    camera_sampler.join();  // This will block until threshold is met or stop is called


// we detected an object, what do we want to do now?
if ( last_detection_class == 41) //cup
*/
if(true)
{

	size_t goal_angle_motor_one = 160;
	size_t goal_angle_motor_two = 160;
	size_t goal_angle_motor_three = 160;
	size_t goal_angle_motor_four = 160;
	size_t goal_angle_motor_five = 160;
	size_t goal_angle_motor_six = 160;
	//Doing each motor's control loop:
	motor_control_loop(0,goal_angle_motor_one, 0.6f, current_sensor_one_reading, motor_positions,pwm);
	motor_control_loop(1,goal_angle_motor_two, 0.6f, current_sensor_one_reading, motor_positions,pwm);
	motor_control_loop(2,goal_angle_motor_three, 0.6f, current_sensor_two_reading, motor_positions,pwm);
	motor_control_loop(3,goal_angle_motor_four, 0.6f, current_sensor_three_reading, motor_positions,pwm);
	motor_control_loop(4,goal_angle_motor_five, 0.6f, current_sensor_four_reading, motor_positions,pwm);
	motor_control_loop(5,goal_angle_motor_six, 0.6f, current_sensor_five_reading, motor_positions,pwm);


}



    while(not stop_signal_called)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // sleep for 1ms to make sure can catch CTRL-C
        count++;
        if(count % 300 == 0)
        {
		//std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "Voltage for ADC 0: " << current_sensor_one_reading << std::endl;
		std::cout << "Voltage for ADC 1: \t" << current_sensor_two_reading << std::endl;
		std::cout << "Voltage for ADC 2: \t\t" << current_sensor_three_reading << std::endl;
		std::cout << "Voltage for ADC 3: \t\t\t" << current_sensor_four_reading << std::endl;
		std::cout << "Voltage for ADC 4: " << current_sensor_five_reading << std::endl;
		//std::cout << "Voltage for ADC 1: " << ADC::readVoltage(1) << std::endl;
		//std::cout << "Voltage for ADC 2: " << ADC::readVoltage(2) << std::endl;
		//std::cout << "Voltage for ADC 3: " << ADC::readVoltage(3) << std::endl;
		//std::cout << "Voltage for ADC 4: " << ADC::readVoltage(4) << std::endl;
		//std::cout << "Voltage for ADC 5: " << ADC::readVoltage(5) << std::endl;

		if(current_joy_reading < LEFT_JOYSTICK_THRESHOLD)
		{
			std::cout << "\t\t\t JOYSTICK PUSHED LEFT";
		}
		if(current_joy_reading > RIGHT_JOYSTICK_THRESHOLD)
		{
			std::cout << "\t\t\t JOYSTICK PUSHED RIGHT";
		}

		std::cout << "Voltage for ADC 6: " << current_joy_reading << std::endl;
		
        }
    }


    //These lines close off our threads cleanly
    // At cleanup/shutdown
	if (joystick_sampler.joinable()) {
    	joystick_sampler.join();
	}

	if (pressure_sampler.joinable()) {
    	pressure_sampler.join();
	}

//	if (camera_sampler.joinable()) {
//    	camera_sampler.join();
//	}

}

