#include "Pwm.h"
#include <fstream>
#include <string>
#include <unistd.h>

void writeFile(const std::string &path, const std::string &value) {
    std::ofstream fs(path);
    fs << value;
}

PWM::PWM()
{
	// Private constructor
    	std::cout << "PWM singleton created" << std::endl;
}


// Destructor
PWM::~PWM() 
{
    std::cout << "PWM Singleton destroyed" << std::endl;
}

void PWM::enable()
{
    const std::string pwmPath = "/sys/class/pwm/pwmchip0/pwm0/";

    // Export PWM if not already exported
    writeFile("/sys/class/pwm/pwmchip0/export", "0");
    usleep(100000); // wait 100ms

    // Set period and duty cycle
    writeFile(pwmPath + "period", "1000000");      // 1 ms period
    writeFile(pwmPath + "duty_cycle", "500000");   // 50% duty
    writeFile(pwmPath + "enable", "1");

    sleep(5);

    // Turn it off
    writeFile(pwmPath + "enable", "0");
}

// Accessor for the singleton instance (thread-safe)
PWM& PWM::getInstance() {
    static PWM instance; // Guaranteed to be initialized once only
    return instance;
}
