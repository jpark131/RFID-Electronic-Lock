#ifndef LCD_H_
#define LCD_H_

/*
 * LCD.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Jake
 */

void write_com(uint8_t com);
void init_LCD();
void clear_LCD();
void home_LCD();
void write_char_LCD(char chr);
void write_string_LCD(char* str);
void next_line_LCD();

#endif /* LCD_H_ */
