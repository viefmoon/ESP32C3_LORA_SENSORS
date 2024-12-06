#pragma once

#include "ISensor.h"
#include "../config/libraries_config.h"

class FlowSensor : public ISensor {
public:
    FlowSensor(const char* name) 
        : m_name(name)
        , m_pulseCount(0)
        , m_flowRate(0.0f)
        , m_totalLiters(0.0f) {
    }

    bool begin() override {
        pinMode(PinConfig::FLOW_SENSOR, INPUT_PULLUP);
        attachInterruptArg(
            digitalPinToInterrupt(PinConfig::FLOW_SENSOR),
            FlowSensor::pulseCounter,
            this,
            FALLING
        );
        return true;
    }

    bool read() override {
        // Deshabilitar interrupciones para leer el contador
        portENTER_CRITICAL(&m_mux);
        uint32_t pulses = m_pulseCount;
        m_pulseCount = 0;  // Resetear contador
        portEXIT_CRITICAL(&m_mux);

        // Calcular caudal (pulsos por segundo)
        float flowRate = (pulses * (1000.0f / 5000.0f));  // pulsos en 5 segundos a pulsos por segundo
        
        // Convertir a litros por minuto
        m_flowRate = (flowRate / SensorConfig::FlowSensor::PULSES_PER_LITER) * 60.0f;
        
        // Actualizar total de litros
        m_totalLiters += (pulses / SensorConfig::FlowSensor::PULSES_PER_LITER);

        return true;
    }

    const char* getName() const override { return m_name; }
    const char* getType() const override { return "FlowSensor"; }
    
    float getFlowRate() const { return m_flowRate; }
    float getTotalLiters() const { return m_totalLiters; }

    String toJson() const override {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), 
                R"({"sensor":"%s","type":"%s","flow_rate":%.2f,"total_liters":%.2f})",
                getName(), getType(), m_flowRate, m_totalLiters);
        return String(buffer);
    }

    // Función para resetear el total de litros
    void resetTotalLiters() {
        m_totalLiters = 0.0f;
    }

private:
    static void IRAM_ATTR pulseCounter(void* arg) {
        FlowSensor* sensor = static_cast<FlowSensor*>(arg);
        portENTER_CRITICAL(&sensor->m_mux);
        sensor->m_pulseCount++;
        portEXIT_CRITICAL(&sensor->m_mux);
    }

    const char* m_name;
    volatile uint32_t m_pulseCount;
    float m_flowRate;      // Litros por minuto
    float m_totalLiters;   // Total de litros acumulados
    portMUX_TYPE m_mux = portMUX_INITIALIZER_UNLOCKED;
}; 