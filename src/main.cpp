#include <Arduino.h>
#include <avr/pgmspace.h>
#include <FreeStack.h>

#include "config.h"
#include "printf.hpp"
#include "sensors.hpp"
#include "sensor_data.hpp"
#include "sd_files.hpp"
#include "datetime_util.hpp"

//////////////////////
// Global variables //
//////////////////////
Sensors sensors;
SensorsData sensor_data;
SdFiles& sd_files = SdFiles::instance;
uint32_t last_sample;

char serial_command;
char serial_command_data[COMMAND_MAX_LEN];
int16_t serial_command_len;

///////////////////////////
// Operational functions //
///////////////////////////

void process_serial_command() {
    switch (serial_command) {
        case 't': {
            uint16_t year; uint8_t month, day, hour, min, sec;
            year = serial_command_data[0];
            year |= serial_command_data[1] << 8;
            month = serial_command_data[2];
            day = serial_command_data[3];
            hour = serial_command_data[4];
            min = serial_command_data[5];
            sec = serial_command_data[6];
            DateTime new_time(year, month, day, hour, min, sec);
            sensors.rtc.adjust(new_time);
#if DEBUG
            debug("adjust time to "); print_datetime(new_time); debugln();
#endif
            break;
        }
        case '\0':
            break;
        default:
            debugf("invalid serial command %c\n", serial_command);
            break;
    }
}

uint16_t get_serial_command_len(/* char serial_command */) {
    switch (serial_command) {
        case 't': return 7;
        case '\0': return 0;
        default: return 0;
    }
}

void setup()
{
    serial_command_len = -1;

    Serial.begin(SERIAL_BAUD);
    infof("EnvironDatalogger v%d setup", 1);
    sensors.begin();
    last_sample = micros();
    sensor_data.time_rtc = sensors.rtc.now();

    infof("FreeStack: %u\n", FreeStack());

    sd_files.begin(&sensor_data.time_rtc);

    infof("FreeStack: %u\n", FreeStack());

    sensor_data.print_csv_header(Serial);
    if (sd_files.isOpen()) {
        sensor_data.print_csv_header(sd_files.file());
    }
}

void read_serial();

void loop()
{
    sensor_data.time_micros = micros();
    sensors.update();

    read_serial();

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

void read_serial() {
    while (Serial.available()) {
        char inChar = (char) Serial.read();
        if (serial_command_len < 0) { 
            serial_command = (char) Serial.read();
            serial_command_len = 0;
            continue;
        }
        if (serial_command_len >= COMMAND_MAX_LEN) {
            // something weird happen, throw away all data and hope it fixes itself
            debugln("serial input buffer overflow");
            serial_command_len = 0;
        }
        serial_command_data[serial_command_len++] = inChar;
        if (serial_command_len >= get_serial_command_len()) {
            process_serial_command();
            serial_command_len = 0;
        }
    }
}
