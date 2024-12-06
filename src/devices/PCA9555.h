#pragma once

#include "../communication/I2CManager.h"

class PCA9555 : public I2CManager {
public:
    enum Register {
        INPUT_PORT0 = 0x00,
        INPUT_PORT1 = 0x01,
        OUTPUT_PORT0 = 0x02,
        OUTPUT_PORT1 = 0x03,
        POLARITY_PORT0 = 0x04,
        POLARITY_PORT1 = 0x05,
        CONFIG_PORT0 = 0x06,
        CONFIG_PORT1 = 0x07
    };

    bool begin() {
        if (!devicePresent(I2CAddress::PCA9555)) {
            Serial.println("Error: PCA9555 no encontrado");
            return false;
        }
        
        // Configurar puertos según necesidad
        // Por ejemplo, configurar pines para ADCs como salidas
        writeRegister(I2CAddress::PCA9555, Register::CONFIG_PORT0, 0x00); // Puerto 0 como salidas
        writeRegister(I2CAddress::PCA9555, Register::CONFIG_PORT1, 0x00); // Puerto 1 como salidas
        
        return true;
    }

    bool digitalWrite(uint8_t pin, bool value) {
        uint8_t port = pin / 8;
        uint8_t bit = pin % 8;
        uint8_t currentState = readRegister(I2CAddress::PCA9555, Register::OUTPUT_PORT0 + port);
        
        if (value) {
            currentState |= (1 << bit);
        } else {
            currentState &= ~(1 << bit);
        }
        
        return writeRegister(I2CAddress::PCA9555, Register::OUTPUT_PORT0 + port, currentState);
    }

    bool digitalRead(uint8_t pin) {
        uint8_t port = pin / 8;
        uint8_t bit = pin % 8;
        uint8_t currentState = readRegister(I2CAddress::PCA9555, Register::INPUT_PORT0 + port);
        return (currentState & (1 << bit)) != 0;
    }
}; 