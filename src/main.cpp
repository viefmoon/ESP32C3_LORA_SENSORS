#include "config/libraries_config.h"
#include "devices/PCA9555.h"
#include "devices/DS3231.h"
#include "devices/SX1262.h"
#include "sensors/SensorManager.h"
#include "sensors/DS18B20Sensor.h"
#include "sensors/SHT40Sensor.h"
#include "sensors/FlowSensor.h"

PCA9555 portExpander;
DS3231 rtc;
SX1262 lora;

// Declaración de sensores
std::vector<DS18B20Sensor*> tempSensors;
std::unique_ptr<SHT40Sensor> sht40;
std::unique_ptr<FlowSensor> flowSensor;

void setup_sensors() {
    if (SensorConfig::ENABLE_DS18B20) {
        for (uint8_t i = 0; i < SensorConfig::DS18B20::MAX_SENSORS; i++) {
            auto sensor = new DS18B20Sensor(SensorConfig::DS18B20::NAMES[i], i);
            tempSensors.push_back(sensor);
            SensorManager::getInstance().registerSensor(sensor);
        }
    }

    if (SensorConfig::ENABLE_SHT40) {
        sht40 = std::make_unique<SHT40Sensor>(SensorConfig::SHT40::NAME);
        SensorManager::getInstance().registerSensor(sht40.get());
    }

    if (SensorConfig::ENABLE_FLOW_SENSOR) {
        flowSensor = std::make_unique<FlowSensor>(SensorConfig::FlowSensor::NAME);
        SensorManager::getInstance().registerSensor(flowSensor.get());
    }
    
    if (!SensorManager::getInstance().begin()) {
        Serial.println("Error iniciando algunos sensores");
    }
}

void setup_serial() {
    Serial.begin(SystemConfig::SERIAL_BAUD_RATE);
    while (!Serial) {
        ; // Esperar a que el puerto serial esté listo
    }
    Serial.println("Sistema iniciando...");
}

void setup_gpio() {
    pinMode(PinConfig::FLOW_SENSOR, INPUT_PULLUP);
    pinMode(PinConfig::PCA9555_INT, INPUT_PULLUP);
}

void setup_i2c_devices() {
    if (!portExpander.begin()) {
        Serial.println("Error inicializando PCA9555");
    }
    
    if (!rtc.begin()) {
        Serial.println("Error inicializando DS3231");
    }
}

void setup_lora() {
    if (!lora.begin()) {
        Serial.println("Error inicializando módulo LoRa");
    } else {
        Serial.println("Módulo LoRa inicializado correctamente");
    }
}

void setup() {
    setup_serial();
    setup_gpio();
    setup_i2c_devices();
    setup_lora();
    setup_sensors();
}

void loop() {
    // Leer todos los sensores
    SensorManager::getInstance().readAll();
    
    // Obtener timestamp
    DS3231::DateTime now = rtc.getDateTime();
    
    // Crear mensaje JSON con timestamp y datos de sensores
    String jsonMessage = "{\"timestamp\":\"";
    char timeStr[20];
    snprintf(timeStr, sizeof(timeStr), "%02d/%02d/%04d %02d:%02d:%02d",
             now.day, now.month, now.year,
             now.hour, now.minute, now.second);
    jsonMessage += timeStr;
    jsonMessage += "\",\"sensors\":";
    jsonMessage += SensorManager::getInstance().getAllDataJson();
    jsonMessage += "}";
    
    // Enviar por LoRa
    lora.transmit((uint8_t*)jsonMessage.c_str(), jsonMessage.length());
    
    Serial.println(jsonMessage);
    delay(5000);
}