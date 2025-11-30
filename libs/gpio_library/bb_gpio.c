// bb_gpio.c - BeagleBone GPIO API Implementation
#include "bb_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// Hardware memory addresses
#define GPIO1_BASE_ADDR 0x4804C000
#define GPIO2_BASE_ADDR 0x481AC000
#define GPIO_SIZE       0x1000

#define CM_PER_BASE_ADDR 0x44E00000
#define CM_PER_SIZE      0x400

// Clock Module registers
#define CM_PER_GPIO1_CLKCTRL_OFFSET 0xAC
#define CM_PER_GPIO2_CLKCTRL_OFFSET 0xB0
#define CM_PER_GPIO3_CLKCTRL_OFFSET 0xB4
#define CLOCK_ENABLE_VALUE          0x2

// GPIO register offsets
#define GPIO_OE_OFFSET           0x134
#define GPIO_SETDATAOUT_OFFSET   0x194
#define GPIO_CLEARDATAOUT_OFFSET 0x190

// GPIO pin to bit mappings
// GPIO44-47 are on GPIO1 bank, bits 12-15
#define GPIO44_BANK  1
#define GPIO44_BIT   12
#define GPIO45_BANK  1
#define GPIO45_BIT   13
#define GPIO46_BANK  1
#define GPIO46_BIT   14
#define GPIO47_BANK  1
#define GPIO47_BIT   15

// GPIO65,68 are on GPIO2 bank, bits 1,4
#define GPIO65_BANK  2
#define GPIO65_BIT   1
#define GPIO68_BANK  2
#define GPIO68_BIT   4

// Internal state
static volatile uint32_t *gpio1_map = NULL;
static volatile uint32_t *gpio2_map = NULL;
static int mem_fd = -1;
static int initialized = 0;

// Helper: Access /dev/mem and map memory region
static void* map_memory(off_t base_addr, size_t size) {
    void *mapped = mmap(NULL, size, PROT_READ | PROT_WRITE, 
                        MAP_SHARED, mem_fd, base_addr);
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    return mapped;
}

// Helper: Set a GPIO pin high
static int set_pin_high(int bank, int bit) {
    if (!initialized) {
        fprintf(stderr, "GPIO not initialized. Call gpio_init() first.\n");
        return GPIO_ERROR;
    }
    
    volatile uint32_t *gpio_base = (bank == 1) ? gpio1_map : gpio2_map;
    *(gpio_base + GPIO_SETDATAOUT_OFFSET/4) = (1 << bit);
    return GPIO_SUCCESS;
}

// Helper: Set a GPIO pin low
static int set_pin_low(int bank, int bit) {
    if (!initialized) {
        fprintf(stderr, "GPIO not initialized. Call gpio_init() first.\n");
        return GPIO_ERROR;
    }
    
    volatile uint32_t *gpio_base = (bank == 1) ? gpio1_map : gpio2_map;
    *(gpio_base + GPIO_CLEARDATAOUT_OFFSET/4) = (1 << bit);
    return GPIO_SUCCESS;
}

int gpio_init(void) {
    if (initialized) {
        fprintf(stderr, "GPIO already initialized\n");
        return GPIO_SUCCESS;
    }

    // Open /dev/mem
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        perror("Failed to open /dev/mem");
        return GPIO_ERROR;
    }

    // Map clock module to enable GPIO clocks
    volatile uint32_t *cm_per = map_memory(CM_PER_BASE_ADDR, CM_PER_SIZE);
    if (!cm_per) {
        close(mem_fd);
        mem_fd = -1;
        return GPIO_ERROR;
    }

    // Enable clocks for GPIO1, GPIO2, GPIO3
    *(cm_per + CM_PER_GPIO1_CLKCTRL_OFFSET/4) = CLOCK_ENABLE_VALUE;
    *(cm_per + CM_PER_GPIO2_CLKCTRL_OFFSET/4) = CLOCK_ENABLE_VALUE;
    *(cm_per + CM_PER_GPIO3_CLKCTRL_OFFSET/4) = CLOCK_ENABLE_VALUE;
    
    usleep(1000); // Allow clocks to stabilize

    // Unmap clock module - no longer needed
    munmap((void*)cm_per, CM_PER_SIZE);

    // Map GPIO banks
    gpio1_map = map_memory(GPIO1_BASE_ADDR, GPIO_SIZE);
    gpio2_map = map_memory(GPIO2_BASE_ADDR, GPIO_SIZE);
    
    if (!gpio1_map || !gpio2_map) {
        if (gpio1_map) munmap((void*)gpio1_map, GPIO_SIZE);
        if (gpio2_map) munmap((void*)gpio2_map, GPIO_SIZE);
        close(mem_fd);
        mem_fd = -1;
        return GPIO_ERROR;
    }

    // Configure all pins as outputs
    // GPIO1: pins 44,45,46,47 = bits 12,13,14,15
    *(gpio1_map + GPIO_OE_OFFSET/4) &= ~((1<<GPIO44_BIT) | (1<<GPIO45_BIT) | 
                                          (1<<GPIO46_BIT) | (1<<GPIO47_BIT));
    
    // GPIO2: pins 65,68 = bits 1,4
    *(gpio2_map + GPIO_OE_OFFSET/4) &= ~((1<<GPIO65_BIT) | (1<<GPIO68_BIT));

    // Set all pins LOW initially
    *(gpio1_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO44_BIT) | (1<<GPIO45_BIT) | 
                                                   (1<<GPIO46_BIT) | (1<<GPIO47_BIT));
    *(gpio2_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO65_BIT) | (1<<GPIO68_BIT));

    initialized = 1;
    return GPIO_SUCCESS;
}

void gpio_cleanup(void) {
    if (!initialized) {
        return;
    }

    // Set all pins LOW before cleanup
    if (gpio1_map) {
        *(gpio1_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO44_BIT) | (1<<GPIO45_BIT) | 
                                                       (1<<GPIO46_BIT) | (1<<GPIO47_BIT));
        munmap((void*)gpio1_map, GPIO_SIZE);
        gpio1_map = NULL;
    }

    if (gpio2_map) {
        *(gpio2_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO65_BIT) | (1<<GPIO68_BIT));
        munmap((void*)gpio2_map, GPIO_SIZE);
        gpio2_map = NULL;
    }

    if (mem_fd >= 0) {
        close(mem_fd);
        mem_fd = -1;
    }

    initialized = 0;
}

// GPIO44 functions
int gpio_high_44(void) { return set_pin_high(GPIO44_BANK, GPIO44_BIT); }
int gpio_low_44(void)  { return set_pin_low(GPIO44_BANK, GPIO44_BIT); }

// GPIO45 functions
int gpio_high_45(void) { return set_pin_high(GPIO45_BANK, GPIO45_BIT); }
int gpio_low_45(void)  { return set_pin_low(GPIO45_BANK, GPIO45_BIT); }

// GPIO46 functions
int gpio_high_46(void) { return set_pin_high(GPIO46_BANK, GPIO46_BIT); }
int gpio_low_46(void)  { return set_pin_low(GPIO46_BANK, GPIO46_BIT); }

// GPIO47 functions
int gpio_high_47(void) { return set_pin_high(GPIO47_BANK, GPIO47_BIT); }
int gpio_low_47(void)  { return set_pin_low(GPIO47_BANK, GPIO47_BIT); }

// GPIO65 functions
int gpio_high_65(void) { return set_pin_high(GPIO65_BANK, GPIO65_BIT); }
int gpio_low_65(void)  { return set_pin_low(GPIO65_BANK, GPIO65_BIT); }

// GPIO68 functions
int gpio_high_68(void) { return set_pin_high(GPIO68_BANK, GPIO68_BIT); }
int gpio_low_68(void)  { return set_pin_low(GPIO68_BANK, GPIO68_BIT); }

// Set all pins HIGH at once
int gpio_all_high(void) {
    if (!initialized) {
        fprintf(stderr, "GPIO not initialized. Call gpio_init() first.\n");
        return GPIO_ERROR;
    }
    
    *(gpio1_map + GPIO_SETDATAOUT_OFFSET/4) = ((1<<GPIO44_BIT) | (1<<GPIO45_BIT) | 
                                                 (1<<GPIO46_BIT) | (1<<GPIO47_BIT));
    *(gpio2_map + GPIO_SETDATAOUT_OFFSET/4) = ((1<<GPIO65_BIT) | (1<<GPIO68_BIT));
    return GPIO_SUCCESS;
}

// Set all pins LOW at once
int gpio_all_low(void) {
    if (!initialized) {
        fprintf(stderr, "GPIO not initialized. Call gpio_init() first.\n");
        return GPIO_ERROR;
    }
    
    *(gpio1_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO44_BIT) | (1<<GPIO45_BIT) | 
                                                   (1<<GPIO46_BIT) | (1<<GPIO47_BIT));
    *(gpio2_map + GPIO_CLEARDATAOUT_OFFSET/4) = ((1<<GPIO65_BIT) | (1<<GPIO68_BIT));
    return GPIO_SUCCESS;
}
