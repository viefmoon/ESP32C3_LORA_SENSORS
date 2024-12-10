#pragma once
#include <RTClib.h>

// Enable/Disable sensors
constexpr bool ENABLE_RTC = true;      // DS3231 RTC
constexpr bool ENABLE_SHT40 = true;    // SHT40 Temperature/Humidity
constexpr bool ENABLE_DS18B20 = true;  // DS18B20 Temperature sensors

// Sensor IDs for identification
enum class SensorType {
    WATER_TEMP,
    SOIL_TEMP,
    AMBIENT_TEMP,
    AMBIENT_HUM
};

// DS18B20 Sensors configuration
namespace DS18B20Sensors {
    struct SensorConfig {
        const uint8_t address[8];
        const char* name;
        SensorType type;
    };

    // Define your sensors here
    constexpr SensorConfig SENSORS[] = {
        // Sensor 1 - Water temperature
        {
            {0x28, 0xFF, 0x64, 0x1E, 0x15, 0x15, 0x15, 0x15},
            "Sensor Agua",
            SensorType::WATER_TEMP
        },
        // Sensor 2 - Soil temperature 1
        {
            {0x28, 0xFF, 0x64, 0x1E, 0x15, 0x15, 0x15, 0x16},
            "Sensor Tierra",
            SensorType::SOIL_TEMP
        },
        // Add more sensors as needed:
        /*
        {
            {0x28, 0xFF, 0x64, 0x1E, 0x15, 0x15, 0x15, 0x17},
            "Sensor Tierra 2",
            SensorType::SOIL_TEMP
        },
        */
    };

    constexpr size_t ACTIVE_SENSORS_COUNT = sizeof(SENSORS) / sizeof(SENSORS[0]);
}

// Structure for sensor readings with timestamp
struct SensorReading {
    SensorType type;
    const char* sensorName;
    float value;
    float value2;         // For sensors with multiple values (like SHT40)
    DateTime timestamp;
    bool valid;
}; 