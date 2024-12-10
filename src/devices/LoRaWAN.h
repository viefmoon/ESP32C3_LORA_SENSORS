#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <hal/hal.h>
#include <lmic.h>
#include "config/pins_config.h"

// Configuración de LoRaWAN
// Reemplazar con tus propias claves de TTN
static const u1_t PROGMEM DEVEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // LSB
static const u1_t PROGMEM APPEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // LSB
static const u1_t PROGMEM APPKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // MSB

// Callbacks para LMIC
void os_getArtEui(u1_t* buf) {
    memcpy_P(buf, APPEUI, 8);
}
void os_getDevEui(u1_t* buf) {
    memcpy_P(buf, DEVEUI, 8);
}
void os_getDevKey(u1_t* buf) {
    memcpy_P(buf, APPKEY, 16);
}

class LoRaWANManager {
private:
    bool initialized = false;
    bool joined = false;
    bool busy = false;
    static uint8_t txBuffer[256];
    static uint8_t txSize;
    static osjob_t sendjob;
    
    // Configuración de pines para LMIC
    const lmic_pinmap lmic_pins = {
        .nss = LORA_CS,
        .rxtx = LMIC_UNUSED_PIN,
        .rst = LORA_RST,
        .dio = {LORA_DIO1, LMIC_UNUSED_PIN, LMIC_UNUSED_PIN},
        .rxtx_rx_active = 0,
        .rssi_cal = 0,
        .spi_freq = 8000000
    };

    static void onEvent(ev_t ev) {
        switch (ev) {
            case EV_JOINED:
                LMIC_setLinkCheckMode(0);
                LMIC_setDrTxpow(DR_SF7, 14);
                joined = true;
                break;
            case EV_TXCOMPLETE:
                busy = false;
                break;
            default:
                break;
        }
    }

public:
    bool begin() {
        if (initialized) return true;

        // Inicializar LMIC
        os_init();
        LMIC_init();
        
        // Reiniciar MAC state
        LMIC_reset();

        // Configurar subband para US915 (canales 8-15)
        for (int i = 0; i < 72; i++) {
            if (i != 64) { // Skip channel 64 (used for join)
                LMIC_disableChannel(i);
            }
        }

        // Configurar data rate inicial
        LMIC_setDrTxpow(DR_SF7, 14);

        initialized = true;
        return true;
    }

    bool join() {
        if (!initialized || joined) return false;

        LMIC_startJoining();
        
        // Esperar hasta que se una o falle
        uint32_t startTime = millis();
        while (!joined && (millis() - startTime < 30000)) {  // Timeout 30s
            os_runloop_once();
            delay(1);
        }

        return joined;
    }

    bool sendData(uint8_t* data, size_t size, uint8_t port = 1) {
        if (!initialized || !joined || busy || size > 256) return false;

        // Copiar datos al buffer
        memcpy(txBuffer, data, size);
        txSize = size;
        busy = true;

        // Preparar uplink
        LMIC_setTxData2(port, txBuffer, txSize, 0);

        // Esperar a que se complete la transmisión
        uint32_t startTime = millis();
        while (busy && (millis() - startTime < 30000)) {  // Timeout 30s
            os_runloop_once();
            delay(1);
        }

        return !busy;  // Si busy es false, la transmisión fue exitosa
    }

    bool isInitialized() const {
        return initialized;
    }

    bool isJoined() const {
        return joined;
    }

    bool isBusy() const {
        return busy;
    }
};

// Definición de variables estáticas
uint8_t LoRaWANManager::txBuffer[256];
uint8_t LoRaWANManager::txSize = 0;
osjob_t LoRaWANManager::sendjob; 