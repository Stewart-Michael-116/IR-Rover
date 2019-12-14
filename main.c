/*
 * configadc.h
 *
 *  Created on: Dec 4, 2019
 *      Author: evanw
 */
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include"configadc.h"
#include ”UART.h”

/* Statics */
static uint16_t resultsBuffer[8];
volatile int IRforward_flag=0;
volatile int IRright_flag=0;
volatile int newresults=0;
static uint16_t resultsBuffer[8];
int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_enableSleepOnIsrExit();

    configadc(); // configures multi channel repeat
    config_UART(); // configures uart
    P3->DIR |= BIT7|BIT5;  // turns on lights to indicate what the adcs are reading
    P2->DIR |= BIT7|BIT6;
    uint8_t i=0;
    while (1)
    {
        if(newresults){
            if(~IRright_flag){
                rightturn();/*turn 90 degrees right*/
for(i=0;i<10000;i++); 
			stop();
            }
            else if(~IRforward_flag){
                straight(); /*go straight*/ //turn brake lights off
for(i=0;i<10000;i++); 
			stop();
            }
            else{
                leftturn(); /*turn 90 degrees left*/ 
for(i=0;i<10000;i++); 
			stop();
            }
            newresults=0;
        }
    }
}


/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM2 the last one to get converted */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);


    if(status & ADC_INT2)
        {
            MAP_ADC14_getMultiSequenceResult(resultsBuffer);
        }
    
    /*throws flags depending on what the ADC reads*/
    if(ADC14_getResult(ADC_MEM0)>0x2000){
        IRforward_flag=1;
        P2->OUT |=BIT2;
    }
    else{
        IRforward_flag=0;
        P2->OUT &=~BIT2;
    }

    if(ADC14_getResult(ADC_MEM1)>0x2000){
        IRright_flag=1;
        P1->OUT |=BIT0;
        P2->OUT |= BIT7;
        P2->OUT |=BIT6;
    }
    else{
        IRright_flag=0;
        P1->OUT &=~BIT0;
        P2->OUT &=~BIT7;
        P2->OUT &=~BIT6;
    }

    if(ADC14_getResult(ADC_MEM2)<0x2000){
        P2->OUT |=BIT1;
        P3->OUT |= BIT7;
        P3->OUT |=BIT5;
    }
    else{
        P2->OUT &=~BIT1;
        P3->OUT &= ~BIT7;
        P3->OUT &= ~BIT5;
    }
    newresults=1;
}
