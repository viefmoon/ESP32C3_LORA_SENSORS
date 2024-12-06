#pragma once

#include <vector>
#include "ISensor.h"

class SensorManager {
public:
    static SensorManager& getInstance() {
        static SensorManager instance;
        return instance;
    }

    void registerSensor(ISensor* sensor) {
        if (sensor) {
            m_sensors.push_back(sensor);
        }
    }

    bool begin() {
        bool success = true;
        for (auto sensor : m_sensors) {
            if (!sensor->begin()) {
                Serial.printf("Error iniciando sensor: %s\n", sensor->getName());
                success = false;
            }
        }
        return success;
    }

    void readAll() {
        for (auto sensor : m_sensors) {
            if (!sensor->read()) {
                Serial.printf("Error leyendo sensor: %s\n", sensor->getName());
            }
        }
    }

    String getAllDataJson() {
        String json = "[";
        for (size_t i = 0; i < m_sensors.size(); ++i) {
            json += m_sensors[i]->toJson();
            if (i < m_sensors.size() - 1) {
                json += ",";
            }
        }
        json += "]";
        return json;
    }

private:
    std::vector<ISensor*> m_sensors;
}; 