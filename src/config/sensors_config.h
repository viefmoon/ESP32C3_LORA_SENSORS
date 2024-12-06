#pragma once

#include <Arduino.h>

namespace SensorConfig {
    // Configuración de sensores habilitados
    constexpr bool ENABLE_DS18B20 = true;
    constexpr bool ENABLE_FLOW_SENSOR = true;
    constexpr bool ENABLE_PH_SENSOR = true;
    constexpr bool ENABLE_EC_SENSOR = true;
    constexpr bool ENABLE_SHT40 = true;
    
    // Configuración específica de sensores
    namespace DS18B20 {
        constexpr uint8_t MAX_SENSORS = 2;
        constexpr char NAMES[MAX_SENSORS][16] = {
            "Temp_Agua",
            "Temp_Ambiente"
        };
    }
    
    namespace FlowSensor {
        constexpr float PULSES_PER_LITER = 450.0f;
        constexpr char NAME[] = "Flujo_Agua";
        constexpr uint32_t MEASURE_INTERVAL = 5000;
        constexpr float MIN_FLOW_RATE = 0.1f;
        constexpr float MAX_FLOW_RATE = 30.0f;
    }

    namespace SHT40 {
        constexpr uint8_t I2C_ADDRESS = 0x44; // Dirección I2C por defecto del SHT40
        constexpr char NAME[] = "SHT40_Ambiente";
        constexpr uint8_t PRECISION = 0x0E;    // Alta precisión (0x0E)
    }
    
    // Añadir más configuraciones según sea necesario
} 