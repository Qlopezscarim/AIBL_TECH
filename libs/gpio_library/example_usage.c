// example_usage.c - Example of using the BeagleBone GPIO API
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "bb_gpio.h"

// Signal handler for clean exit
void signal_handler(int sig) {
    printf("\nCleaning up...\n");
    gpio_cleanup();
    _exit(0);
}

int main() {
    // Setup signal handler for CTRL-C
    signal(SIGINT, signal_handler);

    // Initialize GPIO system
    printf("Initializing GPIO...\n");
    if (gpio_init() != GPIO_SUCCESS) {
        fprintf(stderr, "Failed to initialize GPIO\n");
        return 1;
    }
    printf("GPIO initialized successfully\n");

    // Example 1: Toggle individual pins sequentially
    printf("\nExample 1: Sequential toggle of all pins\n");
    for (int i = 0; i < 3; i++) {
        gpio_high_44(); usleep(200000);
        gpio_low_44();
        
        gpio_high_45(); usleep(200000);
        gpio_low_45();
        
        gpio_high_46(); usleep(200000);
        gpio_low_46();
        
        gpio_high_47(); usleep(200000);
        gpio_low_47();
        
        gpio_high_65(); usleep(200000);
        gpio_low_65();
        
        gpio_high_68(); usleep(200000);
        gpio_low_68();
    }

    // Example 2: All pins on/off together
    printf("\nExample 2: All pins on/off together\n");
    for (int i = 0; i < 5; i++) {
        gpio_all_high();
        usleep(500000);
        gpio_all_low();
        usleep(500000);
    }

    // Example 3: Individual pin control
    printf("\nExample 3: Individual pin patterns\n");
    for (int i = 0; i < 3; i++) {
        // Odd pins
        gpio_high_45();
        gpio_high_47();
        gpio_high_65();
        usleep(500000);
        gpio_low_45();
        gpio_low_47();
        gpio_low_65();
        
        // Even pins
        gpio_high_44();
        gpio_high_46();
        gpio_high_68();
        usleep(500000);
        gpio_low_44();
        gpio_low_46();
        gpio_low_68();
    }

    // Cleanup
    printf("\nDone. Cleaning up...\n");
    gpio_cleanup();
    
    return 0;
}
