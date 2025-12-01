#include <cmath>
#include <csignal>
#include <fstream>
#include <functional>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <sstream>
#include <csignal>
#include <iostream>
#include "Adc.h"
#include <cstdlib>
#pragma once


class PRESSURE_SENSOR
{
public:
//spawns main RX thread according to passes parameters
static void pressure_sensor_thread(size_t sampling_rate);
};
