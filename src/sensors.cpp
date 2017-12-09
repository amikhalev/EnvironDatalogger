#include "sensors.hpp"

#include "config.h"
#include "sensor_data.hpp"
#include "datetime_util.hpp"

Sensors::Sensors() : dust(DustSensor::instance), // use the singleton instance
                     gas(),
                     environment(), // use the i2c bus
                     rtc()
{
}

void Sensors::begin()
{
    Serial.print(F("DustSensor::begin..."));
    int16_t res;
    res = dust.begin(DUST_PIN);
    if (res < 0)
    {
        Serial.print(F("fail: "));
        Serial.println(res);
    }
    else
    {
        Serial.println(F("good"));
    }

    Serial.print(F("MultichannelGasSensor::begin..."));
    res = gas.begin(GAS_ADDR);
    gas.powerOn();
    if (res < 0)
    {
        Serial.print(F("fail: "));
        Serial.println(res);
    }
    else
    {
        Serial.print(F("good. firmwareVersion="));
        Serial.println(res);
    }

    Serial.print(F("Adafruit_BME280::begin..."));
    res = environment.begin(BME280_ADDR, &Wire);
    if (!res)
    {
        Serial.println(F("fail"));
    }
    else
    {
        Serial.println(F("good"));
    }

    Serial.print(F("RTC_DS1307::begin..."));
    res = rtc.begin();
    if (!res)
    {
        Serial.println(F("fail"));
    }
    else
    {
        if (!rtc.isrunning())
            Serial.print(F("not running"));
        else
            Serial.print(F("good"));
#if DEBUG
        Serial.print(F(". now="));
        DateTime now = rtc.now();
        print_datetime(now, Serial);
#endif
        Serial.println();
    }
}

void Sensors::update()
{
    // dust.update();
}

void Sensors::read(SensorsData *data)
{
    int16_t res;

    debug("Sensors.read()...");
    dust.sample();
    res = gas.read();
    if (res < 0)
    {
        info(F("MultichannelGasSensor::read() fail: "));
        info(res);
        infoln('.');
    }

    // data->time_micros = micros();
    data->time_rtc = rtc.now();

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
    debugln();
}
