#pragma once

#include "../communication/I2CManager.h"

class DS3231 : public I2CManager {
public:
    struct DateTime {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    };

    bool begin() {
        if (!devicePresent(I2CAddress::DS3231)) {
            Serial.println("Error: DS3231 no encontrado");
            return false;
        }
        return true;
    }

    DateTime getDateTime() {
        Wire.beginTransmission(I2CAddress::DS3231);
        Wire.write(0x00); // Registro inicial
        Wire.endTransmission(false);
        
        Wire.requestFrom(I2CAddress::DS3231, (uint8_t)7);
        
        DateTime dt;
        dt.second = bcdToDec(Wire.read() & 0x7F);
        dt.minute = bcdToDec(Wire.read());
        dt.hour = bcdToDec(Wire.read() & 0x3F);
        Wire.read(); // Día de la semana
        dt.day = bcdToDec(Wire.read());
        dt.month = bcdToDec(Wire.read() & 0x1F);
        dt.year = bcdToDec(Wire.read()) + 2000;
        
        return dt;
    }

private:
    uint8_t bcdToDec(uint8_t val) {
        return (val / 16 * 10) + (val % 16);
    }

    uint8_t decToBcd(uint8_t val) {
        return (val / 10 * 16) + (val % 10);
    }
}; 