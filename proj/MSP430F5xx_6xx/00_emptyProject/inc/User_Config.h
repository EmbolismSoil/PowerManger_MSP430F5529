#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

/*
------------------------------------------------------------
MODULE_XXX_ENABLE :
���ʹ�ø�ģ��ʱ����� #define MODULE_XXX_ENABLE MODULE_ENABLE
�����ʹ��ģ������� #define MODULE_XXX_ENBALE   MODULE_DISABLE
����ʹ��ADCģ��ʱ��
#define MODULE_ADC_ENBALE       MODULE_ENABLE
------------------------------------------------------------
*/
#define MODULE_ENABLE   1
#define MODULE_DISABLE  0

#define MODULE_ADC_ENABLE       MODULE_ENABLE
#define MODULE_TIMER_ENABLE     MODULE_ENABLE

/*
------------------------------------------------------------
MODULE_XXX_DEBUG :
                ���������Ҫ����ĳģ��ʱ����ú�Ϊ 1
                ���Ҷ��� MODULES_DEBUG Ϊ 1
���磺
                ����TIMERģ��ʱ���� #define MODULE_TIMER_DEBUG 1
------------------------------------------------------------
*/
#define MODULE_ADC_DEBUG        MODULE_DISABLE
#define MODULE_TIMER_DEBUG      MODULE_DISABLE
#define MODULE_SPI_DEBUG        MODULE_DISABLE
#define MODULE_LCD_DEBUG        MODULE_DISABLE

#if (MODULE_ADC_DEBUG || MODULE_TIMER_DEBUG || MODULE_SPI_DEBUG || MODULE_LCD_DEBUG)
    #define MODULES_DEBUG   MODULE_ENABLE
#else
    #define MODULES_DEBUG   MODULE_DISABLE
#endif

/*
��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              ADC ģ������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
*/

/*
----------------------------------------------------------------------------------
    ʹ��ADCģ�飬�����Ҫʹ��ADCģ������붨��ú�Ϊ 1 �������ʹ��ADCģ�飬��
    #define MODULE_ADC_DEBUG 0
----------------------------------------------------------------------------------
*/

#if defined(MODULE_ADC_DEBUG)
    #undef  MODULE_ADC_ENABLE
    #define MODULE_ADC_ENABLE MODULE_ENABLE
#endif

/*����ܽ����ã�User_ADC12_A_Input_Pins �ɶ����� GPIO_PIN0 + GPIO_PIN1 ....*/
#define User_ADC12_A_Input_Port    GPIO_PORT_P6
#define User_ADC12_A_Input_Pins    GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3

/*ADC12_A ��������ʱ��Դ*/
#define User_ADC12_A_SAMPLEHOLDSOURCE  ADC12_A_SAMPLEHOLDSOURCE_SC
/*ADC12_A ʱ��Դ, CLKSRC�����ǣ�ADC12OSC ACLK MCLK SMCKL*/
#define ADC12_A_CLOCKSOURCE  ADC12_A_CLOCKSOURCE_ADC12OSC

/*ADC12 ������ ���߷�Ƶ��*/
#if ADC12_A_CLOCKSOURCE == ADC12_A_CLOCKSOURCE_ADC12OSC
    #define User_ADC12_A_SampleRate_5M    ADC12_A_CLOCKDIVIDER_1
    #define User_ADC12_A_SampleRate_2M5   ADC12_A_CLOCKDIVIDER_2
    #define User_ADC12_A_SampleRate_1M25  ADC12_A_CLOCKDIVIDER_4
    #define User_ADC12_A_SampleRate_1M    ADC12_A_CLOCKDIVIDER_5
    #define User_ADC12_A_SampleRate_M628  ADC12_A_CLOCKDIVIDER_8
    #define User_ADC12_A_SampleRate_M25   ADC12_A_CLOCKDIVIDER_20
    #define User_ADC12_A_SampleRate_M16   ADC12_A_CLOCKDIVIDER_32

    /*ADC12_A �Ĳ�����*/
    #define User_ADC12_A_SampleRate User_ADC12_A_SampleRate_5M

#else
    /*ADC12_A ��Ƶϵ��*/
    #define ADC12_A_CLOCKDIVIDER ADC12_A_CLOCKDIVIDER_1
#endif

/*
----------------------------------------------------------------------
ADC12_A����16��ͨ����16������Ĵ���ADC12MEMx ��ÿ������Ĵ���������
���õ�һ������ͨ������ȥ������ÿ������Ĵ������������ò�������ʱ�䡣
��������ʱ���������Ϊ��
ADC12_A_CYCLEHOLD_4_CYCLES [Default]
ADC12_A_CYCLEHOLD_8_CYCLES
ADC12_A_CYCLEHOLD_16_CYCLES
ADC12_A_CYCLEHOLD_32_CYCLES
ADC12_A_CYCLEHOLD_64_CYCLES
ADC12_A_CYCLEHOLD_96_CYCLES
ADC12_A_CYCLEHOLD_128_CYCLES
ADC12_A_CYCLEHOLD_192_CYCLES
ADC12_A_CYCLEHOLD_256_CYCLES
ADC12_A_CYCLEHOLD_384_CYCLES
ADC12_A_CYCLEHOLD_512_CYCLES
ADC12_A_CYCLEHOLD_768_CYCLES
ADC12_A_CYCLEHOLD_1024_CYCLES
--------------------------------------------------------------------
*/
#define ADC12_A_MEM_0_7_CYCLEHOLD   ADC12_A_CYCLEHOLD_256_CYCLES
#define ADC12_A_MEM_8_16_CYCLEHOLD  ADC12_A_CYCLEHOLD_256_CYCLES

/*���ò���ģʽ�����β������Ƕ�����в���ģʽ*/
#define ADC12_A_SAMPLE_ONCE      ADC12_A_MULTIPLESAMPLESDISABLE
#define ADC12_A_SAMPLE_SEQUENCE  ADC12_A_MULTIPLESAMPLESENABLE
#define ADC12_A_SAMPLE_MODE      ADC12_A_SAMPLE_SEQUENCE

/*���ò��������еĲ���ͨ��*/
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    #define   ADC_12_A_StartCH  0
    #define   ADC_12_A_EndCH  3
#elif
    #define ADC12_A_CH 0
#endif

/*�����Ƿ�ʹ���ж�ģʽ*/
#define INT_MODE  1
/*�ж�����Ҫ�˳��ĵ͹���ģʽ,-1��ʾ����Ҫ�˳��͹���ģʽ*/
#define ADC_EXT_LPMODE LPM0_bits

/*
��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              I2C ģ������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
*/

/*�ж�����Ҫ�˳��ĵ͹���ģʽ,-1��ʾ����Ҫ�˳��͹���ģʽ*/
#define I2C_EXT_LPMODE LPM0_bits
#define I2C_MAX_BUF_SIZES 512

/*������Ŀ��������Դ�����ĵ�ַ*/
#define USER_CONFIG_DEST_ADDRESS  0x48
#define USER_CONFIG_SRC_ADDRESS 0x38

/*ʹ�õ�I2Cģ�����ַ*/
#define USER_CONFIG_USCI_Bx_Base USCI_B0_BASE

/*����Ϊ����ģʽ*/
#define USER_I2C_MASTER 1


/*
��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              ��ʱ�� ģ������
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
*/
#define TIMER_ALARM_EXT_LPMODE LPM0_bits

#endif