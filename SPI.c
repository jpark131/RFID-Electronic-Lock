/*
 * SPI.c
 *
 *  Created on: Jun 1, 2020
 *      Author: Jake
 */

#include "msp.h"

void init_ports(void){  //init ports used by reader
    P3->SEL0 &= ~BIT3;
    P3->SEL1 &= ~BIT3;
    P3->DIR |= BIT3;        //P3.3 GPIO output (/CS)
    P3->OUT |= BIT3;        //Start /CS high

    P1->SEL0 |= BIT5;
    P1->SEL1 &= ~BIT5;      //P1.5 UCB0CLK (SCLK)

    P1->SEL0 |= BIT6;
    P1->SEL1 &= ~BIT6;      //P1.6 UCB0SIMO (MOSI)

    P1->SEL0 |= BIT7;
    P1->SEL1 &= ~BIT7;      //P1.7 UCB0SOMI (MISO)
}

void prepare_spi(void){ //initialize SPI settings
    init_ports();
    EUSCI_B0_SPI->CTLW0 |= BIT0;    //Set reset
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_MST;    //Master mode
    EUSCI_B0_SPI->CTLW0 |= BIT8;     //Synchronous
    EUSCI_B0_SPI->CTLW0 |= BIT7;     //SCLK select
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_MSB;    //MSB first
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_CKPH;    //set clock phase
    EUSCI_B0_SPI->CTLW0 &= ~BIT0;    //Disable reset
    EUSCI_B0_SPI->IE |= EUSCI_B_IE_TXIE | EUSCI_B_IE_RXIE; //enable interrupts
}

void transmit_byte(uint8_t data){ //Transmit one byte
    EUSCI_B0_SPI->TXBUF = data;
    //data
    while(!(EUSCI_B0_SPI->IFG & BIT1)); //wait for transmit to finish
}

uint8_t receive_byte(void){ //recieve one byte
    uint8_t data;
    while(!(EUSCI_B0_SPI->IFG & EUSCI_B_IFG_RXIFG0)); //wait for byte received
    data = EUSCI_B0_SPI->RXBUF; //save data
    return data;
}

uint8_t SPI_transfer(uint8_t data)
{
    uint32_t rxData;

    transmit_byte(data); //transmit one byte on MOSI
    rxData = receive_byte(); //receive one byte on MISO

    return (uint8_t) rxData;
}
