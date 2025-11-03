#include "pwm.h"

PWM::PWM(const std::string& pwm_path)
    {
	pwm_path_ = pwm_path;  // pass "/sys/class/pwm/pwm-1:0"

    	if (!pathExists(pwm_path_)) 
	{
        throw std::runtime_error("PWM path does not exist: " + pwm_path_);
    	}
        

	//could be bad
	// Export PWM if it doesn't exist
        std::ifstream test(pwm_path_);
        if (!test.good()) 
	{
            writeToFile(chip_path_ + "/export", std::to_string(channel_));
            // Wait a moment for sysfs to create the folder
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }


    }

PWM::~PWM() 
	{
        	disable();
        	writeToFile(chip_path_ + "/unexport", std::to_string(channel_));
	}

void PWM::setPeriod(int period_ns) 
    {
        writeToFile(pwm_path_ + "/period", std::to_string(period_ns));
    }

void PWM::setDutyCycle(int duty_ns) 
    {
        writeToFile(pwm_path_ + "/duty_cycle", std::to_string(duty_ns));
    }

void PWM::enable() 
    {
        writeToFile(pwm_path_ + "/enable", "1");
    }

void PWM::disable() 
    {
        writeToFile(pwm_path_ + "/enable", "0");
    }

void PWM::setPolarity(const std::string& polarity) 
    {
        writeToFile(pwm_path_ + "/polarity", polarity);
    }


void PWM::setPinPWM(const std::string& pin) {
    std::string cmd = "config-pin " + pin + " pwm";
    if (std::system(cmd.c_str()) != 0) {
        throw std::runtime_error("Failed to configure " + pin + " as PWM");
    }
}



void PWM::writeToFile(const std::string& path, const std::string& value) 
{
        std::ofstream fs(path);
        if (!fs.is_open()) 
	{
            std::stringstream ss;
            ss << "Failed to open " << path;
            throw std::runtime_error(ss.str());
        }
        fs << value;
        if (fs.fail()) 
	{
            std::stringstream ss;
            ss << "Failed to write value '" << value << "' to " << path;
            throw std::runtime_error(ss.str());
        }
}


bool PWM::pathExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

