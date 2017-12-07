#include <Arduino.h>

#include "config.h"
#include "sensors.hpp"

//////////////////////
// Global variables //
//////////////////////
Sensors sensors;
SensorsData sensor_data;
uint32_t last_sample;

///////////////////////////
// Operational functions //
///////////////////////////
void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("Starting environmental datalogger");

    sensors.begin();

    last_sample = micros();
}

void loop()
{
    uint32_t now = micros();
    sensors.update();

    if (TIME_DIFF(now, last_sample) >= SAMPLE_INTERVAL)
    {
        Serial.println("Reading sensor data");
        sensors.read(&sensor_data);

        Serial.println("Printing sensor data: ");
        sensor_data.print_human(Serial);

        last_sample = now;
    }
}
