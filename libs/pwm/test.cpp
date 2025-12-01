#include "pwm.h"

// Example usage:
int main() {
    try {

	pwm_paths pin_mapping;



	std::string ehrpwm0_path = PWM::getPWMChipPath("ehrpwm0");
	pin_mapping.P9_22 = ehrpwm0_path + "/pwm0/";
	pin_mapping.P9_21 = ehrpwm0_path + "/pwm1/";

	// ehrpwm1 controls P9_14 and P9_16
	std::string ehrpwm1_path = PWM::getPWMChipPath("ehrpwm1");
	pin_mapping.P9_14 = ehrpwm1_path + "/pwm0/";
	pin_mapping.P9_16 = ehrpwm1_path + "/pwm1/";

	// ehrpwm2 controls P8_19 and P8_13
	std::string ehrpwm2_path = PWM::getPWMChipPath("ehrpwm2");
	pin_mapping.P8_19 = ehrpwm2_path + "/pwm0/";
	pin_mapping.P8_13 = ehrpwm2_path + "/pwm1/";


	PWM pwm9_22(pin_mapping.P9_22);

        pwm9_22.setPeriod(1000000);      // 1 kHz
        pwm9_22.setDutyCycle(100000);    // 50%
        pwm9_22.setPolarity("normal");
        pwm9_22.enable();

        std::cout << "PWM should be running on P9_22 press enter to test next connection." << std::endl;
        std::cin.get();

        pwm9_22.disable();


        PWM pwm9_21(pin_mapping.P9_21);

        pwm9_21.setPeriod(1000000);      // 1 kHz
        pwm9_21.setDutyCycle(100000);    // 50%
        pwm9_21.setPolarity("normal");
        pwm9_21.enable();

        std::cout << "PWM should be running on P9_21 press enter to test next connection." << std::endl;
        std::cin.get();

        pwm9_21.disable();


	PWM pwm9_14(pin_mapping.P9_14);

        pwm9_14.setPeriod(1000000);      // 1 kHz
        pwm9_14.setDutyCycle(100000);    // 50%
        pwm9_14.setPolarity("normal");
        pwm9_14.enable();

        std::cout << "PWM should be running on P9_14 press enter to test next connection." << std::endl;
        std::cin.get();

        pwm9_14.disable();


	PWM pwm9_16(pin_mapping.P9_16);

        pwm9_16.setPeriod(1000000);      // 1 kHz
        pwm9_16.setDutyCycle(100000);    // 50%
        pwm9_16.setPolarity("normal");
        pwm9_16.enable();

        std::cout << "PWM should be running on P9_16 press enter to test next connection." << std::endl;
        std::cin.get();

        pwm9_16.disable();





        PWM pwm8_13(pin_mapping.P8_13);

        pwm8_13.setPeriod(1000000);      // 1 kHz
        pwm8_13.setDutyCycle(100000);    // 50%
        pwm8_13.setPolarity("normal");
        pwm8_13.enable();

	std::cout << "PWM should be running on P8_13 press enter to test next connection." << std::endl;
        std::cin.get();

	pwm8_13.disable();


	PWM pwm8_19(pin_mapping.P8_19);
	pwm8_19.setPeriod(1000000);      // 1 kHz
        pwm8_19.setDutyCycle(100000);    // 50%
        pwm8_19.setPolarity("normal");
        pwm8_19.enable();

	std::cout << "PWM should be running on P8_19 press enter to test next connection." << std::endl;
        std::cin.get();

	pwm8_19.disable();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
