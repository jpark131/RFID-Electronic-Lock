#include "msp.h"
#include "LCD.h"
#include "delay.h"
#include <string.h>

/*
 * LCD.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Jake
 */

#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet8bit 0b00111000          // 8-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

void write_com(uint8_t com){ //write a command
    P4->OUT = com; //command
    P5->OUT &= ~BIT4; //clear RS
    P5->OUT &= ~BIT5; //clear RW
    delay_us(1000);
    P6->OUT |= BIT5; //set E
    delay_us(1000);
    P6->OUT &= ~BIT5; //reset E
}

void init_LCD(){ //initialize lcd on powerup
    uint8_t com;
    P4->SEL0 &= ~0xFF; //P4 gpio
    P4->SEL1 &= ~0xFF; //P4 gpio
    P4->DIR |= 0xFF; //P4 output (D0-7)
    P5->SEL0 &= ~(BIT4 | BIT5);
    P5->SEL1 &= ~(BIT4 | BIT5); //P5.4-5 gpio
    P5->DIR |= BIT4; //P5.4-5 output (RS)
    P5->DIR |= BIT5; //P5.4-5 output (RW)
    P6->SEL0 &= ~BIT5;
    P6->SEL1 &= ~BIT5; //P6.5 gpio
    P6->DIR |= BIT5; //P6.5 output (E)

    delay_us(30000);
    com = 0x30;
    write_com(com); //wake up
    //delay_us(1);
    write_com(com); //wake up
    //delay_us(1);
    write_com(com); //wake up
    //delay_us(1);

    com = 0x38;
    write_com(com); //write function
    //delay_us(40);
    com = 0x10;
    write_com(com); //no shift
    //delay_us(40);
    com = 0x01;
    write_com(com); //clear display
    //delay_us(1500);
    com = 0x0F;
    write_com(com); //display on
    //delay_us(40);
    com = 0x06;
    write_com(com); //entry mode
    //delay_us(40);
}

void clear_LCD(){ //clear display
   write_com(0x01);
}

void home_LCD(){ //return cursor home
    write_com(0x02);
    delay_us(1000);
}

void write_char_LCD(char chr){ //write a character
    P4->OUT = chr; //write data
    P5->OUT |= BIT4; //data write mode'
    delay_us(1);
    P6->OUT |= BIT5; //set E
    delay_us(1);
    P6->OUT &= ~BIT5; //reset E
}

void write_string_LCD(char* str){ //write a string
    int len = strlen(str);
    int i;

    for(i=0; i<len; i++){
        write_char_LCD(*str);
        str++;
    }
}

void next_line_LCD(){ //skip to the next line
    write_com(0xC0);
}
