#pragma once

#include <Wire.h>
#include "../config/system_config.h"
#include "../config/pins_config.h"

class I2CManager {
public:
    static bool begin() {
        Wire.begin(PinConfig::I2C_SDA, PinConfig::I2C_SCL);
        Wire.setClock(SystemConfig::I2C_FREQUENCY);
        return true;
    }

    static bool devicePresent(uint8_t address) {
        Wire.beginTransmission(address);
        return (Wire.endTransmission() == 0);
    }

protected:
    static bool writeRegister(uint8_t deviceAddress, uint8_t reg, uint8_t data) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(reg);
        Wire.write(data);
        return (Wire.endTransmission() == 0);
    }

    static uint8_t readRegister(uint8_t deviceAddress, uint8_t reg) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(reg);
        Wire.endTransmission(false);
        
        Wire.requestFrom(deviceAddress, (uint8_t)1);
        return Wire.read();
    }
}; 