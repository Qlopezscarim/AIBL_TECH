#include "motors.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


int main()
{

Motors::init();

/*MAPPINGS
PWM Motors::pwm9_22(pwm_paths.P9_22); //maps to motor 5
PWM Motors::pwm9_21(pwm_paths.P9_21); //maps to motor 6
PWM Motors::pwm9_14(pwm_paths.P9_14); //maps to motor 1
PWM Motors::pwm9_16(pwm_paths.P9_16); //maps to motor 2
PWM Motors::pwm8_19(pwm_paths.P8_19); //maps to motor 3
PWM Motors::pwm8_13(pwm_paths.P8_13); //maps to motor 4
*/

for(int i =0; i<40 ; i++)
{

std::cout << "ENABLING MOTOR 1 \tP9_14" << std::endl;
Motors::motorOneForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 1 \tP9_14" << std::endl;
Motors::motorOneDisable();
usleep(5000000);
std::cout << "ENABLING MOTOR 2 \tP9_16" << std::endl;
Motors::motorTwoForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 2 \tP9_16" << std::endl;
Motors::motorTwoDisable();
usleep(5000000);
std::cout << "ENABLING MOTOR 3 \tP8_19" << std::endl;
Motors::motorThreeForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 3\tP8_19" << std::endl;
Motors::motorThreeDisable();
usleep(5000000);
std::cout << "ENABLING MOTOR 4\tP8_13" << std::endl;
Motors::motorFourForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 4\tP8_13" << std::endl;
Motors::motorFourDisable();
usleep(5000000);
std::cout << "ENABLING MOTOR 5\tP9_22" << std::endl;
Motors::motorFiveForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 5\tP9_22" << std::endl;
Motors::motorFiveDisable();
usleep(5000000);
std::cout << "ENABLING MOTOR 6\tP9_21" << std::endl;
Motors::motorSixForward();
usleep(5000000);
std::cout << "DISABLING MOTOR 6\tP9_21" << std::endl;
Motors::motorSixDisable();
usleep(5000000);


}

Motors::cleanup();

}
