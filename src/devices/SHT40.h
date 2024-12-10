#pragma once

#include <Adafruit_SHT4x.h>

class SHT40Manager {
private:
    Adafruit_SHT4x sht4;
    bool initialized = false;

public:
    struct Reading {
        float temperature;
        float humidity;
        bool valid;
    };

    bool begin() {
        if (!sht4.begin()) {
            return false;
        }
        
        // Configurar precisión y heater
        sht4.setPrecision(SHT4X_HIGH_PRECISION); // Alta precisión
        sht4.setHeater(SHT4X_NO_HEATER);        // Sin calentador
        initialized = true;
        return true;
    }

    Reading read() {
        if (!initialized) {
            return {-273.15f, -1.0f, false};
        }

        sensors_event_t humidity, temp;
        
        if (!sht4.getEvent(&humidity, &temp)) {
            return {-273.15f, -1.0f, false};
        }

        return {
            .temperature = temp.temperature,
            .humidity = humidity.relative_humidity,
            .valid = true
        };
    }

    bool isInitialized() const {
        return initialized;
    }
}; 