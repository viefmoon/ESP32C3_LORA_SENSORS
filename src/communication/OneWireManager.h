#pragma once

#include "../config/libraries_config.h"

class OneWireManager {
public:
    static OneWire& getInstance() {
        static OneWire instance(PinConfig::ONE_WIRE);
        return instance;
    }

protected:
    static void reset() {
        getInstance().reset();
    }

    static uint8_t search(uint8_t *addr) {
        return getInstance().search(addr);
    }

    static void select(const uint8_t *addr) {
        getInstance().select(addr);
    }

    static void write(uint8_t data, uint8_t power = 0) {
        getInstance().write(data, power);
    }

    static uint8_t read() {
        return getInstance().read();
    }
}; 