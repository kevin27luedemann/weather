/*
 * HDC1080 Library for humidity and temperature readings
 *
 * Created by Kevin Luedemann
 * 15.07.2018 17:47
 */

#include "HDC1080.h"

hdc1080::hdc1080(){
    rawtemp = 0.;
    rawhum  = 0.;

    i2c.twi_start();
    i2c.twi_write(HDC1080_I2C_ADDR | I2C_WRITE);
    i2c.twi_write(HDC1080_CONFIG);
    i2c.twi_write(0x10);
    i2c.twi_stop();
}

hdc1080::~hdc1080(){}

void hdc1080::gettemphum(){
    i2c.twi_start();
    i2c.twi_write(HDC1080_I2C_ADDR | I2C_WRITE);
    i2c.twi_write(HDC1080_TEMP);
    i2c.twi_stop();
    _delay_ms(20);

    i2c.twi_start();
    i2c.twi_write(HDC1080_I2C_ADDR | I2C_READ);

    rawtemp = rawtemp   << 8 | i2c.twi_read(1);
    rawtemp = rawtemp   << 8 | i2c.twi_read(1);
    rawhum  = rawhum    << 8 | i2c.twi_read(1);
    rawhum  = rawhum    << 8 | i2c.twi_read(0);
    i2c.twi_stop();
}

float hdc1080::temperature(){
    return ((float)rawtemp)*165./65536.-40.;
}

float hdc1080::humidity(){
    return ((float)rawhum)*100./65536.;
}
