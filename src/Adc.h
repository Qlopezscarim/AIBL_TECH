#include <iostream>
#include <fstream>
#include <string>
#pragma once

class ADC {
public:
    static int read(int channel);
    static float readVoltage(int channel);
};
