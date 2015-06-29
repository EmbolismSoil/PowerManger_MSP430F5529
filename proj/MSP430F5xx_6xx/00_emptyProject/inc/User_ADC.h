#ifndef __USER_ADC_H__
#define __USER_ADC_H__

#include "driverlib.h"
#include "User_Config.h"
/*开始转换*/
extern void User_ADC12_A_StartConv(void);
/*ADC12_A模块初始化函数，使用模块前必须调用该函数初始化*/
extern void User_ADC12_A_Init(void);
/*获取转换结果，如果为单次转换，令StartCh = EndCh即可*/
extern void User_ADC12_A_GetRet(uint16_t* Buf, uint8_t StartCh, uint8_t EndCh);

/*立即停止转换*/
#define User_ADC12_A_StopConv() \
ADC12_A_disableConversions(ADC12_A_BASE, ADC12_A_PREEMPTCONVERSION );

inline void User_ADC12_A_InterrupStop(void)
{
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    ADC12_A_clearInterrupt(ADC12_A_BASE, 1 << ADC_12_A_EndCH);
    ADC12_A_disableInterrupt(ADC12_A_BASE, 1 << ADC_12_A_EndCH);
#elif ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_ONCE
    ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_CH);
   ADC12_A_disableInterrupt(ADC12_A_BASE, ADC12_A_CH);
#endif
}

inline void User_ADC12_A_InterruptStart(void)
{
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    ADC12_A_clearInterrupt(ADC12_A_BASE, 1 << ADC_12_A_EndCH);
    ADC12_A_enableInterrupt(ADC12_A_BASE, 1 << ADC_12_A_EndCH);
#elif ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_ONCE
    ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_CH);
    ADC12_A_enableInterrupt(ADC12_A_BASE, ADC12_A_CH);
#endif
}
#endif
