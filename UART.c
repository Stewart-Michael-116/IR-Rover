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
    EUSCI_A2 -> CTLW0 |= EUSCI_A_CTLW0_SWRST;           //sets UCSWRST to 1
    EUSCI_A2-> CTLW0 = (EUSCI_A_CTLW0_MODE_0     |     // select UART mode
                        EUSCI_A_CTLW0_MST        |     // select master mode
                        EUSCI_A_CTLW0_SSEL__SMCLK     // select small clock
                        );

    //motor driver config
    EUSCI_A2->CTLW0 &= (~EUSCI_A_CTLW0_PEN |      //parity disabled
                        ~EUSCI_B_CTLW0_SEVENBIT|  //7 bit disabled in order to communicate with 8 bit data
                        ~EUSCI_A_CTLW0_MSB); //LSB first

    //baud rate
    EUSCI_A2->BRW = 78;
    EUSCI_A2->MCTLW = (2<<EUSCI_A_MCTLW_BRF_OFS) |EUSCI_A_MCTLW_OS16;

    //clock stuff
    CS->KEY = 0x695A;
    CS->CTL0 = ((CS->CTL0 &~ CS_CTL0_DCORSEL_MASK) | 12);
    BITBAND_PERI(CS->KEY, CS_KEY_KEY_OFS) =1;

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST); // clears UCSWRST
}

void send(uint8_t go){
    while (!(EUSCI_A2->IFG & BIT1));
    EUSCI_A2-> TXBUF |= go;
}


// NVIC_EnableIRQ(EUSCIA2_IRQn); //enable IQR

/*
void EUSCIA2_IRQHandler(void)
{
    EUSCI_A2-> TXBUF |= *sending;

    EUSCI_A2->IFG &= ~EUSCI_A_IFG_TXCPTIFG;
}
*/


/*void motor1(){
    int i;
    EUSCI_A2-> TXBUF |= 'M';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '1';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= 'F';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '3';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '4';
}
void motor2(){
    int i;
    EUSCI_A2-> TXBUF |= 'M';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '2';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= 'F';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '3';
    for(i=0; i<3000; i++);
    EUSCI_A2-> TXBUF |= '4';
}
*/


// __enable_interrupt();
//interrupt flag
//EUSCI_A2->IFG = 0;    // Clear eUSCI TX interrupt flag
//EUSCI_A2->IE |= EUSCI_A_IE_TXIE;        // Enable USCI_A2 TX interrupt
