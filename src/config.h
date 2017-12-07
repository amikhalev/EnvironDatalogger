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

// in microseconds
#define SAMPLE_INTERVAL ((uint32_t) 10 * 1000 * 1000)

#define TIME_DIFF(t1, t2) ((t1 > t2) ? (t1 - t2) : (t2 - t1))

#endif // __CONFIG_H__