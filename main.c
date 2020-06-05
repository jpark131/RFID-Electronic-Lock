#include "MFRC552.h"
#include "msp.h"
#include "delay.h"
#include "P3.h"
#include "LCD.h"
#inlcude "SPI.h"

void init_leds(void){
    //Green LED starts off
    P2->SEL0 &= ~BIT1;
    P2->SEL1 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;

    //Red LED starts on
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;
}

int main(void)
{
    init_leds();
    prepare_spi();
    MFRC552_Init();
    init_LCD();
    write_string_LCD("LOCKED");
    while(1)
    {
        determine_RFID_state();
    }
}

