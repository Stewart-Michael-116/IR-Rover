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
