#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "pwm.h" //PWM library


//Handling forceful linking of a C library
#ifdef __cplusplus
extern "C" {
#endif

#include "bb_gpio.h" //GPIO library

#ifdef __cplusplus
}
#endif


struct motor_pwm_paths
{
        std::string P9_22; //untested associations
        std::string P9_21;
        std::string P9_14; //Tested and confirmed alias for 6.17.5-bone14
        std::string P9_16; //Tested and confirmed alias for 6.17.5-bone14
        std::string P8_19; //Tested and confirmed alias for 6.17.5-bone14
        std::string P8_13; //Tested and confirmed alias for 6.17.5-bone14
};




class Motors {
public:
    // Initialization + Cleanup
    static int init();      // returns 0 on success, non-zero on failure
    static void cleanup();

    // Motor 1
    static void motorOneForward();
    static void motorOneBackward();
    static void motorOneDisable();

    // Motor 2
    static void motorTwoForward();
    static void motorTwoBackward();
    static void motorTwoDisable();

    // Motor 3
    static void motorThreeForward();
    static void motorThreeBackward();
    static void motorThreeDisable();

    // Motor 4
    static void motorFourForward();
    static void motorFourBackward();
    static void motorFourDisable();

    // Motor 5
    static void motorFiveForward();
    static void motorFiveBackward();
    static void motorFiveDisable();

    // Motor 6
    static void motorSixForward();
    static void motorSixBackward();
    static void motorSixDisable();

private:
	static PWM* pwm9_22;  // Change to pointers
    	static PWM* pwm9_21;
    	static PWM* pwm9_14;
    	static PWM* pwm9_16;
    	static PWM* pwm8_19;
    	static PWM* pwm8_13;
};
#endif // MOTORS_H

