// bb_gpio.h - BeagleBone GPIO API for pins 44,45,46,47,65,68
#ifndef BB_GPIO_H
#define BB_GPIO_H

#include <stdint.h>

// Return codes
#define GPIO_SUCCESS  0
#define GPIO_ERROR   -1

// GPIO pin definitions
#define GPIO_PIN_44  44
#define GPIO_PIN_45  45
#define GPIO_PIN_46  46
#define GPIO_PIN_47  47
#define GPIO_PIN_65  65
#define GPIO_PIN_68  68

// Initialize GPIO system - must be called first
// Returns GPIO_SUCCESS on success, GPIO_ERROR on failure
int gpio_init(void);

// Cleanup GPIO system - call when done
void gpio_cleanup(void);

// Set individual pins HIGH
int gpio_high_44(void);
int gpio_high_45(void);
int gpio_high_46(void);
int gpio_high_47(void);
int gpio_high_65(void);
int gpio_high_68(void);

// Set individual pins LOW
int gpio_low_44(void);
int gpio_low_45(void);
int gpio_low_46(void);
int gpio_low_47(void);
int gpio_low_65(void);
int gpio_low_68(void);

// Set all pins HIGH at once
int gpio_all_high(void);

// Set all pins LOW at once
int gpio_all_low(void);

#endif // BB_GPIO_H
