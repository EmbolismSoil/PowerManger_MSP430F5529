#ifndef __USER_ADC_H__
#define __USER_ADC_H__

#include "driverlib.h"
#include "User_Config.h"
/*��ʼת��*/
extern void User_ADC12_A_StartConv(void);
/*ADC12_Aģ���ʼ��������ʹ��ģ��ǰ������øú�����ʼ��*/
extern void User_ADC12_A_Init(void);
/*��ȡת����������Ϊ����ת������StartCh = EndCh����*/
extern void User_ADC12_A_GetRet(uint16_t* Buf, uint8_t StartCh, uint8_t EndCh);

/*����ֹͣת��*/
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
