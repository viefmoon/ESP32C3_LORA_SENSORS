#pragma once

#include "../config/libraries_config.h"

class SPIManager {
public:
    static bool begin() {
        SPI.begin(PinConfig::LORA_SCK, PinConfig::LORA_MISO, PinConfig::LORA_MOSI);
        SPI.beginTransaction(SPISettings(SystemConfig::SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
        return true;
    }

protected:
    static uint8_t transfer(uint8_t data) {
        return SPI.transfer(data);
    }

    static void transfer(uint8_t* buffer, size_t size) {
        SPI.transfer(buffer, size);
    }

    static void endTransaction() {
        SPI.endTransaction();
    }
}; 