#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

/*
------------------------------------------------------------
MODULE_XXX_ENABLE :
如果使用该模块时则定义宏 #define MODULE_XXX_ENABLE MODULE_ENABLE
如果不使用模块则定义宏 #define MODULE_XXX_ENBALE   MODULE_DISABLE
例如使用ADC模块时：
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
                调试如果需要调试某模块时定义该宏为 1
                并且定义 MODULES_DEBUG 为 1
例如：
                调试TIMER模块时定义 #define MODULE_TIMER_DEBUG 1
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
——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              ADC 模块配置
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
*/

/*
----------------------------------------------------------------------------------
    使用ADC模块，如果想要使用ADC模块则必须定义该宏为 1 ，如果不使用ADC模块，则
    #define MODULE_ADC_DEBUG 0
----------------------------------------------------------------------------------
*/

#if defined(MODULE_ADC_DEBUG)
    #undef  MODULE_ADC_ENABLE
    #define MODULE_ADC_ENABLE MODULE_ENABLE
#endif

/*输入管脚配置：User_ADC12_A_Input_Pins 可定义如 GPIO_PIN0 + GPIO_PIN1 ....*/
#define User_ADC12_A_Input_Port    GPIO_PORT_P6
#define User_ADC12_A_Input_Pins    GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3

/*ADC12_A 采样保持时钟源*/
#define User_ADC12_A_SAMPLEHOLDSOURCE  ADC12_A_SAMPLEHOLDSOURCE_SC
/*ADC12_A 时钟源, CLKSRC可以是：ADC12OSC ACLK MCLK SMCKL*/
#define ADC12_A_CLOCKSOURCE  ADC12_A_CLOCKSOURCE_ADC12OSC

/*ADC12 采样率 或者分频率*/
#if ADC12_A_CLOCKSOURCE == ADC12_A_CLOCKSOURCE_ADC12OSC
    #define User_ADC12_A_SampleRate_5M    ADC12_A_CLOCKDIVIDER_1
    #define User_ADC12_A_SampleRate_2M5   ADC12_A_CLOCKDIVIDER_2
    #define User_ADC12_A_SampleRate_1M25  ADC12_A_CLOCKDIVIDER_4
    #define User_ADC12_A_SampleRate_1M    ADC12_A_CLOCKDIVIDER_5
    #define User_ADC12_A_SampleRate_M628  ADC12_A_CLOCKDIVIDER_8
    #define User_ADC12_A_SampleRate_M25   ADC12_A_CLOCKDIVIDER_20
    #define User_ADC12_A_SampleRate_M16   ADC12_A_CLOCKDIVIDER_32

    /*ADC12_A 的采样率*/
    #define User_ADC12_A_SampleRate User_ADC12_A_SampleRate_5M

#else
    /*ADC12_A 分频系数*/
    #define ADC12_A_CLOCKDIVIDER ADC12_A_CLOCKDIVIDER_1
#endif

/*
----------------------------------------------------------------------
ADC12_A中有16个通道和16个结果寄存器ADC12MEMx ，每个结果寄存器都可以
配置到一个采样通道当中去，并且每个结果寄存器都可以设置采样保持时间。
采样保持时间可以设置为：
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

/*设置采样模式，单次采样还是多次序列采样模式*/
#define ADC12_A_SAMPLE_ONCE      ADC12_A_MULTIPLESAMPLESDISABLE
#define ADC12_A_SAMPLE_SEQUENCE  ADC12_A_MULTIPLESAMPLESENABLE
#define ADC12_A_SAMPLE_MODE      ADC12_A_SAMPLE_SEQUENCE

/*设置采样序列中的采样通道*/
#if ADC12_A_SAMPLE_MODE == ADC12_A_SAMPLE_SEQUENCE
    #define   ADC_12_A_StartCH  0
    #define   ADC_12_A_EndCH  3
#elif
    #define ADC12_A_CH 0
#endif

/*设置是否使用中断模式*/
#define INT_MODE  1
/*中断中需要退出的低功耗模式,-1表示不需要退出低功耗模式*/
#define ADC_EXT_LPMODE LPM0_bits

/*
——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              I2C 模块配置
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
*/

/*中断中需要退出的低功耗模式,-1表示不需要退出低功耗模式*/
#define I2C_EXT_LPMODE LPM0_bits
#define I2C_MAX_BUF_SIZES 512

/*配配置目标主机和源主机的地址*/
#define USER_CONFIG_DEST_ADDRESS  0x48
#define USER_CONFIG_SRC_ADDRESS 0x38

/*使用的I2C模块基地址*/
#define USER_CONFIG_USCI_Bx_Base USCI_B0_BASE

/*设置为主机模式*/
#define USER_I2C_MASTER 1


/*
——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                              定时器 模块配置
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
*/
#define TIMER_ALARM_EXT_LPMODE LPM0_bits

#endif