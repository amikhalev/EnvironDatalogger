#ifndef __SENSORS_HPP__
#define __SENSORS_HPP__

// These includes are not used directly, but are required for other library includes to work
#include <Wire.h>
#include <SPI.h>

#include <MultichannelGasSensor.hpp>
#include <Adafruit_BME280.h>
#include <RTClib.h>
#include "dust_sensor.hpp"

struct SensorsData;

class Sensors
{
  public: // fields
    DustSensor &dust;
    MultichannelGasSensor gas;
    Adafruit_BME280 environment;

  public: // methods
    Sensors();

    void begin();
    void update();
    void read(SensorsData *data);
};

#endif // __SENSORS_HPP__