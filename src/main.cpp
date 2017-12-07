#include <Arduino.h>
#include <SdFat.h>

#include "config.h"
#include "sensors.hpp"
#include "sensor_data.hpp"
#include "printf.hpp"

//////////////////////
// Global variables //
//////////////////////
Sensors sensors;
SensorsData sensor_data;
uint32_t last_sample;
SdFat sd;

///////////////////////////
// Operational functions //
///////////////////////////
void setup()
{
    Serial.begin(SERIAL_BAUD);
    infoln(PRINTF("EnvironDatalogger v%d setup", 1));
    sensors.begin();
    last_sample = micros();

    if (!sd.begin(SD_CS_PIN, SD_SPI_SETTINGS)) {
        sd.initErrorPrint("SD card open");
    }

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
