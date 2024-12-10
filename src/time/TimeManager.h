#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "../devices/DS3231.h"

class TimeManager {
private:
    DS3231Manager& rtc;
    const char* ntpServer = "pool.ntp.org";
    long gmtOffset_sec = -21600;     // GMT-6 for Mexico (Central Time)
    const int daylightOffset_sec = 0;
    bool timeSync = false;

public:
    TimeManager(DS3231Manager& rtc_instance) : rtc(rtc_instance) {}

    bool syncWithNTP(const char* ssid, const char* password) {
        Serial.println("Connecting to WiFi");
        WiFi.begin(ssid, password);
        
        // Wait for connection with timeout
        uint8_t attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("\nWiFi connection failed");
            return false;
        }

        Serial.println("\nWiFi connected");

        // Init and get time from NTP
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time from NTP");
            WiFi.disconnect();
            return false;
        }

        // Update RTC with NTP time
        rtc.setDateTime(DateTime(
            timeinfo.tm_year + 1900,
            timeinfo.tm_mon + 1,
            timeinfo.tm_mday,
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        ));

        WiFi.disconnect();
        timeSync = true;
        return true;
    }

    bool isTimeSync() const {
        return timeSync;
    }

    void setGMTOffset(long offset_sec) {
        gmtOffset_sec = offset_sec;
    }
}; 