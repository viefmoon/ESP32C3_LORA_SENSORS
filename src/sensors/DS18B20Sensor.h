#pragma once

#include "ISensor.h"
#include "../communication/OneWireManager.h"
#include <DallasTemperature.h>

class DS18B20Sensor : public ISensor {
public:
    DS18B20Sensor(const char* name, uint8_t index = 0) 
        : m_name(name)
        , m_index(index)
        , m_sensors(&OneWireManager::getInstance())
        , m_lastTemp(DEVICE_DISCONNECTED_C) {
    }

    bool begin() override {
        m_sensors.begin();
        return m_sensors.getDeviceCount() > m_index;
    }

    bool read() override {
        m_sensors.requestTemperatures();
        float temp = m_sensors.getTempCByIndex(m_index);
        if (temp != DEVICE_DISCONNECTED_C) {
            m_lastTemp = temp;
            return true;
        }
        return false;
    }

    const char* getName() const override { return m_name; }
    const char* getType() const override { return "DS18B20"; }
    
    float getTemperature() const { return m_lastTemp; }

    String toJson() const override {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), 
                R"({"sensor":"%s","type":"%s","temperature":%.2f})",
                getName(), getType(), m_lastTemp);
        return String(buffer);
    }

private:
    const char* m_name;
    uint8_t m_index;
    DallasTemperature m_sensors;
    float m_lastTemp;
}; 