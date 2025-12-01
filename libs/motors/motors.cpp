#include "motors.h"

motor_pwm_paths pwm_paths;




//PWM Motors::pwm9_22(pwm_paths.P9_22); //maps to motor 5
//PWM Motors::pwm9_21(pwm_paths.P9_21); //maps to motor 6
//PWM Motors::pwm9_14(pwm_paths.P9_14); //maps to motor 1
//PWM Motors::pwm9_16(pwm_paths.P9_16); //maps to motor 2
//PWM Motors::pwm8_19(pwm_paths.P8_19); //maps to motor 3
//PWM Motors::pwm8_13(pwm_paths.P8_13); //maps to motor 4


// Define the static PWM pointers as nullptr
PWM* Motors::pwm9_22 = nullptr;
PWM* Motors::pwm9_21 = nullptr;
PWM* Motors::pwm9_14 = nullptr;
PWM* Motors::pwm9_16 = nullptr;
PWM* Motors::pwm8_19 = nullptr;
PWM* Motors::pwm8_13 = nullptr;

int Motors::init()
{
    // Initialize GPIO system
    printf("Initializing GPIO...\n");
    if (gpio_init() != GPIO_SUCCESS) {
        fprintf(stderr, "Failed to initialize GPIO\n");
        return 1;
    }
    printf("GPIO (Dependency of motors) initialized successfully\n");

    //need this stuff to run sequentially//
    std::string ehrpwm0_path = PWM::getPWMChipPath("ehrpwm0");
    pwm_paths.P9_22 = ehrpwm0_path + "/pwm0/";
    pwm_paths.P9_21 = ehrpwm0_path + "/pwm1/";

    // ehrpwm1 controls P9_14 and P9_16
    std::string ehrpwm1_path = PWM::getPWMChipPath("ehrpwm1");
    pwm_paths.P9_14 = ehrpwm1_path + "/pwm0/";
    pwm_paths.P9_16 = ehrpwm1_path + "/pwm1/";

    // ehrpwm2 controls P8_19 and P8_13
    std::string ehrpwm2_path = PWM::getPWMChipPath("ehrpwm2");
    pwm_paths.P8_19 = ehrpwm2_path + "/pwm0/";
    pwm_paths.P8_13 = ehrpwm2_path + "/pwm1/";

    // Create PWM objects with new
    pwm9_22 = new PWM(pwm_paths.P9_22); //maps to motor 5
    pwm9_21 = new PWM(pwm_paths.P9_21); //maps to motor 6
    pwm9_14 = new PWM(pwm_paths.P9_14); //maps to motor 1
    pwm9_16 = new PWM(pwm_paths.P9_16); //maps to motor 2
    pwm8_19 = new PWM(pwm_paths.P8_19); //maps to motor 3
    pwm8_13 = new PWM(pwm_paths.P8_13); //maps to motor 4

    return 0;
}


void Motors::motorOneForward()
{
	gpio_high_68();
	Motors::pwm9_14->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_14->setDutyCycle(100000);    // 50%
        Motors::pwm9_14->setPolarity("normal");
        Motors::pwm9_14->enable();

}

void Motors::motorOneBackward()
{
        gpio_low_68();
	Motors::pwm9_14->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_14->setDutyCycle(100000);    // 50%
        Motors::pwm9_14->setPolarity("normal");
        Motors::pwm9_14->enable();

}

void Motors::motorOneDisable()
{

        Motors::pwm9_14->setDutyCycle(0);    // 50%
        Motors::pwm9_14->disable();
}



void Motors::motorTwoForward()
{
        gpio_high_44();
	Motors::pwm9_16->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_16->setDutyCycle(100000);    // 50%
        Motors::pwm9_16->setPolarity("normal");
        Motors::pwm9_16->enable();

}

void Motors::motorTwoBackward()
{
        gpio_low_44();
	Motors::pwm9_16->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_16->setDutyCycle(100000);    // 50%
        Motors::pwm9_16->setPolarity("normal");
        Motors::pwm9_16->enable();

}

void Motors::motorTwoDisable()
{

        Motors::pwm9_16->setDutyCycle(0);    // 50%
        Motors::pwm9_16->disable();
}

void Motors::motorThreeForward()
{
        gpio_high_46();
	Motors::pwm8_19->setPeriod(1000000);      // 1 kHz
        Motors::pwm8_19->setDutyCycle(100000);    // 50%
        Motors::pwm8_19->setPolarity("normal");
        Motors::pwm8_19->enable();

}

void Motors::motorThreeBackward()
{
        gpio_low_46();
	Motors::pwm8_19->setPeriod(1000000);      // 1 kHz
        Motors::pwm8_19->setDutyCycle(100000);    // 50%
        Motors::pwm8_19->setPolarity("normal");
        Motors::pwm8_19->enable();

}

void Motors::motorThreeDisable()
{

        Motors::pwm8_19->setDutyCycle(0);    // 50%
        Motors::pwm8_19->disable();
}

void Motors::motorFourForward()
{
        gpio_high_65();
	Motors::pwm8_13->setPeriod(1000000);      // 1 kHz
        Motors::pwm8_13->setDutyCycle(100000);    // 50%
        Motors::pwm8_13->setPolarity("normal");
        Motors::pwm8_13->enable();

}

void Motors::motorFourBackward()
{
        gpio_low_65();
	Motors::pwm8_13->setPeriod(1000000);      // 1 kHz
        Motors::pwm8_13->setDutyCycle(100000);    // 50%
        Motors::pwm8_13->setPolarity("normal");
        Motors::pwm8_13->enable();

}

void Motors::motorFourDisable()
{

        Motors::pwm8_13->setDutyCycle(0);    // 50%
        Motors::pwm8_13->disable();
}

void Motors::motorFiveForward()
{
        gpio_high_45();
	Motors::pwm9_21->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_21->setDutyCycle(100000);    // 50%
        Motors::pwm9_21->setPolarity("normal");
        Motors::pwm9_21->enable();

}

void Motors::motorFiveBackward()
{
        gpio_low_45();
	Motors::pwm9_21->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_21->setDutyCycle(100000);    // 50%
        Motors::pwm9_21->setPolarity("normal");
        Motors::pwm9_21->enable();

}

void Motors::motorFiveDisable()
{

        Motors::pwm9_21->setDutyCycle(0);    // 50%
        Motors::pwm9_21->disable();
}

void Motors::motorSixForward()
{
	gpio_high_47();
	Motors::pwm9_21->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_21->setDutyCycle(100000);    // 50%
        Motors::pwm9_21->setPolarity("normal");
        Motors::pwm9_21->enable();

}

void Motors::motorSixBackward()
{
        gpio_low_47();
	Motors::pwm9_21->setPeriod(1000000);      // 1 kHz
        Motors::pwm9_21->setDutyCycle(100000);    // 50%
        Motors::pwm9_21->setPolarity("normal");
        Motors::pwm9_21->enable();

}

void Motors::motorSixDisable()
{

        Motors::pwm9_21->setDutyCycle(0);    // 50%
        Motors::pwm9_21->disable();
}


void Motors::cleanup()
{
	// Cleanup

	delete Motors::pwm9_22;
	delete Motors::pwm9_21;
	delete Motors::pwm9_14;
	delete Motors::pwm9_16;
	delete Motors::pwm8_19;
	delete Motors::pwm8_13;


	printf("\nDone. Cleaning up...\n");
	gpio_cleanup();
}

