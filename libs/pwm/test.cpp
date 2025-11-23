#include "pwm.h"

// Example usage:
int main() {
    try {

	pwm_paths pin_mapping;



        // PWM0B → P9_22
        PWM pwm22(pin_mapping.P9_14);

        pwm22.setPeriod(1000000);      // 1 kHz
        pwm22.setDutyCycle(100000);    // 50%
        pwm22.setPolarity("normal");
        pwm22.enable();


	PWM pwm21(pin_mapping.P9_16);
	pwm21.setPeriod(1000000);      // 1 kHz
        pwm21.setDutyCycle(100000);    // 50%
        pwm21.setPolarity("normal");
        pwm21.enable();

        std::cout << "PWM running on P9_22 and P9_21 Press Enter to stop." << std::endl;
        std::cin.get();

        pwm22.disable();
	pwm21.disable();
        std::cout << "PWM stopped." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
