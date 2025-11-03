#include "Adc.h"
int ADC::read(int channel) 
{
	if (channel < 0 || channel > 6) 
	{
		std::cerr << "Invalid ADC channel: " << channel << std::endl;
		return -1;
	}
        
        std::string path = "/sys/bus/iio/devices/iio:device0/in_voltage" 
                          + std::to_string(channel) + "_raw";
        
        std::ifstream fs(path);
        if (!fs.is_open()) 
	{
            std::cerr << "Failed to open: " << path << std::endl;
            return -1;
        }
        
        int value;
        fs >> value;
        return value;
    }
    
float ADC::readVoltage(int channel) 
	{
        	int raw = read(channel);
        	if (raw < 0) return -1.0f;
        	return (raw / 4095.0f) * 1.8f;
    	}
