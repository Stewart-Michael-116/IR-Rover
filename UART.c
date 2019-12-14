/*
 * UART.c
 *
 *  Created on: Nov 20, 2019
 *      Author: rosie
 */

#include "msp.h"
#include <stdint.h>
#include "UART.h"

//tx-3.3 yellow
//rx- 3.2 green

void config_UART(void){
    //Configure Ports
       P3->SEL0 |= (BIT2 | BIT3);
       P3->SEL1 &= ~(BIT2 | BIT3);

    // configure UART
    EUSCI_A2 -> CTLW0 |= EUSCI_A_CTLW0_SWRST; //sets UCSWRST to 1
    EUSCI_A2-> CTLW0 = (EUSCI_A_CTLW0_MODE_0 |// select UART mode
                        EUSCI_A_CTLW0_SSEL__SMCLK); // select small clock

    //motor driver config
    EUSCI_A2->CTLW0 &= (~EUSCI_A_CTLW0_PEN |      //parity disabled
                        ~EUSCI_A_CTLW0_SEVENBIT|  //7 bit disabled in order to communicate with 8 bit data
                        ~EUSCI_A_CTLW0_MSB|  //LSB first
                        ~EUSCI_A_CTLW0_SPB
                        );
    EUSCI_A2->CTLW0 &=~EUSCI_A_CTLW0_SPB; //makes 1 bit stop

    //baud rate
    EUSCI_A2->BRW = 78;
    EUSCI_A2->MCTLW = (2<<EUSCI_A_MCTLW_BRF_OFS) |EUSCI_A_MCTLW_OS16;

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST); // clears UCSWRST

//    clock stuff
    CS->KEY = 0x695A;
    CS -> CTL0 =0;
    CS->CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3| CS_CTL1_SELM_3;
    CS ->KEY = 0;


}

void send(uint8_t go){ //sends info to motor driver
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG ));
    EUSCI_A2-> TXBUF = go;
}

void rightturn(){
    send('M');
        send('1');
        send('F');
        send('1');
        send('0');
        send('0');
        send('\r');
        send('\n');
}

void leftturn(){
    send('M');
    send('2');
    send('F');
    send('9');
    send('9');
    send('\r');
    send('\n');
}

void straight(){
    send('M');
    send('1');
    send('F');
    send('1');
    send('0');
    send('0');
    send('\r');
    send('\n');
    send('M');
    send('2');
    send('F');
    send('1');
    send('0');
    send('\r');
    send('\n');
}
 void stop(){
     send('M');
         send('1');
         send('F');
         send('0');
         send('\r');
         send('\n');
         send('M');
         send('2');
         send('F');
         send('0');
         send('\r');
         send('\n');
 }
