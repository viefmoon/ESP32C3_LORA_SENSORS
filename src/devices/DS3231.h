#pragma once

#include <RTClib.h>

class DS3231Manager {
private:
    RTC_DS3231 rtc;
    bool initialized = false;

public:
    bool begin() {
        if (!rtc.begin()) {
            return false;
        }
        initialized = true;
        return true;
    }

    bool isInitialized() const {
        return initialized;
    }

    void setDateTime(const DateTime& dt) {
        if (initialized) {
            rtc.adjust(dt);
        }
    }

    DateTime getDateTime() {
        if (initialized) {
            return rtc.now();
        }
        return DateTime(1970, 1, 1, 0, 0, 0); // Return Unix epoch as invalid date
    }

    float getTemperature() {
        if (initialized) {
            return rtc.getTemperature();
        }
        return -273.15; // Invalid value
    }
}; 