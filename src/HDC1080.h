/*
 * HDC1080 Library for humidity and temperature readings
 *
 * Created by Kevin Luedemann
 * 15.07.2018 17:47
 */

#ifndef __HDC1080_H__
#define __HDC1080_H__

#define HDC1080_I2C_ADDR    0b10000000
#define HDC1080_CONFIG      0x02
#define HDC1080_TEMP        0x00
#define HDC1080_HUM         0x01

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <util/delay.h>
#include "I2C.h"


class hdc1080
{
public:
    hdc1080();
    ~hdc1080();
    int16_t rawtemp;
    uint16_t rawhum;
    void gettemphum();
    void reset();
    float temperature();
    float humidity();

private:
    I2c i2c;
};



#endif
