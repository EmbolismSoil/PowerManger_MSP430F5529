#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__
#include "driverlib.h"
#include <MSP430.h>

/*
-----------------------------------------------------------------------------
数据类型定义
-----------------------------------------------------------------------------
*/
typedef void (*User_Timer_CallBack_t)(void);

/*
------------------------------------------------------------------------------
TAxOUTx 引脚映射：
TA0.0 = P1.1
TA0.1 = P1.2
TA0.2 = P1.3
TA0.3 = P1.4
TA0.4 = P1.5

TA1.0 = P1.7
TA1.1 = P2.0
TA1.2 = P2.1

TA2.0 = P2.3
TA2.1 = P2.4
TA2.2 = P2.5
------------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
定时器输出管脚配置函数
GPIO_PORT 可以为 GPIO_PORT_Px 如 GPIO_PORT_P1 GPIO_PORT_P2 GPIO_PORT_P3等等
GPIO_PIN  可以为 GPIO_PINx 如GPIO_PIN0 GPIO_PIN1等等，
并且可以用GPIO_PIN0 + GPIO_PIN1等方式一次配置多个管脚
------------------------------------------------------------------------------
*/
#define User_TimerA_SetAFIO(PORT, PIN) \
GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_##PORT, GPIO_##PIN)

/*
----------------------------------------------------------------------------------------------------------
User_TimerA_OutPutPWM : 如果需要TimerA输出PWM时调用此函数即可
Ax为定时器实例，可以是 A0 A1 A2

TIMER_A_CLOCKSOURCE 选择定时器时钟源 可以为：
                    TIMER_A_CLOCKSOURCE_SMCLK
                    TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK
                    TIMER_A_CLOCKSOURCE_ACLK
                    TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
TIMER_A_CLOCKSOURCE_DIVIDER 是时钟分频系数可以为：
                    TIMER_A_CLOCKSOURCE_DIVIDER_1
                    TIMER_A_CLOCKSOURCE_DIVIDER_2
                    TIMER_A_CLOCKSOURCE_DIVIDER_3
                    TIMER_A_CLOCKSOURCE_DIVIDER_4
                    TIMER_A_CLOCKSOURCE_DIVIDER_5
                    TIMER_A_CLOCKSOURCE_DIVIDER_6
                    TIMER_A_CLOCKSOURCE_DIVIDER_7
                    TIMER_A_CLOCKSOURCE_DIVIDER_8
                    TIMER_A_CLOCKSOURCE_DIVIDER_10
                    TIMER_A_CLOCKSOURCE_DIVIDER_12
                    TIMER_A_CLOCKSOURCE_DIVIDER_14
                    TIMER_A_CLOCKSOURCE_DIVIDER_16
                    TIMER_A_CLOCKSOURCE_DIVIDER_20
                    TIMER_A_CLOCKSOURCE_DIVIDER_24
                    TIMER_A_CLOCKSOURCE_DIVIDER_28
                    TIMER_A_CLOCKSOURCE_DIVIDER_32
                    TIMER_A_CLOCKSOURCE_DIVIDER_40
                    TIMER_A_CLOCKSOURCE_DIVIDER_48
                    TIMER_A_CLOCKSOURCE_DIVIDER_56
                    TIMER_A_CLOCKSOURCE_DIVIDER_64
TIMER_PERIOD 定时器周期，不是时间，而是计数周期。比如 TIMER_PERIOD = 1000时，时间周期 = 1000 * 计数频率
TIMER_A_CAPTURECOMPARE_REGISTER  捕获寄存器 可以为：
                    TIMER_A_CAPTURECOMPARE_REGISTER_0
                    TIMER_A_CAPTURECOMPARE_REGISTER_1
                    TIMER_A_CAPTURECOMPARE_REGISTER_2
                    TIMER_A_CAPTURECOMPARE_REGISTER_3
                    TIMER_A_CAPTURECOMPARE_REGISTER_4
                    TIMER_A_CAPTURECOMPARE_REGISTER_5
                    TIMER_A_CAPTURECOMPARE_REGISTER_6
TIMER_A_OUTPUTMODE  可以是
                    TIMER_A_OUTPUTMODE_OUTBITVALUE
                    TIMER_A_OUTPUTMODE_SET
                    TIMER_A_OUTPUTMODE_TOGGLE_RESET
                    TIMER_A_OUTPUTMODE_SET_RESET
                    TIMER_A_OUTPUTMODE_TOGGLE
                    TIMER_A_OUTPUTMODE_RESET
                    TIMER_A_OUTPUTMODE_TOGGLE_SET
                    TIMER_A_OUTPUTMODE_RESET_SET
----------------------------------------------------------------------------------------------------------
*/
#define User_TimerA_OutPutPWM(Ax,TIMER_A_CLOCKSOURCE, TIMER_A_CLOCKSOURCE_DIVIDER,\
                 TIMER_PERIOD, DUTY_CYCLE, TIMER_A_CAPTURECOMPARE_REGISTER,TIMER_A_OUTPUTMODE) do{\
    Timer_A_outputPWMParam param = {0};\
    param.clockSource = TIMER_A_CLOCKSOURCE;\
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER;\
    param.timerPeriod = TIMER_PERIOD;\
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER;\
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;\
    param.dutyCycle = DUTY_CYCLE;\
        Timer_A_outputPWM(TIMER_##Ax##_BASE, &param);\
}while(0)



#define User_TimerA_SetTAxCCRx(TAx,CCRx,RegVal) TAx##CCRx = (RegVal)

/*
---------------------------------------------------------------------------
停止定时器：
Timer 为定时器实例，如 User_Timer_Stop(A1)
---------------------------------------------------------------------------
*/
#define User_Timer_Stop(Timer) Timer_A_stop (TIMER_##Timer##_BASE)

/*
---------------------------------------------------------------------------
计数器开始计数：
Timer 为定时器实例，
Mode 为计数模式，可以为：
TIMER_A_STOP_MODE
TIMER_A_UP_MODE
TIMER_A_CONTINUOUS_MODE [Default]
TIMER_A_UPDOWN_MODE
---------------------------------------------------------------------------
*/
#define User_Timer_startCounter(Timer,Mode) Timer_A_startCounter(TIMER_##Timer##_BASE, Mode)

/*
---------------------------------------------------------------------------
定时函数：
该函数使用TA0CCR0定时器进行定时，定时 Clk 个SMCLK周期，CallBack为定时器回调函数。
---------------------------------------------------------------------------
*/
extern void User_Timer_SetCallBack(User_Timer_CallBack_t CallBack);
extern User_Timer_CallBack_t UserTimer_GetCallBack(void);

#define User_Timer_StartAlarm(Clk, CallBack) do{\
    User_Timer_SetCallBack(CallBack);\
    TA0CCTL0 = CCIE;\
    TA0CCR0 = Clk;\
    TA0CTL = TASSEL_2 + MC_1 + TACLR;\
}while(0)

/*停止定时器*/
#define User_Timer_StopAlarm() TA0CCR0 = 0
/*重启定时器，Clk为新定时器周期， 这个函数必须在User_Timer_StopAlarm已经
被调用的情况下调用才有效*/
#define User_Timer_ReStartAlarm(Clk) TA0CCR0 = Clk


#endif