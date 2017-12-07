#ifndef __SENSOR_DATA_HPP__
#define __SENSOR_DATA_HPP__

#include <Print.h>
#include <HardwareSerial.h>
#include <RTClib.h>

struct SensorsData
{
  public:                     // fields

    uint32_t time_micros; // current micros() time
    DateTime time_rtc;
    
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

inline SensorsData::SensorsData()
{
    reset();
}

#endif // __SENSOR_DATA_HPP__