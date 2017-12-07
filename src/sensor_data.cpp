#include "sensor_data.hpp"

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

    PRINT_VALUE(time_micros, "μs")
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
    Serial.print(time_micros); Serial.print(',');
    Serial.print(dust_low_ratio_raw); Serial.print(',');
    Serial.print(dust_concentration); Serial.print(',');
    Serial.print(gas_concentration_nh3); Serial.print(',');
    Serial.print(gas_concentration_co); Serial.print(',');
    Serial.print(gas_concentration_no2); Serial.print(',');
    Serial.print(gas_concentration_c3h8); Serial.print(',');
    Serial.print(gas_concentration_c4h10); Serial.print(',');
    Serial.print(gas_concentration_h2); Serial.print(',');
    Serial.print(gas_concentration_c2h5oh); Serial.print(',');
    Serial.print(environment_temperature); Serial.print(',');
    Serial.print(environment_humidity); Serial.print(',');
    Serial.print(environment_pressure); Serial.println();
}

void SensorsData::print_csv_header(Print &output)
{
    output.println(
        "time_micros,"
        "dust_raw,dust_conc,"
        "gas_nh3_conc,gas_co_conc,gas_no2_conc,gas_c3h8_conc,gas_c3h10_conc,gas_h2_conc,gas_c2h4oh_conc,"
        "env_temperature,env_humidity,env_pressure"
        );
}
