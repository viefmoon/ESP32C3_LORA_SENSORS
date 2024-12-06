#pragma once

namespace SystemConfig {
    // Configuración del sistema
    constexpr uint32_t SERIAL_BAUD_RATE = 115200;
    constexpr uint32_t I2C_FREQUENCY = 400000; // 400kHz
    constexpr uint32_t SPI_FREQUENCY = 8000000; // 8MHz
    
    // Voltajes de referencia y constantes
    constexpr float SYSTEM_VOLTAGE = 3.3f; // Voltaje del sistema
    constexpr float ADC_VREF = 1.2f;      // Voltaje de referencia del ADC

    // Configuración LoRa
    namespace LoRa {
        constexpr uint32_t FREQUENCY = 915E6;    // Frecuencia en Hz (915MHz)
        constexpr int8_t TX_POWER = 22;          // Potencia de transmisión en dBm
        constexpr uint8_t SPREADING_FACTOR = 7;   // Factor de dispersión
        constexpr uint32_t BANDWIDTH = 125E3;     // Ancho de banda en Hz
        constexpr uint8_t CODING_RATE = 5;        // Tasa de codificación (4/5)
    }
} 