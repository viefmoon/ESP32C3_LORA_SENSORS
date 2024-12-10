#pragma once

// Pin definitions
constexpr uint8_t LED_PIN = LED_BUILTIN;

// I2C Configuration
constexpr uint8_t I2C_SDA = 1;     // GPIO1 for SDA
constexpr uint8_t I2C_SCL = 0;     // GPIO0 for SCL

// OneWire Configuration
constexpr uint8_t ONE_WIRE_PIN = 2; // GPIO2 for DS18B20 

// LoRa Configuration
constexpr uint8_t LORA_MISO = 6;    // GPIO6
constexpr uint8_t LORA_MOSI = 7;    // GPIO7
constexpr uint8_t LORA_SCK = 8;     // GPIO8 (agregamos SCK que faltaba)
constexpr uint8_t LORA_CS = 10;     // GPIO10 (agregamos CS que faltaba)
constexpr uint8_t LORA_DIO1 = 3;    // GPIO3
constexpr uint8_t LORA_BUSY = 4;    // GPIO4
constexpr uint8_t LORA_RST = 5;     // GPIO5