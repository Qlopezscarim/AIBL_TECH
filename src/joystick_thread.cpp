#include "joystick_thread.h"

//Global variable we are editing
extern double current_joy_reading;
extern bool stop_signal_called;

void JOYSTICK::joystick_thread(size_t sampling_rate)
{

	while(not stop_signal_called)
	{
		current_joy_reading = ADC::readVoltage(6);
		std::this_thread::sleep_for(std::chrono::milliseconds(sampling_rate));
	}
	return;
}
