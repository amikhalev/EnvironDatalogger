#include <Arduino.h>

#include "config.h"
#include "sensors.hpp"
#include "sensor_data.hpp"

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
    debugln(F("EnvironDatalogger v1 setup"));
    sensors.begin();
    last_sample = micros();

    sensor_data.print_csv_header(Serial);
}

void loop()
{
    uint32_t now = micros();
    sensors.update();

    if (TIME_DIFF(now, last_sample) >= SAMPLE_INTERVAL)
    {
        // debugln("Reading sensor data");
        sensors.read(&sensor_data);

        // debugln("Printing sensor data: ");
        // sensor_data.print_human(Serial);
        sensor_data.print_csv(Serial);

        last_sample = now;
    }
}
