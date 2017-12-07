#ifndef __SENSORS_HPP__
#define __SENSORS_HPP__

#include <Wire.h>
#include <SPI.h>
// These includes are not used directly, but are required for other library includes to work
#include <MultichannelGasSensor.hpp>
#include <Adafruit_BME280.h>
#include "dust_sensor.hpp"

struct SensorsData
{
  public:                     // fields
    float dust_low_ratio_raw; // percentage from 0-1
    float dust_concentration; // particles / 0.01 ft^3

    float gas_concentration_nh3;    // ppm
    float gas_concentration_co;     // ppm
    float gas_concentration_no2;    // ppm
    float gas_concentration_c3h8;   // ppm
    float gas_concentration_c4h10;  // ppm
    float gas_concentration_h2;     // ppm
    float gas_concentration_c2h5oh; // ppm

    float environment_temperature; // degrees celcius (°C)
    float environment_humidity;    // relative humidity percentage
    float environment_pressure;    // atmospheric pressure (kPa)

  public: // methods
    inline SensorsData();

    void reset();

    void print_human(Print &out = Serial);
    void print_csv(Print &out = Serial);

    static void print_csv_header(Print &out = Serial);
};

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

inline SensorsData::SensorsData()
{
    reset();
}

#endif // __SENSORS_HPP__