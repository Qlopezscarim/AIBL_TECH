#ifndef PCA9685_H
#define PCA9685_H

#include <cstdint>
#include <string>

// PCA9685 Register Definitions
#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01
#define PCA9685_SUBADR1 0x02
#define PCA9685_SUBADR2 0x03
#define PCA9685_SUBADR3 0x04
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x06
#define PCA9685_LED0_ON_H 0x07
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09
#define PCA9685_ALL_LED_ON_L 0xFA
#define PCA9685_ALL_LED_ON_H 0xFB
#define PCA9685_ALL_LED_OFF_L 0xFC
#define PCA9685_ALL_LED_OFF_H 0xFD

// MODE1 bits
#define MODE1_RESTART 0x80
#define MODE1_SLEEP 0x10
#define MODE1_ALLCALL 0x01
#define MODE1_AI 0x20  // Auto-Increment

// MODE2 bits
#define MODE2_OUTDRV 0x04

class PCA9685 {
public:
    /**
     * Constructor
     * @param i2c_bus I2C bus device (e.g., "/dev/i2c-2")
     * @param address I2C address of PCA9685 (default 0x40)
     */
    PCA9685(const std::string& i2c_bus = "/dev/i2c-2", uint8_t address = 0x40);
    
    /**
     * Destructor - closes I2C connection
     */
    ~PCA9685();
    
    /**
     * Initialize the PCA9685 chip
     * @return true if successful
     */
    bool init();
    
    /**
     * Set PWM frequency (typically 50Hz for servos, 1000Hz for motors)
     * @param freq_hz Frequency in Hz (24-1526 Hz range)
     */
    void setFrequency(float freq_hz);
    
    /**
     * Set PWM for a specific channel
     * @param channel Channel number (0-15)
     * @param on When to turn on (0-4095)
     * @param off When to turn off (0-4095)
     */
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);
    
    /**
     * Set PWM duty cycle for a channel (0-4095)
     * @param channel Channel number (0-15)
     * @param value PWM value (0=off, 4095=full on)
     */
    void setChannelPWM(uint8_t channel, uint16_t value);
    
    /**
     * Set servo angle (assumes standard servo with 1-2ms pulse)
     * @param channel Channel number (0-15)
     * @param angle Angle in degrees (0-180)
     */
    void setServoAngle(uint8_t channel, float angle);
    
    /**
     * Set motor speed (-100 to +100, for bi-directional motor control)
     * @param channel Channel number (0-15)
     * @param speed Speed percentage (-100 to +100)
     */
    void setMotorSpeed(uint8_t channel, float speed);
    
    /**
     * Turn off all PWM outputs
     */
    void allOff();
    
    /**
     * Put device to sleep (low power mode)
     */
    void sleep();
    
    /**
     * Wake device from sleep
     */
    void wakeup();

private:
    int i2c_fd_;
    uint8_t address_;
    std::string i2c_bus_;
    
    /**
     * Write a byte to a register
     */
    void writeRegister(uint8_t reg, uint8_t value);
    
    /**
     * Read a byte from a register
     */
    uint8_t readRegister(uint8_t reg);
    
    /**
     * Calculate prescale value for desired frequency
     */
    uint8_t calculatePrescale(float freq_hz);
};

#endif // PCA9685_H
