/*
    MultichannelGasSensor.h
    2015 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author: Jacky Zhang
    2015-3-17
    http://www.seeed.cc/
    modi by Jack, 2015-8
    
    V2 by Loovee
    2016-11-11

    Modified by amikhalev
    2017-12-05

    The MIT License (MIT)

    Copyright (c) 2015 Seeed Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __MULTICHANNELGASSENSOR_H__
#define __MULTICHANNELGASSENSOR_H__

#include <stdint.h>

class MultichannelGasSensor
{
  public:
    enum : uint8_t
    {
        DEFAULT_I2C_ADDR = 0x04
    };

    enum : uint32_t
    {
        DEFAULT_READ_TIMEOUT = 10000 // 100 ms
    };

    enum Address : uint8_t
    {
        kAddrIsSet = 0,

        // Factory calibration values
        kAddrFactoryADC_NH3 = 2,
        kAddrFactoryADC_CO = 4,
        kAddrFactoryADC_NO2 = 6,

        // User calibration values
        kAddrUserADC_NH3 = 8,
        kAddrUserADC_CO = 10,
        kAddrUserADC_NO2 = 12,

        kAddrIfCali = 14,
        kAddrI2CAddress = 20
    };

    enum Command : uint8_t
    {
        kCmdAdcRes0 = 1, // NH3
        kCmdAdcRes1 = 2, // CO
        kCmdAdcRes2 = 3, // NO2
        kCmdAdcResAll = 4,
        kCmdChangeI2CAddr = 5,
        kCmdReadEEPROM = 6,
        kCmdSetR0Adc = 7,
        kCmdGetR0Adc = 8,
        kCmdGetR0AdcFactory = 9,
        kCmdControlLed = 10,
        kCmdControlPower = 11,
        kCmdGetAdcR0Res0V1 = 0x11,
        kCmdGetAdcR0Res1V1 = 0x12,
        kCmdGetAdcR0Res2V1 = 0x13,
        kCmdPowerOnV1 = 0x21,
        kCmdPowerOffV1 = 0x20,
        kCmdCalibrateV1 = 0x22,
        kCmdChangeI2CAddrV1 = 0x23
    };

    enum Channel : uint8_t
    {
        kChannel_NH3 = kCmdAdcRes0,
        kChannel_CO = kCmdAdcRes1,
        kChannel_NO2 = kCmdAdcRes2
    };

    enum GasType : uint8_t
    {
        kGas_CO,
        kGas_NO2,
        kGas_NH3,
        kGas_C3H8,
        kGas_C4H10,
        kGas_CH4,
        kGas_H2,
        kGas_C2H5OH
    };

    enum OnState : uint8_t
    {
        OFF = 0,
        ON = 1
    };

    enum Error : int16_t
    {
        ERROR_GENERAL = -1,
        ERROR_TIMEOUT = -2,
        ERROR_I2C = -3,
        ERROR_CHECKSUM = -4,
        ERROR_UNSUPPORTED = -5
    };

  private:
    uint8_t m_i2cAddr;
    uint8_t m_firmwareVersion;
    uint32_t m_readTimeout;

    uint8_t m_adcValueBufs[3];

  private:
    // convenience methods for writing to i2c
    void i2c_write(uint8_t data1);
    void i2c_write(uint8_t data1, uint8_t data2);
    void i2c_write(uint8_t *data, uint8_t data_len);

    // methods for safely (ie. check available) reading from i2c
    int16_t i2c_read(uint8_t *data, uint8_t data_len);
    // handles endian conversion & checksum on fw v1
    int16_t i2c_read_int16();

    // reads a 16 bit response to a command
    int16_t read_command_int16(Command cmd);
    // reads a 16 bit value from the eeprom
    int16_t read_eeprom(Address addr);

    // loads the firmware version from eeprom and stores it
    uint8_t load_firmware_version();

    int16_t read_channel_rs(Channel ch);
    int16_t read_channel_r0(Channel ch);

  public:
    MultichannelGasSensor(); 

    /** Setup gas sensor on i2c address */
    void begin(int address);
    /** Setup gas sensor on default i2c address */
    void begin();
    /** gets the firmware version. either 1 or 2 */
    inline uint8_t firmwareVersion() { return m_firmwareVersion; }

    void setPower(OnState on);
    inline void powerOn() { setPower(ON); }
    inline void powerOff() { setPower(OFF); }

    void setLED(OnState on);
    inline void ledOn() { setLED(ON); }
    inline void ledOff() { setLED(OFF); }

    void doCalibrate(void);

    //get gas concentration, unit: ppm
    float measure(GasType gas);
    inline float measure_CO() { return measure(kGas_CO); }
    inline float measure_NO2() { return measure(kGas_NO2); }
    inline float measure_NH3() { return measure(kGas_NH3); }
    inline float measure_C3H8() { return measure(kGas_C3H8); }
    inline float measure_C4H10() { return measure(kGas_C4H10); }
    inline float measure_CH4() { return measure(kGas_CH4); }
    inline float measure_H2() { return measure(kGas_H2); }
    inline float measure_C2H5OH() { return measure(kGas_C2H5OH); }

    void print_eeprom();
    float printR0(Channel ch);
    float printRs(Channel ch);
    void factory_setting();
    void change_i2c_address(uint8_t addr);
};

#endif // __MULTICHANNELGASSENSOR_H__
