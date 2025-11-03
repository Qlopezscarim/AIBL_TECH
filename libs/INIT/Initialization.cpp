#include "pwm.h"

int main() {

//Setting up our PWM pins - We want to run it exactly in this order every time 
//so that our linux device tree doesn't swap numbers: this would be really bad 
//and could prevent a motor from being correctly associated with it's pressure
//sensor!
PWM::setPinPWM("P9_22");
PWM::setPinPWM("P9_21");
PWM::setPinPWM("P9_14");
PWM::setPinPWM("P9_16");
PWM::setPinPWM("P8_19");
PWM::setPinPWM("P8_13");


}
