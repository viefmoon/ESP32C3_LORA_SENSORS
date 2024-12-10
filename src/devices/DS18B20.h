#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>
#include <vector>

class DS18B20Manager {
private:
    OneWire oneWire;
    DallasTemperature sensors;
    std::vector<uint8_t*> allowedAddresses;
    bool initialized = false;

public:
    DS18B20Manager(uint8_t pin) : oneWire(pin), sensors(&oneWire) {}

    ~DS18B20Manager() {
        for(auto addr : allowedAddresses) {
            delete[] addr;
        }
    }

    bool begin() {
        sensors.begin();
        if (sensors.getDeviceCount() == 0) {
            return false;
        }
        initialized = true;
        return true;
    }

    // Escanea y muestra todas las direcciones disponibles
    void scanAddresses() {
        uint8_t addr[8];
        Serial.println("Scanning for DS18B20 devices...");
        
        while(oneWire.search(addr)) {
            Serial.print("Device found: ");
            for(uint8_t i = 0; i < 8; i++) {
                Serial.print(addr[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
        oneWire.reset_search();
    }

    // Agrega una dirección a la lista de sensores permitidos
    void addAllowedAddress(const uint8_t* address) {
        uint8_t* newAddr = new uint8_t[8];
        memcpy(newAddr, address, 8);
        allowedAddresses.push_back(newAddr);
    }

    // Lee la temperatura de un sensor específico por su dirección
    float readTemperature(const uint8_t* address) {
        if (!initialized) return -273.15f;
        
        sensors.requestTemperaturesByAddress(address);
        return sensors.getTempC(address);
    }

    // Lee las temperaturas solo de los sensores permitidos
    std::vector<float> readAllowedTemperatures() {
        std::vector<float> temperatures;
        
        if (!initialized) return temperatures;

        sensors.requestTemperatures(); // Solicita temperaturas de todos los sensores
        
        for(const auto& addr : allowedAddresses) {
            float temp = sensors.getTempC(addr);
            temperatures.push_back(temp);
        }
        
        return temperatures;
    }

    size_t getAllowedSensorsCount() const {
        return allowedAddresses.size();
    }
}; 