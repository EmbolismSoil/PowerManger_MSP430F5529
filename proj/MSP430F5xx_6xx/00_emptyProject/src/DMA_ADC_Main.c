#include <msp430.h>
#include "driverlib.h"
#include "User_SysInit.h"

volatile uint16_t DMA_DST[24];
volatile uint16_t *pDST;

void DMAInit(void);
void  ADCInit(void);

void main(void)
{
    WatchDogStop();
    PowerLevel_3();
    Clk_MCLK_24M_SMCLK_3M_ACLK_32768Hz();

    pDST = DMA_DST;

    ADCInit();
    DMAInit();
    __bis_SR_register(GIE);
    while(1);
}

void ADCInit(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
                                               GPIO_PIN3
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
    param2.endOfSequence = ADC12_A_ENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param2);

    ADC12_A_startConversion(ADC12_A_BASE,
                            ADC12_A_MEMORY_0,
                            ADC12_A_REPEATED_SEQOFCHANNELS);
}

void DMAInit(void)
{
    DMA_disableTransferDuringReadModifyWrite();

    DMA_initParam initParam = {0};
    initParam.channelSelect = DMA_CHANNEL_0;
    initParam.transferModeSelect = DMA_TRANSFER_REPEATED_BLOCK;
    initParam.transferSize = 3;
    initParam.triggerSourceSelect = DMA_TRIGGERSOURCE_24;
    initParam.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    initParam.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;
    DMA_init(&initParam);

    DMA_setSrcAddress(DMA_CHANNEL_0,
                      ADC12_A_getMemoryAddressForDMA(ADC12_A_BASE,
                                                     ADC12_A_MEMORY_0),
                      DMA_DIRECTION_INCREMENT);

    DMA_setDstAddress(DMA_CHANNEL_0,
                      (uint32_t)&DMA_DST,
                      DMA_DIRECTION_INCREMENT);

    //Enable DMA channel 0 interrupt
    DMA_clearInterrupt(DMA_CHANNEL_0);
    DMA_enableInterrupt(DMA_CHANNEL_0);

    //Enable transfers on DMA channel 0
    DMA_enableTransfers(DMA_CHANNEL_0);
}


#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
  switch(__even_in_range(DMAIV,16))
  {
    case 0: break;
    case 2:                                 // DMA0IFG = DMA Channel 0
     __data16_write_addr((unsigned short) &DMA0DA,(unsigned long)(pDST += 3));
     if(pDST - DMA_DST == 24)
         pDST = DMA_DST;
      break;
    case 4: break;                          // DMA1IFG = DMA Channel 1
    case 6: break;                          // DMA2IFG = DMA Channel 2
    case 8: break;                          // DMA3IFG = DMA Channel 3
    case 10: break;                         // DMA4IFG = DMA Channel 4
    case 12: break;                         // DMA5IFG = DMA Channel 5
    case 14: break;                         // DMA6IFG = DMA Channel 6
    case 16: break;                         // DMA7IFG = DMA Channel 7
    default: break;
  }
}
