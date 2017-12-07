/*******************************************************************************
 * Configuration constants for EnvironDatalogger
 *******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SERIAL_BAUD (115200)

#define DUST_PIN (2)
// the default I2C address of the gas sensor slave is 0x04
#define GAS_ADDR (0x04)
#define BME280_ADDR (0x76)

#define SD_CS_PIN (9)
#define SD_SPI_SETTINGS (SPISettings(1000000UL * 10, MSBFIRST, SPI_MODE0))

// in microseconds
#define SAMPLE_INTERVAL ((uint32_t)10 * 1000 * 1000)

#define TIME_DIFF(t1, t2) ((t1 > t2) ? (t1 - t2) : (t2 - t1))

// #define MULTICHANNELGASSENSOR_DEBUG 1
// #define DEBUG 1

#define info(...) Serial.print(__VA_ARGS__)
#define infoln(...) Serial.println(__VA_ARGS__)

#if DEBUG
#define debug(...) Serial.print(__VA_ARGS__)
#define debugln(...) Serial.print(__VA_ARGS__)
#else
#define debug(...)
#define debugln(...)
#endif

#endif // __CONFIG_H__