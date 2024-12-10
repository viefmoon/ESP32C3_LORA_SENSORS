#include <Arduino.h>
#include "config/pins_config.h"
#include "config/wifi_config.h"
#include "config/sensors_config.h"
#include "devices/DS3231.h"
#include "devices/DS18B20.h"
#include "devices/SHT40.h"
#include "devices/LoRaWAN.h"
#include "time/TimeManager.h"
#include "data/SensorData.h"

// Definición del tiempo de sueño (5 minutos en microsegundos)
constexpr uint64_t SLEEP_TIME_US = 5 * 60 * 1000000ULL; // 5 minutos

DS3231Manager rtc;
TimeManager timeManager(rtc);
DS18B20Manager tempSensor(ONE_WIRE_PIN);
SHT40Manager sht40;
LoRaWANManager lorawan;
SensorDataManager sensorData;

void goToSleep() {
    Serial.println("Entrando en modo deep sleep por 5 minutos...");
    Serial.flush();
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_US);
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(LED_PIN, OUTPUT);

    // Inicializar LoRaWAN
    if (!lorawan.begin()) {
        Serial.println("Error iniciando LoRaWAN!");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(50);
            digitalWrite(LED_PIN, LOW);
            delay(50);
        }
    }
    Serial.println("LoRaWAN inicializado correctamente!");

    // Unirse a la red LoRaWAN
    Serial.println("Intentando unirse a la red LoRaWAN...");
    if (!lorawan.join()) {
        Serial.println("Error al unirse a la red LoRaWAN!");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(200);
            digitalWrite(LED_PIN, LOW);
            delay(200);
        }
    }
    Serial.println("Conectado a la red LoRaWAN!");

    if (ENABLE_RTC) {
        if (!rtc.begin()) {
            Serial.println("RTC not found!");
            while (1) {
                digitalWrite(LED_PIN, HIGH);
                delay(100);
                digitalWrite(LED_PIN, LOW);
                delay(100);
            }
        }
    }

    if (ENABLE_SHT40) {
        if (!sht40.begin()) {
            Serial.println("SHT40 not found!");
            while (1) {
                digitalWrite(LED_PIN, HIGH);
                delay(300);
                digitalWrite(LED_PIN, LOW);
                delay(300);
            }
        }
    }

    if (ENABLE_DS18B20) {
        if (!tempSensor.begin()) {
            Serial.println("Error iniciando sensores DS18B20!");
            while (1) {
                digitalWrite(LED_PIN, HIGH);
                delay(500);
                digitalWrite(LED_PIN, LOW);
                delay(500);
            }
        }

        Serial.println("Escaneando sensores disponibles:");
        tempSensor.scanAddresses();

        for(size_t i = 0; i < DS18B20Sensors::ACTIVE_SENSORS_COUNT; i++) {
            tempSensor.addAllowedAddress(DS18B20Sensors::SENSORS[i].address);
        }
    }

    // Leer y enviar datos
    DateTime now;
    if (ENABLE_RTC) {
        now = rtc.getDateTime();
    }

    if (ENABLE_SHT40) {
        auto sht40Reading = sht40.read();
        if (sht40Reading.valid) {
            sensorData.addDualReading(
                SensorType::AMBIENT_TEMP,
                "SHT40",
                sht40Reading.temperature,
                sht40Reading.humidity,
                now
            );
        }
    }

    if (ENABLE_DS18B20) {
        auto temperatures = tempSensor.readAllowedTemperatures();
        for(size_t i = 0; i < temperatures.size() && i < DS18B20Sensors::ACTIVE_SENSORS_COUNT; i++) {
            const auto& sensorConfig = DS18B20Sensors::SENSORS[i];
            sensorData.addReading(
                sensorConfig.type,
                sensorConfig.name,
                temperatures[i],
                now
            );
        }
    }

    // Enviar datos por LoRaWAN
    const auto& readings = sensorData.getReadings();
    uint8_t buffer[256];
    uint8_t index = 0;

    // Empaquetar datos
    for (const auto& reading : readings) {
        if (reading.valid && index + 8 <= 256) {
            buffer[index++] = static_cast<uint8_t>(reading.type);
            
            uint32_t value = *reinterpret_cast<const uint32_t*>(&reading.value);
            buffer[index++] = (value >> 24) & 0xFF;
            buffer[index++] = (value >> 16) & 0xFF;
            buffer[index++] = (value >> 8) & 0xFF;
            buffer[index++] = value & 0xFF;

            if (reading.type == SensorType::AMBIENT_TEMP) {
                uint32_t value2 = *reinterpret_cast<const uint32_t*>(&reading.value2);
                buffer[index++] = (value2 >> 24) & 0xFF;
                buffer[index++] = (value2 >> 16) & 0xFF;
                buffer[index++] = (value2 >> 8) & 0xFF;
                buffer[index++] = value2 & 0xFF;
            }
        }
    }

    // Enviar datos
    if (index > 0) {
        if (lorawan.sendData(buffer, index)) {
            Serial.println("Datos enviados correctamente por LoRaWAN");
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
        } else {
            Serial.println("Error al enviar datos por LoRaWAN");
            for(int i = 0; i < 3; i++) {
                digitalWrite(LED_PIN, HIGH);
                delay(100);
                digitalWrite(LED_PIN, LOW);
                delay(100);
            }
        }
    }

    // Ir a dormir
    goToSleep();
}

void loop() {
    // El loop no se ejecutará porque usamos deep sleep
}