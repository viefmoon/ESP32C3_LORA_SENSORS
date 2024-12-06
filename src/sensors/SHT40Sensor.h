#pragma once

#include "ISensor.h"
#include "../communication/I2CManager.h"

class SHT40Sensor : public ISensor {
public:
    struct Measurements {
        float temperature;
        float humidity;
    };

    SHT40Sensor(const char* name) 
        : m_name(name)
        , m_lastMeasurement{-999.0f, -999.0f} {
    }

    bool begin() override {
        Wire.beginTransmission(SensorConfig::SHT40::I2C_ADDRESS);
        if (Wire.endTransmission() != 0) {
            Serial.println("Error: SHT40 no encontrado");
            return false;
        }
        return true;
    }

    bool read() override {
        Wire.beginTransmission(SensorConfig::SHT40::I2C_ADDRESS);
        Wire.write(SensorConfig::SHT40::PRECISION);
        if (Wire.endTransmission() != 0) return false;

        delay(10); // Esperar a que la medición esté lista

        uint8_t data[6];
        Wire.requestFrom(SensorConfig::SHT40::I2C_ADDRESS, (uint8_t)6);
        if (Wire.available() != 6) return false;

        for (int i = 0; i < 6; i++) {
            data[i] = Wire.read();
        }

        // Convertir los datos raw a temperatura y humedad
        uint16_t rawTemp = (data[0] << 8) | data[1];
        uint16_t rawHum = (data[3] << 8) | data[4];

        // Fórmulas de conversión del datasheet
        m_lastMeasurement.temperature = -45.0f + 175.0f * rawTemp / 65535.0f;
        m_lastMeasurement.humidity = -6.0f + 125.0f * rawHum / 65535.0f;

        // Limitar la humedad a 100%
        if (m_lastMeasurement.humidity > 100.0f) {
            m_lastMeasurement.humidity = 100.0f;
        }

        return true;
    }

    const char* getName() const override { return m_name; }
    const char* getType() const override { return "SHT40"; }
    
    float getTemperature() const { return m_lastMeasurement.temperature; }
    float getHumidity() const { return m_lastMeasurement.humidity; }

    String toJson() const override {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), 
                R"({"sensor":"%s","type":"%s","temperature":%.2f,"humidity":%.2f})",
                getName(), getType(), 
                m_lastMeasurement.temperature, 
                m_lastMeasurement.humidity);
        return String(buffer);
    }

private:
    const char* m_name;
    Measurements m_lastMeasurement;
}; 