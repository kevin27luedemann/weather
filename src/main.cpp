#define F_CPU 8000000
#define BAUD 9600
#define GMT 2
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
#include "HDC1080.h"
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

//ds3231 rtc;
hdc1080 sens;
Input SQM(&DDRB,&PORTB,&PINB,0,true);
uint8_t flag_reg;
#define CLOCK_TICK_BIT  0

void output_time(char* out){
    /*
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

    uint8_t hour = rtc.t.hour+GMT;

    out[10] = 'T';
    out[11] = '0'+hour/10;
    out[12] = '0'+hour%10;
    out[13] = ':';
    out[14] = '0'+rtc.t.min/10;
    out[15] = '0'+rtc.t.min%10;
    out[16] = ':';
    out[17] = '0'+rtc.t.sec/10;
    out[18] = '0'+rtc.t.sec%10;
    out[19] = ' ';
    */
    out[0] = '1';
    out[1] = '7';
    out[2] = '.';
    out[3] = '0';
    out[4] = '7';
    out[5] = '.';
    out[6] = '2';
    out[7] = '0';
    out[8] = '1';
    out[9] = '8';

    out[10] = 'T';
    out[11] = '0';
    out[12] = '9';
    out[13] = ':';
    out[14] = '0';
    out[15] = '0';
    out[16] = ':';
    out[17] = '0';
    out[18] = '0';
    out[19] = ' ';

    float tempera   = sens.temperature();
    float humid     = sens.humidity();
    if (tempera < 0.){out[20] = '-'; tempera*=-1.;}
    else{out[20] = '+';}
    out[21] = '0'+(uint8_t)tempera/10;
    out[22] = '0'+(uint8_t)tempera%10;
    out[23] = '.';
    out[24] = '0'+(uint8_t)(tempera*10.)%10;
    out[25] = '0'+(uint8_t)(tempera*100.)%10;
    out[26] = 'C';
    out[27] = '0'+(uint8_t)(humid/10);
    out[28] = '0'+(uint8_t)humid%10;
    out[29] = '.';
    out[30] = '0'+(uint8_t)(humid*10)%10;
    out[31] = '%';

    out[32] = '\r';
    out[33] = '\n';
    out[34] = '\0';
    //if((uint8_t)humid/10>=10.){
        //sens.reset();
    //}
}

ISR(USART_RX_vect){
    uint8_t temp = uart_getc();
    if (temp == 't'){
        flag_reg |= (1<<CLOCK_TICK_BIT);
    }
    else if(temp == 'p'){
        uart_putc(temp+1);
    }
}

ISR(INT0_vect){
    flag_reg |= (1<<CLOCK_TICK_BIT);
}

int main(void){
    flag_reg = 0;
    ACSR = (1<<ACD);
    uart_init();
    //rtc.deactivate_sqm();
    EICRA |= (1<<ISC01);
    EIMSK |= (1<<INT0);
    sei();

    while(true){
        if(flag_reg&(1<<CLOCK_TICK_BIT)){
            sens.gettemphum();
            //rtc.get();
            char outpp[40];
            output_time(outpp);
            uart_puts(outpp);
            flag_reg&=~(1<<CLOCK_TICK_BIT);
        }
        else{
            _delay_us(1);
        }

    }
    return 0;
}
