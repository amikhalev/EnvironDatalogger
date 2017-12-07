#include "sensors.hpp"

#include "config.h"

Sensors::Sensors() : dust(DustSensor::instance), // use the singleton instance
                     gas(),
                     environment() // use the i2c bus
{
}

void Sensors::begin()
{
    int16_t res;
    res = dust.begin(DUST_PIN);
    if (res < 0)
    {
        Serial.println("DustSensor begin fail");
    }

    res = gas.begin(GAS_ADDR);
    gas.powerOn();
    if (res < 0)
    {
        Serial.println("MultichannelGasSensor begin fail");
    }
    else
    {
        Serial.print("Gas sensor firmware version = ");
        Serial.println(res);
    }

    res = environment.begin(BME280_ADDR, &Wire);
    if (!res)
    {
        Serial.println("BME280 begin fail");
    }
}

void Sensors::update()
{
    // dust.update();
}

void Sensors::read(SensorsData *data)
{
    int16_t res;
    dust.sample();
    res = gas.read();
    if (res < 0) {
        Serial.print("MultichannelGasSensor read fail: ");
        Serial.println(res);
    }

    data->dust_low_ratio_raw = dust.getLowRatio();
    data->dust_concentration = dust.getConcentration();

    data->gas_concentration_nh3 = gas.measure_NH3();
    data->gas_concentration_co = gas.measure_CO();
    data->gas_concentration_no2 = gas.measure_NO2();
    data->gas_concentration_c3h8 = gas.measure_C3H8();
    data->gas_concentration_c4h10 = gas.measure_C4H10();
    data->gas_concentration_h2 = gas.measure_H2();
    data->gas_concentration_c2h5oh = gas.measure_C2H5OH();

    data->environment_temperature = environment.readTemperature();
    data->environment_humidity = environment.readHumidity();
    data->environment_pressure = environment.readPressure();
}

void SensorsData::reset()
{
    dust_low_ratio_raw = 0.f;
    dust_concentration = 0.f;

    gas_concentration_nh3 = 0.f;
    gas_concentration_co = 0.f;
    gas_concentration_no2 = 0.f;
    gas_concentration_c3h8 = 0.f;
    gas_concentration_c4h10 = 0.f;
    gas_concentration_h2 = 0.f;
    gas_concentration_c2h5oh = 0.f;

    environment_temperature = 0.f;
    environment_humidity = 0.f;
    environment_pressure = 0.f;
}

void SensorsData::print_human(Print &output)
{
#define PRINT_VALUE(key, units)  \
    output.print("-> ");         \
    output.print(#key);          \
    output.print(" = ");         \
    if (this->key >= 0)          \
        output.print(this->key); \
    else                         \
        output.print("invalid"); \
    output.println(units);

    PRINT_VALUE(dust_low_ratio_raw, " %")
    PRINT_VALUE(dust_concentration, " pcs/0.01cf")
    PRINT_VALUE(gas_concentration_nh3, " ppm")
    PRINT_VALUE(gas_concentration_co, " ppm")
    PRINT_VALUE(gas_concentration_no2, " ppm")
    PRINT_VALUE(gas_concentration_c3h8, " ppm")
    PRINT_VALUE(gas_concentration_c4h10, " ppm")
    PRINT_VALUE(gas_concentration_h2, " ppm")
    PRINT_VALUE(gas_concentration_c2h5oh, " ppm")
    PRINT_VALUE(environment_temperature, " ˚C")
    PRINT_VALUE(environment_humidity, "%")
    PRINT_VALUE(environment_pressure, " kPa")

#undef PRINT_VALUE
}
