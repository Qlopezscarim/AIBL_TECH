#ifndef PWM_HEADER
#define PWM_HEADER


#include <unistd.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <thread>
#include <sys/stat.h>
#include <string>

struct pwm_paths
{
	// Relics of prior kernels
	/*std::string P9_22 = "/sys/class/pwm/pwm-1:0";
	std::string P9_21 = "/sys/class/pwm/pwm-1:1";
	std::string P9_14 = "/sys/class/pwm/pwm-4:0";
	std::string P9_16 = "/sys/class/pwm/pwm-4:1";
	std::string P8_19 = "/sys/class/pwm/pwm-7:0";
	std::string P8_13 = "/sys/class/pwm/pwm-7:1";*/
	
	std::string P9_22 = "/sys/class/pwm/pwmchip2/pwm0/"; //untested associations
        std::string P9_21 = "/sys/class/pwm/pwmchip2/pwm1/";
        std::string P9_14 = "/sys/class/pwm/pwmchip0/pwm0/";
        std::string P9_16 = "/sys/class/pwm/pwmchip0/pwm1/";
        std::string P8_19 = "/sys/class/pwm/pwmchip1/pwm0/";
        std::string P8_13 = "/sys/class/pwm/pwmchip1/pwm1/";
};


class PWM {

public:
    PWM(const std::string& pwm_path);

    ~PWM();

    void setPeriod(int period_ns);

    void setDutyCycle(int duty_ns);

    void enable();

    void disable();

    void setPolarity(const std::string& polarity);

    static void setPinPWM(const std::string& pin);


private:
    std::string chip_path_;
    std::string pwm_path_;
    int channel_;

    void writeToFile(const std::string& path, const std::string& value);
    bool pathExists(const std::string& path);
};


#endif
