#define F_CPU 8000000
#define BAUD 9600
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "com.h"
#include "Input.h"
#include "Output.h"
#include "I2C.h"
//#include "SPI.h"
#include "ds3231.h"
void * operator new(size_t size)
{
	return malloc(size);
}
void * operator new[](size_t size)
{
	return malloc(size);
}
void operator delete(void * ptr)
{
	free(ptr);
}
void operator delete[](void * ptr)
{
	free(ptr);
}

ds3231 rtc;

void output_time(char* out){
    out[0] = '0'+rtc.t.mday/10;
    out[1] = '0'+rtc.t.mday%10;
    out[2] = '.';
    out[3] = '0'+rtc.t.mon/10;
    out[4] = '0'+rtc.t.mon%10;
    out[5] = '.';
    out[6] = '0'+rtc.t.year/1000;
    out[7] = '0'+rtc.t.year/1000/100;
    out[8] = '0'+rtc.t.year_s/10;
    out[9] = '0'+rtc.t.year_s%10;

    out[10] = 'T';
    out[11] = '0'+rtc.t.hour/10;
    out[12] = '0'+rtc.t.hour%10;
    out[13] = ':';
    out[14] = '0'+rtc.t.min/10;
    out[15] = '0'+rtc.t.min%10;
    out[16] = ':';
    out[17] = '0'+rtc.t.sec/10;
    out[18] = '0'+rtc.t.sec%10;
    out[19] = '\n';
    out[20] = '\r';
    out[21] = '\0';
}

ISR(USART_RX_vect){
    uint8_t temp = uart_getc();
    if (temp == 't'){
        rtc.get();
        char outpp[30];
        output_time(outpp);
        uart_puts(outpp);
    }
}

int main(void){
    uart_init();
    sei();

    while(true){


    }
    return 0;
}
