#pragma once

#include <vector>
#include "config/sensors_config.h"

class SensorDataManager {
private:
    std::vector<SensorReading> readings;

public:
    void addReading(SensorType type, const char* name, float value, DateTime timestamp, bool valid = true) {
        readings.push_back({
            .type = type,
            .sensorName = name,
            .value = value,
            .value2 = 0,
            .timestamp = timestamp,
            .valid = valid
        });
    }

    void addDualReading(SensorType type, const char* name, float value1, float value2, DateTime timestamp, bool valid = true) {
        readings.push_back({
            .type = type,
            .sensorName = name,
            .value = value1,
            .value2 = value2,
            .timestamp = timestamp,
            .valid = valid
        });
    }

    const std::vector<SensorReading>& getReadings() const {
        return readings;
    }

    void clearReadings() {
        readings.clear();
    }

    size_t readingsCount() const {
        return readings.size();
    }
}; 