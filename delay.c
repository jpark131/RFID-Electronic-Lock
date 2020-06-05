#include "msp.h"

#define SETUP_TIME 56
#define LOOP_TIME 7

void delay_us(uint16_t us)
{
    int i;
    int k;
    int n;
    uint32_t freq = CS->CTL0;

    if(freq == 0x0){ //if 1.5MHz
        k = 1;
    }
    if(freq == 0x10000){ //if 3MHz
        k = 2;
    }
    if(freq == 0x20000){ //if 6MHz
        k = 4;
    }
    if(freq == 0x30000){ //if 12MHz
        k = 8;
    }
    if(freq == 0x40000){ //if 24 MHz
        k = 16;
    }

    n = (k*(us-(SETUP_TIME/k)))/LOOP_TIME; //formula for number of iterations
    for (i = 0; i <= n; i++)
    {

    }
}

void delay_ms(uint16_t ms)
{
    int i;
    int k;
    int n;
    int us = ms*1000;
    uint32_t freq = CS->CTL0;

    if(freq == 0x0){ //if 1.5MHz
        k = 1;
    }
    if(freq == 0x10000){ //if 3MHz
        k = 2;
    }
    if(freq == 0x20000){ //if 6MHz
        k = 4;
    }
    if(freq == 0x30000){ //if 12MHz
        k = 8;
    }
    if(freq == 0x40000){ //if 24 MHz
        k = 16;
    }

    n = (k*(us-(SETUP_TIME/k)))/LOOP_TIME; //formula for number of iterations
    for (i = 0; i <= n; i++)
    {

    }
}

void delay_s(uint16_t s)
{
    int i;
    int k;
    int n;
    int us = s*1000000;
    uint32_t freq = CS->CTL0;

    if(freq == 0x0){ //if 1.5MHz
        k = 1;
    }
    if(freq == 0x10000){ //if 3MHz
        k = 2;
    }
    if(freq == 0x20000){ //if 6MHz
        k = 4;
    }
    if(freq == 0x30000){ //if 12MHz
        k = 8;
    }
    if(freq == 0x40000){ //if 24 MHz
        k = 16;
    }

    n = (k*(us-(SETUP_TIME/k)))/LOOP_TIME; //formula for number of iterations
    for (i = 0; i <= n; i++)
    {

    }
}
