/*
    MultichannelGasSensor.cpp
    2015 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author: Jacky Zhang
    2015-3-17
    http://www.seeed.cc/
    modi by Jack, 2015-8

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

#include <math.h>
#include <Wire.h>
#include <Arduino.h>
#include "MultichannelGasSensor.hpp"

MultichannelGasSensor::MultichannelGasSensor() : m_i2cAddr(DEFAULT_I2C_ADDR),
                                                 m_firmwareVersion(1),
                                                 m_readTimeout(DEFAULT_READ_TIMEOUT),
                                                 m_r0Buffers{-1, -1, -1},
                                                 m_ratioBuffers{NAN, NAN, NAN}
{
}

void MultichannelGasSensor::begin(uint8_t address)
{
    Wire.begin();
    m_i2cAddr = address;
    read_firmware_version();
}

void MultichannelGasSensor::begin()
{
    begin(DEFAULT_I2C_ADDR);
}

void MultichannelGasSensor::i2c_write(uint8_t data1)
{
    Wire.beginTransmission(m_i2cAddr);
    Wire.write(data1);
    Wire.endTransmission();
}

void MultichannelGasSensor::i2c_write(uint8_t data1, uint8_t data2)
{
    Wire.beginTransmission(m_i2cAddr);
    Wire.write(data1);
    Wire.write(data2);
    Wire.endTransmission();
}

void MultichannelGasSensor::i2c_write(uint8_t *data, uint8_t data_len)
{
    Wire.beginTransmission(m_i2cAddr);
    Wire.write(data, data_len);
    Wire.endTransmission();
}

int16_t MultichannelGasSensor::i2c_read(uint8_t *data, uint8_t data_len)
{
    size_t i = 0;
    Wire.requestFrom(m_i2cAddr, data_len);
    uint32_t start_time = micros();
    while (Wire.available() == 0)
    {
        if (micros() >= start_time + m_readTimeout)
        {
            return ERROR_TIMEOUT;
        }
        delay(2); // delay 2ms
    }
    if (Wire.available() != data_len)
    {
        return ERROR_I2C;
    }
    for (; i < data_len; i++)
    {
        data[i] = Wire.read();
    }
    return i;
}

int16_t MultichannelGasSensor::i2c_read_int16()
{
    int16_t res = ERROR_UNSUPPORTED; // unsupported firmware
    if (m_firmwareVersion == 1)
    {
        uint8_t raw[4];
        res = i2c_read(raw, sizeof(raw));
        if (res < 0)
            return res;
        uint8_t checksum = (uint8_t)(raw[0] + raw[1] + raw[2]);
        if (checksum != raw[4])
            return ERROR_CHECKSUM;
        res = raw[2];
        res |= raw[1] << 8;
    }
    else if (m_firmwareVersion == 2)
    {
        uint8_t raw[2];
        res = i2c_read(raw, sizeof(raw));
        if (res < 0)
            return res;
        res = raw[1];
        res |= raw[0] << 8;
    }
    return res;
}

int16_t MultichannelGasSensor::read_command_int16(MultichannelGasSensor::Command cmd)
{
#ifdef MULTICHANNELGASSENSOR_DEBUG
    Serial.print("MultichannelGasSensor::read_command_int16(");
    Serial.print(cmd);
    Serial.print(") = ");
#endif
    i2c_write(cmd);
    int16_t res = i2c_read_int16();
#ifdef MULTICHANNELGASSENSOR_DEBUG
    Serial.println(res);
#endif
    return res;
}

int16_t MultichannelGasSensor::read_eeprom(MultichannelGasSensor::Address addr)
{
#ifdef MULTICHANNELGASSENSOR_DEBUG
    Serial.print("MultichannelGasSensor::read_eeprom(");
    Serial.print(addr);
    Serial.print(") = ");
#endif
    i2c_write(kCmdReadEEPROM, addr);
    int16_t res = i2c_read_int16();
#ifdef MULTICHANNELGASSENSOR_DEBUG
    Serial.println(res);
#endif
    return res;
}

uint8_t MultichannelGasSensor::read_firmware_version()
{
    m_firmwareVersion = 2;               // force use new protocol
    if (read_eeprom(kAddrIsSet) == 1126) // if it succeeded and is the new version
        m_firmwareVersion = 2;
    else
        m_firmwareVersion = 1;
    return m_firmwareVersion;
}

void MultichannelGasSensor::setPower(MultichannelGasSensor::OnState on)
{
    if (m_firmwareVersion == 1)
    {
        uint8_t command = on ? kCmdPowerOnV1 : kCmdPowerOffV1;
        i2c_write(command);
    }
    else if (m_firmwareVersion == 2)
    {
        i2c_write(kCmdControlPower, on);
    }
}

void MultichannelGasSensor::setLED(MultichannelGasSensor::OnState on)
{
    if (m_firmwareVersion == 1)
    {
        return; // unsupported
    }
    else if (m_firmwareVersion == 2)
    {
        i2c_write(kCmdControlLed, on);
    }
}

int16_t MultichannelGasSensor::read_channel_rs(MultichannelGasSensor::Channel ch)
{
    // command is same regardless of version
    int16_t data = read_command_int16((Command)ch);
    return data;
}

int16_t MultichannelGasSensor::read_channel_r0(MultichannelGasSensor::Channel ch)
{
    switch (ch)
    {
    case kChannel_NH3:
        if (m_firmwareVersion == 1)
            return read_command_int16(kCmdGetAdcR0Res0V1);
        else if (m_firmwareVersion == 2)
            return read_eeprom(kAddrUserADC_NH3);
    case kChannel_CO:
        if (m_firmwareVersion == 1)
            return read_command_int16(kCmdGetAdcR0Res1V1);
        else if (m_firmwareVersion == 2)
            return read_eeprom(kAddrUserADC_CO);
    case kChannel_NO2:
        if (m_firmwareVersion == 1)
            return read_command_int16(kCmdGetAdcR0Res2V1);
        else if (m_firmwareVersion == 2)
            return read_eeprom(kAddrUserADC_NO2);
    }
    return ERROR_UNSUPPORTED;
}

int16_t MultichannelGasSensor::get_channel_r0(MultichannelGasSensor::Channel ch)
{
    int16_t data = ERROR_UNSUPPORTED;
    int16_t *buffer = NULL;
    switch (ch)
    {
    case kChannel_NH3:
        buffer = &m_r0Buffers[0];
        break;
    case kChannel_CO:
        buffer = &m_r0Buffers[1];
        break;
    case kChannel_NO2:
        buffer = &m_r0Buffers[2];
        break;
    }
    if (buffer != NULL && buffer >= 0)
        return *buffer;
    data = read_channel_r0(ch);
    if (buffer != NULL && data >= 0)
        *buffer = data;
    return data;
}

int16_t MultichannelGasSensor::read()
{
    int16_t res = ERROR_UNSUPPORTED;
    ledOn();

    int16_t A0_0, A0_1, A0_2, An_0, An_1, An_2;
#define GET_VALUE(VAR, FUN) \
    if ((VAR = FUN) < 0)    \
    {                       \
        res = VAR;          \
        goto fail;          \
    }
    GET_VALUE(A0_0, get_channel_r0(kChannel_NH3))
    GET_VALUE(A0_1, get_channel_r0(kChannel_CO))
    GET_VALUE(A0_2, get_channel_r0(kChannel_NO2))
    GET_VALUE(An_0, read_channel_rs(kChannel_NH3))
    GET_VALUE(An_1, read_channel_rs(kChannel_CO))
    GET_VALUE(An_2, read_channel_rs(kChannel_NO2))
#undef GET_VALUE

    if (1 == m_firmwareVersion)
    {
        m_ratioBuffers[0] = (float)An_0 / A0_0;
        m_ratioBuffers[1] = (float)An_1 / A0_1;
        m_ratioBuffers[2] = (float)An_2 / A0_2;
    }
    else if (2 == m_firmwareVersion)
    {
        m_ratioBuffers[0] = (float)An_0 / (float)A0_0 * (1023.0 - A0_0) / (1023.0 - An_0);
        m_ratioBuffers[1] = (float)An_1 / (float)A0_1 * (1023.0 - A0_1) / (1023.0 - An_1);
        m_ratioBuffers[2] = (float)An_2 / (float)A0_2 * (1023.0 - A0_2) / (1023.0 - An_2);
    }

fail:
    return res;
    ledOff();
}

float MultichannelGasSensor::measure(MultichannelGasSensor::GasType gas)
{
    float c = NAN;

    switch (gas)
    {
    case kGas_CO:
        c = pow(m_ratioBuffers[1], -1.179) * 4.385; //mod by jack
        break;
    case kGas_NO2:
        c = pow(m_ratioBuffers[2], 1.007) / 6.855; //mod by jack
        break;
    case kGas_NH3:
        c = pow(m_ratioBuffers[0], -1.67) / 1.47; //modi by jack
        break;
    case kGas_C3H8: //add by jack
        c = pow(m_ratioBuffers[0], -2.518) * 570.164;
        break;
    case kGas_C4H10: //add by jack
        c = pow(m_ratioBuffers[0], -2.138) * 398.107;
        break;
    case kGas_CH4: //add by jack
        c = pow(m_ratioBuffers[1], -4.363) * 630.957;
        break;
    case kGas_H2: //add by jack
        c = pow(m_ratioBuffers[1], -1.8) * 0.73;
        break;
    case kGas_C2H5OH: //add by jack
        c = pow(m_ratioBuffers[1], -1.552) * 1.622;
        break;
    }

    return c;
}

/*********************************************************************************************************
** Function name:           doCalibrate
** Descriptions:            tell slave to do a calibration, it will take about 8s
                            after the calibration, must reread the R0 values
*********************************************************************************************************/
void MultichannelGasSensor::doCalibrate(void)
{

    if (1 == m_firmwareVersion)
    {
        Serial.println("Unsupported calibrate on fw v1");
        return;
    }
    else if (2 == m_firmwareVersion)
    {
        uint16_t i, a0, a1, a2, b0, b1, b2;
        while (1)
        {
            a0 = read_channel_rs(kChannel_NH3);
            a1 = read_channel_rs(kChannel_CO);
            a2 = read_channel_rs(kChannel_NO2);

            Serial.print(a0);
            Serial.print('\t');
            Serial.print(a1);
            Serial.print('\t');
            Serial.print(a2);
            Serial.println('\t');
            ledOn();

            int cnt = 0;
            for (i = 0; i < 20; i++)
            {
                b0 = read_channel_rs(kChannel_NH3);
                b1 = read_channel_rs(kChannel_CO);
                b2 = read_channel_rs(kChannel_NO2);
                if ((a0 - b0) > 2 || (b0 - a0) > 2)
                    cnt++;
                if ((a1 - b1) > 2 || (b1 - a1) > 2)
                    cnt++;
                if ((a2 - b2) > 2 || (b2 - a2) > 2)
                    cnt++;

                if (cnt > 5)
                {
                    break;
                }
                delay(1000);
            }

            ledOff();
            if (cnt <= 5)
                break;
            delay(200);
        }

        Serial.print("write user adc value: ");
        Serial.print(a0);
        Serial.print('\t');
        Serial.print(a1);
        Serial.print('\t');
        Serial.print(a2);
        Serial.println('\t');

        unsigned char tmp[7];

        tmp[0] = 7;

        tmp[1] = a0 >> 8;
        tmp[2] = a0 & 0xff;

        tmp[3] = a1 >> 8;
        tmp[4] = a1 & 0xff;

        tmp[5] = a2 >> 8;
        tmp[6] = a2 & 0xff;

        i2c_write(tmp, 7);
    }
}

void MultichannelGasSensor::print_eeprom()
{
    if (m_firmwareVersion == 1)
    {
        Serial.println("ERROR: display_eeprom() is NOT support by V1 firmware.");
        return;
    }

    Serial.print("ADDR_IS_SET = ");
    Serial.println(read_eeprom(kAddrIsSet));
    Serial.print("ADDR_FACTORY_ADC_NH3 = ");
    Serial.println(read_eeprom(kAddrFactoryADC_NH3));
    Serial.print("ADDR_FACTORY_ADC_CO = ");
    Serial.println(read_eeprom(kAddrFactoryADC_CO));
    Serial.print("ADDR_FACTORY_ADC_NO2 = ");
    Serial.println(read_eeprom(kAddrFactoryADC_NO2));
    Serial.print("ADDR_USER_ADC_NH3 = ");
    Serial.println(read_eeprom(kAddrUserADC_NH3));
    Serial.print("ADDR_USER_ADC_CO = ");
    Serial.println(read_eeprom(kAddrUserADC_CO));
    Serial.print("ADDR_USER_ADC_NO2 = ");
    Serial.println(read_eeprom(kAddrUserADC_CO));
    Serial.print("ADDR_I2C_ADDRESS = ");
    Serial.println(read_eeprom(kAddrI2CAddress));
}

float MultichannelGasSensor::printR0(Channel ch)
{
    if (m_firmwareVersion == 1)
    {
        Serial.println("ERROR: getR0() is NOT support by V1 firmware.");
        return -1;
    }

    int a = 0;
    switch (ch)
    {
    case kChannel_NH3: // NH3
        a = read_eeprom(kAddrUserADC_NH3);
        Serial.print("a_ch3 = ");
        Serial.println(a);
        break;

    case kChannel_CO: // CO
        a = read_eeprom(kAddrUserADC_CO);
        Serial.print("a_co = ");
        Serial.println(a);
        break;

    case kChannel_NO2: // NO2
        a = read_eeprom(kAddrUserADC_NO2);
        Serial.print("a_no2 = ");
        Serial.println(a);
        break;

    default:;
    }

    float r = 56.0 * (float)a / (1023.0 - (float)a);
    return r;
}

float MultichannelGasSensor::printRs(Channel ch)
{

    if (m_firmwareVersion == 1)
    {
        Serial.println("ERROR: getRs() is NOT support by V1 firmware.");
        return -1;
    }

    int a = 0;
    switch (ch)
    {
    case kChannel_NH3: // NH3
        a = read_command_int16(kCmdAdcRes0);
        break;

    case kChannel_CO: // CO
        a = read_command_int16(kCmdAdcRes1);
        break;

    case kChannel_NO2: // NO2
        a = read_command_int16(kCmdAdcRes2);
        break;

    default:;
    }

    float r = 56.0 * (float)a / (1023.0 - (float)a);
    return r;
}

// 1. change i2c address to 0x04
// 2. change adc value of R0 to default
void MultichannelGasSensor::factory_setting()
{
    uint8_t tmp[7];

    uint8_t error;
    uint8_t address = 0;

    for (address = 1; address < 127; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            // change i2c to 0x04

            Serial.print("I2C address is: 0x");
            Serial.println(address, HEX);
            Serial.println("Change I2C address to 0x04");

            Wire.beginTransmission(address);
            Wire.write(kCmdChangeI2CAddr);
            Wire.write(DEFAULT_I2C_ADDR);
            Wire.endTransmission();

            m_i2cAddr = DEFAULT_I2C_ADDR;
            delay(100);
            read_firmware_version();
            break;
        }
    }

    int16_t a0 = read_eeprom(kAddrFactoryADC_NH3);
    int16_t a1 = read_eeprom(kAddrFactoryADC_CO);
    int16_t a2 = read_eeprom(kAddrFactoryADC_NO2);

    tmp[0] = 7;
    tmp[1] = a0 >> 8;
    tmp[2] = a0 & 0xff;
    tmp[3] = a1 >> 8;
    tmp[4] = a1 & 0xff;

    tmp[5] = a2 >> 8;
    tmp[6] = a2 & 0xff;
    delay(100);
    i2c_write(tmp, 7);
    delay(100);
}

void MultichannelGasSensor::change_i2c_address(uint8_t addr)
{
    uint8_t cmd;
    if (m_firmwareVersion == 2)
        cmd = kCmdChangeI2CAddr;
    else
        cmd = kCmdChangeI2CAddrV1;
    i2c_write(cmd, addr);

    Serial.print("FUNCTION: CHANGE I2C ADDRESS: 0X");
    Serial.print(m_i2cAddr, HEX);
    Serial.print(" > 0x");
    Serial.println(addr, HEX);

    m_i2cAddr = addr;
}
