/*
 * SPI.h
 *
 *  Created on: Jun 1, 2020
 *      Author: Jake
 */
#ifndef SPI_H_
#define SPI_H_

#include "msp.h"

void init_ports(void);
void prepare_spi(void);
void transmit_byte(uint8_t data);
uint8_t receive_byte(void);
uint8_t SPI_transfer(uint8_t data);

#endif /* SPI_H_ */
