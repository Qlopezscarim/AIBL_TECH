#include "PCA9685.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>
#include <stdexcept>
#include <thread>
#include <chrono>

PCA9685::PCA9685(const std::string& i2c_bus, uint8_t address)
    : i2c_fd_(-1), address_(address), i2c_bus_(i2c_bus) {
}

PCA9685::~PCA9685() {
    if (i2c_fd_ >= 0) {
        allOff();
        close(i2c_fd_);
    }
}

bool PCA9685::init() {
    // Open I2C bus
    i2c_fd_ = open(i2c_bus_.c_str(), O_RDWR);
    if (i2c_fd_ < 0) {
        throw std::runtime_error("Failed to open I2C bus: " + i2c_bus_);
    }
    
    // Set I2C slave address
    if (ioctl(i2c_fd_, I2C_SLAVE, address_) < 0) {
        close(i2c_fd_);
        i2c_fd_ = -1;
        throw std::runtime_error("Failed to set I2C slave address");
    }
    
    // Reset device
    writeRegister(PCA9685_MODE1, MODE1_RESTART);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Configure MODE1: Auto-increment enabled
    writeRegister(PCA9685_MODE1, MODE1_AI | MODE1_ALLCALL);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    // Configure MODE2: totem pole output (not open drain)
    writeRegister(PCA9685_MODE2, MODE2_OUTDRV);
    
    // Set default frequency to 50Hz (good for servos)
    setFrequency(50.0f);
    
    return true;
}

void PCA9685::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    if (write(i2c_fd_, buffer, 2) != 2) {
        throw std::runtime_error("Failed to write to I2C device");
    }
}

uint8_t PCA9685::readRegister(uint8_t reg) {
    uint8_t value = 0;
    if (write(i2c_fd_, &reg, 1) != 1) {
        throw std::runtime_error("Failed to write register address");
    }
    if (read(i2c_fd_, &value, 1) != 1) {
        throw std::runtime_error("Failed to read from I2C device");
    }
    return value;
}

uint8_t PCA9685::calculatePrescale(float freq_hz) {
    // PCA9685 uses 25MHz internal oscillator
    // prescale = round(25MHz / (4096 * freq)) - 1
    float prescale_val = 25000000.0f / (4096.0f * freq_hz) - 1.0f;
    return static_cast<uint8_t>(std::round(prescale_val));
}

void PCA9685::setFrequency(float freq_hz) {
    if (freq_hz < 24.0f || freq_hz > 1526.0f) {
        throw std::runtime_error("Frequency out of range (24-1526 Hz)");
    }
    
    uint8_t prescale = calculatePrescale(freq_hz);
    
    // Need to sleep device to change prescale
    uint8_t oldmode = readRegister(PCA9685_MODE1);
    uint8_t newmode = (oldmode & 0x7F) | MODE1_SLEEP;  // sleep
    writeRegister(PCA9685_MODE1, newmode);
    writeRegister(PCA9685_PRESCALE, prescale);
    writeRegister(PCA9685_MODE1, oldmode);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    // Restart and enable auto-increment
    writeRegister(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
}

void PCA9685::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    if (channel > 15) {
        throw std::runtime_error("Channel out of range (0-15)");
    }
    
    // Each channel has 4 registers: ON_L, ON_H, OFF_L, OFF_H
    uint8_t base_reg = PCA9685_LED0_ON_L + (4 * channel);
    
    writeRegister(base_reg, on & 0xFF);
    writeRegister(base_reg + 1, on >> 8);
    writeRegister(base_reg + 2, off & 0xFF);
    writeRegister(base_reg + 3, off >> 8);
}

void PCA9685::setChannelPWM(uint8_t channel, uint16_t value) {
    if (value > 4095) {
        value = 4095;
    }
    setPWM(channel, 0, value);
}

void PCA9685::setServoAngle(uint8_t channel, float angle) {
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    
    // Standard servo: 1ms = 0°, 1.5ms = 90°, 2ms = 180°
    // At 50Hz: period = 20ms = 4096 counts
    // 1ms = 205 counts, 2ms = 410 counts
    // So: pulse_width = 205 + (angle/180) * 205
    uint16_t pulse = static_cast<uint16_t>(205 + (angle / 180.0f) * 205);
    
    setPWM(channel, 0, pulse);
}

void PCA9685::setMotorSpeed(uint8_t channel, float speed) {
    if (speed < -100.0f) speed = -100.0f;
    if (speed > 100.0f) speed = 100.0f;
    
    // Convert speed percentage to PWM value (0-4095)
    uint16_t pwm_value = static_cast<uint16_t>((std::abs(speed) / 100.0f) * 4095);
    
    setChannelPWM(channel, pwm_value);
}

void PCA9685::allOff() {
    writeRegister(PCA9685_ALL_LED_OFF_H, 0x10);  // Full OFF
}

void PCA9685::sleep() {
    uint8_t mode = readRegister(PCA9685_MODE1);
    writeRegister(PCA9685_MODE1, mode | MODE1_SLEEP);
}

void PCA9685::wakeup() {
    uint8_t mode = readRegister(PCA9685_MODE1);
    writeRegister(PCA9685_MODE1, mode & ~MODE1_SLEEP);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}
