
/*
------------------------------------------------------------------------------
Headers:
------------------------------------------------------------------------------
*/
#include <msp430.h>
#include "driverlib.h"
#include "User_SysInit.h"
#include "User_Config.h"
#include "User_ADC.h"

#if MODULE_ADC_ENABLE
/*
------------------------------------------------------------------------------
Marcos
------------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
Local Variable
------------------------------------------------------------------------------
*/

#if INT_MODE
static uint16_t __ADC12_A_RetBuf[16] = {0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0};
static uint8_t __ADC12_A_StartCh = ADC_12_A_StartCH;
static uint8_t __ADC12_A_EndCh = ADC_12_A_EndCH;
#endif


/*
------------------------------------------------------------------------------
Local Functions
------------------------------------------------------------------------------
*/
static void __ADC12_A_ConfigureMemory(uint8_t StartCH, uint8_t EndCH)
{
    uint8_t cnt = 0;

    ADC12_A_configureMemoryParam param3 = {0};
    param3.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param3.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param3.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;

    for (cnt = StartCH; cnt < EndCH; cnt++){
        param3.memoryBufferControlIndex = cnt;
        param3.inputSourceSelect = cnt;
        ADC12_A_configureMemory(ADC12_A_BASE,&param3);
    }

    param3.memoryBufferControlIndex = cnt;
    param3.inputSourceSelect = cnt;
    param3.endOfSequence = ADC12_A_ENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param3);
}

static inline void __ADC_12_A_GetRet(void)
{
    uint8_t cnt = 0;
    for (cnt = __ADC12_A_StartCh; cnt <= __ADC12_A_EndCh; cnt++)
        __ADC12_A_RetBuf[cnt] = ADC12_A_getResults(ADC12_A_BASE, cnt);

    return;
}

/*
------------------------------------------------------------------------------
Module's Functions
------------------------------------------------------------------------------
*/
void User_ADC12_A_StartConv(void)
{
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    ADC12_A_startConversion ( ADC12_A_BASE, ADC_12_A_StartCH,
                                ADC12_A_REPEATED_SEQOFCHANNELS);
#elif   ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_ONCE
    ADC12_A_startConversion ( ADC12_A_BASE, ADC12_A_CH,
                                ADC12_A_REPEATED SINGLECHANNE);
#endif
    return;
}

void User_ADC12_A_GetRet(uint16_t* Buf, uint8_t StartCh, uint8_t EndCh)
{
    uint8_t cnt = 0;
    for (cnt = StartCh; cnt <= EndCh; cnt++){
#if INT_MODE
        Buf[cnt] = __ADC12_A_RetBuf[cnt];
#else
       while(ADC12_A_isBusy(ADC12_A_BASE) == ADC12 A NOTBUSY);
       Buf[cnt] = ADC12_A_getResults(ADC12_A_BASE, cnt);
#endif
    }

    return;
}

void User_ADC12_A_Init(void)
{
#if ((!defined(User_ADC12_A_Input_Port)) || (!defined(User_ADC12_A_Input_Pins)))
#error "User_ADC.c : Please define the input port and input pins for ADC12_A.\
    You can define those two macros User_ADC12_A_Input_Pins and \
        User_ADC12_A_Input_Port in User_Config.h"
#endif
    GPIO_setAsPeripheralModuleFunctionInputPin(User_ADC12_A_Input_Port,
                                                      User_ADC12_A_Input_Pins);

#if ADC12_A_CLOCKSOURCE == ADC12_A_CLOCKSOURCE_ADC12OSC
    ADC12_A_init(ADC12_A_BASE, User_ADC12_A_SAMPLEHOLDSOURCE,
                ADC12_A_CLOCKSOURCE, User_ADC12_A_SampleRate);
#else
    ADC12_A_init(ADC12_A_BASE, User_ADC12_A_SAMPLEHOLDSOURCE,
                ADC12_A_CLOCKSOURCE, ADC12_A_CLOCKDIVIDER);
#endif
     /*使能ADC内核*/
     ADC12_A_enable(ADC12_A_BASE);

#if !defined(ADC12_A_SAMPLE_MODE)
#error "Please define the ADC12_A_SAMPLE_MODE in User_Config.h"
#endif
    /*设置采样保持时间和采样模式*/
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
                               ADC12_A_MEM_0_7_CYCLEHOLD,
                               ADC12_A_MEM_8_16_CYCLEHOLD,
                               ADC12_A_SAMPLE_MODE);

    /*设置采样序列中的通道*/
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    __ADC12_A_ConfigureMemory(ADC_12_A_StartCH, ADC_12_A_EndCH);
#elif ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_ONCE
    ADC12_A_configureMemoryParam param2 = {0};
    param2.memoryBufferControlIndex = ADC12_A_CH;
    param2.inputSourceSelect = ADC12_A_CH;
    param2.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param2.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param2.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE,&param2);
#endif

#if INT_MODE
    User_ADC12_A_InterruptStart();
    __bis_SR_register(GIE);
#endif
    /*开始转换*/
   User_ADC12_A_StartConv();
   return;
}



/*
------------------------------------------------------------------------------
Demo Main
------------------------------------------------------------------------------
*/
#if MODULE_ADC_DEBUG
static uint16_t ADC_RetBuf[4];
void main(void)
{
    WatchDogStop();
    Clk_MCLK_24M_SMCLK_3M_ACLK_32768Hz();
    User_ADC12_A_Init();
    while(1) User_ADC12_A_GetRet(ADC_RetBuf,ADC_12_A_StartCH,ADC_12_A_EndCH);
}
#endif

/*
------------------------------------------------------------------------------
                                    ISR
------------------------------------------------------------------------------
*/
#if INT_MODE == 1
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR(void)
{
    switch(__even_in_range(ADC12IV,34))
    {
    case  0: break;       //Vector  0:  No interrupt
    case  2: break;       //Vector  2:  ADC overflow
    case  4: break;       //Vector  4:  ADC timing overflow
    case  6: break;       //Vector  6:  ADC12IFG0
    case  8: break;       //Vector  8:  ADC12IFG1
    case 10: break;       //Vector 10:  ADC12IFG2
    case 12:              //Vector 12:  ADC12IFG3
            __ADC_12_A_GetRet();
#if ADC_EXT_LPMODE != -1
            __bic_SR_register_on_exit(ADC_EXT_LPMODE);
#endif
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

#endif
#endif