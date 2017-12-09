#include <Arduino.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "printf.hpp"
#include "sensors.hpp"
#include "sensor_data.hpp"
#include "sd_files.hpp"

//////////////////////
// Global variables //
//////////////////////
Sensors sensors;
SensorsData sensor_data;
SdFiles& sd_files = SdFiles::instance;
uint32_t last_sample;

///////////////////////////
// Operational functions //
///////////////////////////

void setup()
{
    Serial.begin(SERIAL_BAUD);
    infoln(PRINTF("EnvironDatalogger v%d setup", 1));
    sensors.begin();
    last_sample = micros();
    sensor_data.time_rtc = sensors.rtc.now();

    sd_files.begin(&sensor_data.time_rtc);

    sensor_data.print_csv_header(Serial);
    if (sd_files.isOpen()) {
        sensor_data.print_csv_header(sd_files.file());
    }
}

void loop()
{
    sensor_data.time_micros = micros();
    sensors.update();

    if (TIME_DIFF(sensor_data.time_micros, last_sample) >= SAMPLE_INTERVAL)
    {
        sensors.read(&sensor_data);

        // debugln("Printing sensor data: ");
        // sensor_data.print_human(Serial);
        sensor_data.print_csv(Serial);
        if (sd_files.isOpen()) {
            sensor_data.print_csv(sd_files.file());
            debug(F("SdFiles.sync..."));
            bool res = sd_files.sync();
            if (!res) {
                infoln(F("SdFiles.sync fail!"));
            } else {
                debugln(F("good"));
            }
        }

        last_sample = sensor_data.time_micros;
    }
}
