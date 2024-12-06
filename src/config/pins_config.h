#pragma once

namespace PinConfig {
    // Configuración I2C
    constexpr uint8_t I2C_SCL = 0;
    constexpr uint8_t I2C_SDA = 1;

    // Configuración SPI para LoRa
    constexpr uint8_t LORA_MISO = 6;
    constexpr uint8_t LORA_MOSI = 7;
    constexpr uint8_t LORA_SCK = 10;
    constexpr uint8_t LORA_DIO1 = 3;
    constexpr uint8_t LORA_BUSY = 4;
    constexpr uint8_t LORA_RST = 5;

    // Configuración RS485
    constexpr uint8_t RS485_RX = 20;
    constexpr uint8_t RS485_TX = 21;

    // Otros pines
    constexpr uint8_t ONE_WIRE = 2;
    constexpr uint8_t FLOW_SENSOR = 18;
    constexpr uint8_t PCA9555_INT = 19;
}

// Direcciones I2C
namespace I2CAddress {
    constexpr uint8_t PCA9555 = 0x20;
    constexpr uint8_t DS3231 = 0x68;
} 