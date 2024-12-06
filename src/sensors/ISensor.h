#pragma once

#include <Arduino.h>

class ISensor {
public:
    virtual ~ISensor() = default;
    
    virtual bool begin() = 0;
    virtual bool read() = 0;
    virtual const char* getName() const = 0;
    virtual const char* getType() const = 0;
    
    // Método para obtener los datos en formato JSON
    virtual String toJson() const = 0;
}; 