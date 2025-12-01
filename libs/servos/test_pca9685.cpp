#include "PCA9685.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        // Create PCA9685 instance on I2C bus 2 (P9_19/P9_20) at default address 0x40
        PCA9685 pwm("/dev/i2c-2", 0x40);
        
        std::cout << "Initializing PCA9685..." << std::endl;
        pwm.init();
        std::cout << "PCA9685 initialized successfully!" << std::endl;
        
        // Example 1: Test servo control on channel 0
        std::cout << "\n=== Testing Servo on Channel 0 ===" << std::endl;
        std::cout << "Moving servo to 0 degrees..." << std::endl;
        pwm.setServoAngle(0, 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
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
    
    return 0;
}

// Compile with:
// g++ -o test_pca9685 test_pca9685.cpp PCA9685.cpp -std=c++11
//
// Run with:
// sudo ./test_pca9685
//
// Note: You may need sudo for I2C access, or add your user to the i2c group:
// sudo usermod -a -G i2c $USER
// Then logout and login again
