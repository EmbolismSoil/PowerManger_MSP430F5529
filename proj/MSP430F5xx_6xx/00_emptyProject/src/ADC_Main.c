/*
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
Headers:
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
*/
#include <msp430.h>
#include "driverlib.h"
#include "User_SysInit.h"
#include "User_Config.h"



#define   Num_of_Results   8

volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A2results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];


#if MODULES_DEBUG == 0

void main(void)
{
    //Stop Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

       PowerLevel_3();
   Clk_MCLK_24M_SMCLK_3M_ACLK_32768Hz();
    //Enable A/D channel inputs
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
                                               GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
                                               GPIO_PIN6 + GPIO_PIN7
                                               );

    //Initialize the ADC12_A Module
    /*
     * Base address of ADC12_A Module
     * Use internal ADC12_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC12_A_init(ADC12_A_BASE,
                 ADC12_A_SAMPLEHOLDSOURCE_SC,
                 ADC12_A_CLOCKSOURCE_ADC12OSC,
                 ADC12_A_CLOCKDIVIDER_1
                 );

    ADC12_A_enable(ADC12_A_BASE);

    /*
     * Base address of ADC12_A Module
     * For memory buffers 0-7 sample/hold for 256 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Enable Multiple Sampling
     */
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
                               ADC12_A_CYCLEHOLD_256_CYCLES,
                               ADC12_A_CYCLEHOLD_4_CYCLES,
                               ADC12_A_MULTIPLESAMPLESENABLE);

    //Configure Memory Buffers
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 0
     * Map input A0 to memory buffer 0
     * Vref+ = AVcc
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    ADC12_A_configureMemoryParam param0 = {0};
    param0.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    param0.inputSourceSelect = ADC12_A_INPUT_A0;
    param0.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param0.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param0.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param0);

    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 1
     * Map input A1 to memory buffer 1
     * Vref+ = AVcc
     * Vref- = AVss
     * Memory buffer 1 is not the end of a sequence
     *
     */
    ADC12_A_configureMemoryParam param1 = {0};
    param1.memoryBufferControlIndex = ADC12_A_MEMORY_1;
    param1.inputSourceSelect = ADC12_A_INPUT_A1;
    param1.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param1.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param1.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param1);
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 2
     * Map input A2 to memory buffer 2
     * Vref+ = AVcc
     * Vref- = AVss
     * Memory buffer 2 is not the end of a sequence
     */
    ADC12_A_configureMemoryParam param2 = {0};
    param2.memoryBufferControlIndex = ADC12_A_MEMORY_2;
    param2.inputSourceSelect = ADC12_A_INPUT_A2;
    param2.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param2.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param2.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param2);
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 3
     * Map input A3 to memory buffer 3
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 3 IS the end of a sequence
     */
    ADC12_A_configureMemoryParam param3 = {0};
    param3.memoryBufferControlIndex = ADC12_A_MEMORY_3;
    param3.inputSourceSelect = ADC12_A_INPUT_A3;
    param3.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param3.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param3.endOfSequence = ADC12_A_ENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param3);

    //Enable memory buffer 3 interrupt
    ADC12_A_clearInterrupt(ADC12_A_BASE,
                           ADC12IFG3);
    ADC12_A_enableInterrupt(ADC12_A_BASE,
                            ADC12IE3);

    //Enable/Start first sampling and conversion cycle
    /*
     * Base address of ADC12_A Module
     * Start the conversion into memory buffer 0
     * Use the repeated sequence of channels
     */
    ADC12_A_startConversion(ADC12_A_BASE,
                            ADC12_A_MEMORY_0,
                            ADC12_A_REPEATED_SEQOFCHANNELS);

    //Enter LPM0, Enable interrupts
    __bis_SR_register(LPM0_bits + GIE);
    //For debugger
    __no_operation();
}
#endif

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR(void)
{
    static uint16_t index = 0;

    switch(__even_in_range(ADC12IV,34))
    {
    case  0: break;       //Vector  0:  No interrupt
    case  2: break;       //Vector  2:  ADC overflow
    case  4: break;       //Vector  4:  ADC timing overflow
    case  6: break;       //Vector  6:  ADC12IFG0
    case  8: break;       //Vector  8:  ADC12IFG1
    case 10: break;       //Vector 10:  ADC12IFG2
    case 12:              //Vector 12:  ADC12IFG3
        //Move A0 results, IFG is cleared
        A0results[index] =
            ADC12_A_getResults(ADC12_A_BASE,
                               ADC12_A_MEMORY_0);
        //Move A1 results, IFG is cleared
        A1results[index] =
            ADC12_A_getResults(ADC12_A_BASE,
                               ADC12_A_MEMORY_1);
        //Move A2 results, IFG is cleared
        A2results[index] =
            ADC12_A_getResults(ADC12_A_BASE,
                               ADC12_A_MEMORY_2);
        //Move A3 results, IFG is cleared
        A3results[index] =
            ADC12_A_getResults(ADC12_A_BASE,
                               ADC12_A_MEMORY_3);

        //Increment results index, modulo; Set BREAKPOINT here
        index++;

        if(index == 8)
        {
            (index = 0);
        }
    case 14: break;       //Vector 14:  ADC12IFG4
    case 16: break;       //Vector 16:  ADC12IFG5
    case 18: break;       //Vector 18:  ADC12IFG6
    case 20: break;       //Vector 20:  ADC12IFG7
    case 22: break;       //Vector 22:  ADC12IFG8
    case 24: break;       //Vector 24:  ADC12IFG9
    case 26: break;       //Vector 26:  ADC12IFG10
    case 28: break;       //Vector 28:  ADC12IFG11
    case 30: break;       //Vector 30:  ADC12IFG12
    case 32: break;       //Vector 32:  ADC12IFG13
    case 34: break;       //Vector 34:  ADC12IFG14
    default: break;
    }
}

