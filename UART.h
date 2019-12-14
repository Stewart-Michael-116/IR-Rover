/*
 * UART.h
 *
 *  Created on: Nov 20, 2019
 *      Author: rosie
 */

#ifndef UART_H_
#define UART_H_

void config_UART(void);

void send(uint8_t go);

void rightturn();

void leftturn();

void straight();

void stop();

#endif /* UART_H_ */
