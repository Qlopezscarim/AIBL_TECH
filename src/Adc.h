#include <iostream>
#include <fstream>
#include <string>

class ADC {
public:
    static int read(int channel);
    static float readVoltage(int channel);
};
