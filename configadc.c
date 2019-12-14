/*
 * configadc.c
 *
 *  Created on: Dec 4, 2019
 *      Author: evanw
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,            // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // ACLK/1 = 32Khz
        1000,                                //1000,
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
        1000                                       // 1000 Period
};

void configadc(){
        P1->DIR |=0x01;     //config P1.0 as an output
        P2->DIR |=BIT2;
        P2->DIR |=BIT1;

        /* Setting up clocks
         * MCLK = MCLK = 3MHz
         * ACLK = REFO = 32Khz */
        MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

        /* Initializing ADC (MCLK/1/1) */
        MAP_ADC14_enableModule();
        MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                0);

        /* Setting reference voltage to 2.5*/
        MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
        MAP_REF_A_enableReferenceVoltage();

        /* Configuring GPIOs () */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4,
        GPIO_TERTIARY_MODULE_FUNCTION);
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN7|GPIO_PIN6,
            GPIO_TERTIARY_MODULE_FUNCTION);

        /* Configuring ADC Memory */
        MAP_ADC14_configureMultiSequenceMode(ADC_MEM0,ADC_MEM2, true);


        MAP_ADC14_configureConversionMemory(ADC_MEM0,
                    ADC_VREFPOS_INTBUF_VREFNEG_VSS,
                    ADC_INPUT_A7, false);
            MAP_ADC14_configureConversionMemory(ADC_MEM1,
                    ADC_VREFPOS_INTBUF_VREFNEG_VSS,
                    ADC_INPUT_A1, false);
            MAP_ADC14_configureConversionMemory(ADC_MEM2,
                    ADC_VREFPOS_INTBUF_VREFNEG_VSS,
                    ADC_INPUT_A6, false);



        /* Configuring Timer_A in continuous mode and sourced from ACLK */
        MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);

        /* Configuring Timer_A0 in CCR1 to trigger at 1000 */
        MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

        /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
         * to automatic iteration after it is triggered*/
        MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

        /* Enabling the interrupt when a conversion on channel 1 is complete and
         * enabling conversions */
        MAP_ADC14_enableInterrupt(ADC_INT2);
        MAP_ADC14_enableConversion();

        /* Enabling Interrupts */
        MAP_Interrupt_enableInterrupt(INT_ADC14);
        MAP_Interrupt_setPriority(1,4);
        MAP_Interrupt_enableMaster();

        /* Starting the Timer */
        MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}
