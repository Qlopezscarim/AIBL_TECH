#include "pwm.h"

// Example usage:
int main() {
    try {

	pwm_paths pin_mapping;


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
