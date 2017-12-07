#include "dust_sensor.hpp"

#include <Arduino.h>
#include "config.h"

DustSensor DustSensor::instance = DustSensor();

void DustSensor::on_falling_edge()
{
    attachInterrupt(instance.m_pin, DustSensor::on_rising_edge, RISING);
    instance.mv_last_low = micros();
}

void DustSensor::on_rising_edge()
{
    uint32_t now = micros();
    uint32_t dust_low_duration = TIME_DIFF(now, instance.mv_last_low);
    instance.mv_low_accumulator += dust_low_duration;
    attachInterrupt(instance.m_pin, DustSensor::on_falling_edge, FALLING);
}

DustSensor::DustSensor() : m_samplingInterval(DEFAULT_SAMPLING_INTERVAL)
{
}

int8_t DustSensor::begin()
{
    return begin(DEFAULT_PIN);
}

int8_t DustSensor::begin(uint8_t pin)
{
    m_pin = digitalPinToInterrupt(pin);
    if (m_pin < 0)
        return ERROR_INVALID_PIN;

    noInterrupts();
    pinMode(pin, INPUT);

    mv_last_sample = micros();
    mv_last_low = 0;
    mv_low_accumulator = 0;

    attachInterrupt(instance.m_pin, DustSensor::on_falling_edge, FALLING);

    interrupts();
    return SUCCESS;
}

volatile void DustSensor::sample()
{
    uint32_t now = micros();
    uint32_t dust_sample_duration = TIME_DIFF(now, mv_last_sample);
    float dust_low_ratio = ((float)mv_low_accumulator) / dust_sample_duration;
    m_low_ratio = dust_low_ratio;
    m_concentration = 1.1f * pow(m_low_ratio, 3.0f) - 3.8f * pow(m_low_ratio, 2.0f) + 520.0f * m_low_ratio + 0.62f;
    mv_last_sample = now;
    mv_low_accumulator = 0;
}

volatile void DustSensor::update()
{
    uint32_t now = micros();
    if (now >= mv_last_sample + m_samplingInterval) {
        sample();
    }
}

void DustSensor::end()
{
    detachInterrupt(m_pin);
}
