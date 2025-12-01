#include "joystick_thread.h"
#include "pressure_sensor_thread.h"

//Global variable we are editing
extern double current_sensor_one_reading;
extern double current_sensor_two_reading;
extern double current_sensor_three_reading;
extern double current_sensor_four_reading;
extern double current_sensor_five_reading;

extern bool stop_signal_called;

void PRESSURE_SENSOR::pressure_sensor_thread(size_t sampling_rate)
{
	while(not stop_signal_called)
	{
		current_sensor_one_reading = ADC::readVoltage(0);
		current_sensor_two_reading = ADC::readVoltage(1);
		current_sensor_three_reading = ADC::readVoltage(2);
		current_sensor_four_reading = ADC::readVoltage(3);
		current_sensor_five_reading = ADC::readVoltage(4);
		std::this_thread::sleep_for(std::chrono::milliseconds(sampling_rate));
	}
	return;
}
