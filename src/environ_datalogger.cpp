#include <Arduino.h>

// These includes are not used directly, but are required for other library includes to work
#include <Wire.h>
#include <SPI.h>
#include <MultichannelGasSensor.h>
// #include <MutichannelGasSensor.h>
#include <Adafruit_BME280.h>

MultichannelGasSensor gas;

/////////////
// Defines //
/////////////
#define SERIAL_BAUD (115200)
#define DUST_PIN (2)
#define DUST_INTERRUPT_PIN digitalPinToInterrupt(DUST_PIN)
// the default I2C address of the gas sensor slave is 0x04
#define GAS_ADDR (0x04)
#define BME280_ADDR (0x76)
#define TIME_DIFF(t1, t2) ((t1 > t2) ? (t1 - t2) : (t2 - t1))

///////////
// Types //
///////////
struct SensorsState
{
    uint32_t dust_last_sample;
    uint32_t dust_last_low;
    uint32_t dust_low_accumulator;
    Adafruit_BME280 environment_sensor;
};

struct SensorsData
{
    float dust_low_ratio_raw; // percentage from 0-1
    float dust_concentration; // particles / 0.01 ft^3

    float gas_concentration_nh3; // ppm
    float gas_concentration_co; // ppm
    float gas_concentration_no2; // ppm
    float gas_concentration_c3h8; // ppm
    float gas_concentration_c4h10; // ppm
    float gas_concentration_h2; // ppm
    float gas_concentration_c2h5oh; // ppm

    float environment_temperature; // degrees celcius (°C)
    float environment_humidity; // relative humidity percentage
    float environment_pressure; // atmospheric pressure (kPa)
};

//////////////////////
// Global variables //
//////////////////////
SensorsState sensor_state;
SensorsData sensor_data;
uint32_t last_sample;

///////////////////////
// Library functions //
///////////////////////
void sensors_init()
{
    // Dust sensor
    sensor_state.dust_last_sample = micros();
    sensor_state.dust_last_low = 0;
    sensor_state.dust_low_accumulator = 0;
    // This means that it is using I2C
    sensor_state.environment_sensor = Adafruit_BME280();
}

void sensors_dust_rising();

void sensors_setup()
{
    pinMode(DUST_PIN, INPUT);
    sensors_dust_rising();

    gas.begin(GAS_ADDR);
    gas.powerOn();

    bool result = sensor_state.environment_sensor.begin(BME280_ADDR, &Wire);
    if (!result) {
        Serial.println("BME280 not found!");
    }

    Serial.print("Gas sensor firmware version = ");
    Serial.println(gas.firmwareVersion());
    // Serial.print("Gas sensor firmware ");
    // gas.getVersion();
}

void sensors_dust_falling()
{
    attachInterrupt(DUST_INTERRUPT_PIN, sensors_dust_rising, RISING);
    sensor_state.dust_last_low = micros();
}

void sensors_dust_rising()
{
    uint32_t now = micros();
    uint32_t dust_low_duration = TIME_DIFF(now, sensor_state.dust_last_low);
    sensor_state.dust_low_accumulator += dust_low_duration;
    attachInterrupt(DUST_INTERRUPT_PIN, sensors_dust_falling, FALLING);
}

void sensors_update()
{
}

void sensors_data_read(SensorsData *sensor_data)
{
    uint32_t now = micros();
    uint32_t dust_sample_duration = TIME_DIFF(now, sensor_state.dust_last_sample);
    float dust_low_ratio = ((float)sensor_state.dust_low_accumulator) / dust_sample_duration;
    sensor_data->dust_low_ratio_raw = dust_low_ratio;
    sensor_data->dust_concentration = 1.1f * pow(dust_low_ratio, 3.0f) - 3.8f * pow(dust_low_ratio, 2.0f) + 520.0f * dust_low_ratio + 0.62f;
    sensor_state.dust_last_sample = now;
    sensor_state.dust_low_accumulator = 0;

    sensor_data->gas_concentration_nh3 = gas.measure_NH3();
    sensor_data->gas_concentration_co = gas.measure_CO();
    sensor_data->gas_concentration_no2 = gas.measure_NO2();
    sensor_data->gas_concentration_c3h8 = gas.measure_C3H8();
    sensor_data->gas_concentration_c4h10 = gas.measure_C4H10();
    sensor_data->gas_concentration_h2 = gas.measure_H2();
    sensor_data->gas_concentration_c2h5oh = gas.measure_C2H5OH();

    sensor_data->environment_temperature = sensor_state.environment_sensor.readTemperature();
    sensor_data->environment_humidity = sensor_state.environment_sensor.readHumidity();
    sensor_data->environment_pressure = sensor_state.environment_sensor.readPressure();
}

void sensors_data_print_human(SensorsData *sensor_data, Print& output)
{
    output.println("=== SensorsData: ");

#define PRINT_VALUE(key, units)                        \
    output.print("-> ");                               \
    output.print(#key);                                \
    output.print(" = ");                               \
    if (sensor_data->key >= 0)                         \
        output.print(sensor_data->key); \
    else                                               \
        output.print("invalid");                       \
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

///////////////////////////
// Operational functions //
///////////////////////////

void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("Starting environmental datalogger");

    sensors_init();
    sensors_setup();

    last_sample = micros();
}

void loop()
{
    uint32_t now = micros();
    sensors_update();

    if ((now - last_sample) >= 1E+6)
    {
        Serial.println("Reading sensor data");
        sensors_data_read(&sensor_data);
        sensors_data_print_human(&sensor_data, Serial);

        last_sample = now;
    }
}
