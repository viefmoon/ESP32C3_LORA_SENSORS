#pragma once

#include "../communication/SPIManager.h"
#include <Arduino.h>

class SX1262 : public SPIManager {
public:
    enum class Command : uint8_t {
        SET_STANDBY = 0x80,
        SET_TX = 0x83,
        SET_RX = 0x82,
        SET_SLEEP = 0x84,
        SET_RF_FREQUENCY = 0x86,
        SET_PACKET_TYPE = 0x8A,
        SET_TX_PARAMS = 0x8E,
        SET_MODULATION_PARAMS = 0x8B,
        SET_PACKET_PARAMS = 0x8C,
        GET_STATUS = 0xC0,
        READ_BUFFER = 0x1E,
        WRITE_BUFFER = 0x0E
    };

    bool begin() {
        // Inicializar pines
        pinMode(PinConfig::LORA_RST, OUTPUT);
        pinMode(PinConfig::LORA_BUSY, INPUT);
        pinMode(PinConfig::LORA_DIO1, INPUT);

        // Reset del módulo
        reset();

        // Inicializar SPI
        SPIManager::begin();

        // Configuración básica
        setStandby();
        setPacketType();
        setRfFrequency(SystemConfig::LoRa::FREQUENCY);
        setModulationParams();
        setTxParams();

        return true;
    }

    bool transmit(const uint8_t* data, size_t size) {
        if (size > 255) return false;

        writeBuffer(0x00, data, size);
        setTx();
        
        // Esperar a que termine la transmisión
        while(digitalRead(PinConfig::LORA_BUSY));
        return true;
    }

private:
    void reset() {
        digitalWrite(PinConfig::LORA_RST, LOW);
        delay(10);
        digitalWrite(PinConfig::LORA_RST, HIGH);
        delay(10);
    }

    void setStandby() {
        uint8_t cmd[] = {static_cast<uint8_t>(Command::SET_STANDBY), 0x00};
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void setPacketType() {
        uint8_t cmd[] = {static_cast<uint8_t>(Command::SET_PACKET_TYPE), 0x01}; // 0x01 = LoRa
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void setRfFrequency(uint32_t frequency) {
        uint32_t freq = (frequency / 32768) * 32768;
        uint8_t cmd[] = {
            static_cast<uint8_t>(Command::SET_RF_FREQUENCY),
            static_cast<uint8_t>((freq >> 24) & 0xFF),
            static_cast<uint8_t>((freq >> 16) & 0xFF),
            static_cast<uint8_t>((freq >> 8) & 0xFF),
            static_cast<uint8_t>(freq & 0xFF)
        };
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void setModulationParams() {
        uint8_t cmd[] = {
            static_cast<uint8_t>(Command::SET_MODULATION_PARAMS),
            SystemConfig::LoRa::SPREADING_FACTOR,
            static_cast<uint8_t>(SystemConfig::LoRa::BANDWIDTH),
            SystemConfig::LoRa::CODING_RATE,
            0x00 // Low Data Rate Optimization (auto)
        };
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void setTxParams() {
        uint8_t cmd[] = {
            static_cast<uint8_t>(Command::SET_TX_PARAMS),
            static_cast<uint8_t>(SystemConfig::LoRa::TX_POWER),
            0x04 // Ramping time
        };
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void setTx() {
        uint8_t cmd[] = {static_cast<uint8_t>(Command::SET_TX), 0x00, 0x00, 0x00};
        transfer(cmd, sizeof(cmd));
        while(digitalRead(PinConfig::LORA_BUSY));
    }

    void writeBuffer(uint8_t offset, const uint8_t* data, size_t size) {
        uint8_t cmd[] = {
            static_cast<uint8_t>(Command::WRITE_BUFFER),
            offset
        };
        transfer(cmd, sizeof(cmd));
        transfer(const_cast<uint8_t*>(data), size);
        while(digitalRead(PinConfig::LORA_BUSY));
    }
}; 