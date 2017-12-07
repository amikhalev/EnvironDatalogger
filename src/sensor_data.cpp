#include "sensor_data.hpp"

#include "datetime_util.hpp"

void SensorsData::reset()
{
    time_micros = 0;
    time_rtc = DateTime(0);

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
#define PRINT_VALUE(key, units)        \
    output.print(F("-> " #key " = ")); \
    if (this->key >= 0)                \
        output.print(this->key);       \
    else                               \
        output.print(F("invalid"));    \
    output.println(F(units));

    PRINT_VALUE(time_micros, "μs")
    output.print(F("-> time_rtc = "));
    print_datetime(time_rtc, output);
    output.println();
    PRINT_VALUE(dust_low_ratio_raw, "%")
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

void SensorsData::print_csv(Print &output)
{
    output.print(time_micros);
    output.print(',');
    print_datetime(time_rtc, output);
    output.print(',');
    output.print(dust_low_ratio_raw);
    output.print(',');
    output.print(dust_concentration);
    output.print(',');
    output.print(gas_concentration_nh3);
    output.print(',');
    output.print(gas_concentration_co);
    output.print(',');
    output.print(gas_concentration_no2);
    output.print(',');
    output.print(gas_concentration_c3h8);
    output.print(',');
    output.print(gas_concentration_c4h10);
    output.print(',');
    output.print(gas_concentration_h2);
    output.print(',');
    output.print(gas_concentration_c2h5oh);
    output.print(',');
    output.print(environment_temperature);
    output.print(',');
    output.print(environment_humidity);
    output.print(',');
    output.print(environment_pressure);
    output.println();
}

void SensorsData::print_csv_header(Print &output)
{
    output.println(
        "time_micros,time_rtc,"
        "dust_raw,dust_conc,"
        "gas_nh3_conc,gas_co_conc,gas_no2_conc,gas_c3h8_conc,gas_c3h10_conc,gas_h2_conc,gas_c2h4oh_conc,"
        "env_temperature,env_humidity,env_pressure");
}
