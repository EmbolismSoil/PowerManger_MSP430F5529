#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__
#include "driverlib.h"
#include <MSP430.h>

/*
-----------------------------------------------------------------------------
�������Ͷ���
-----------------------------------------------------------------------------
*/
typedef void (*User_Timer_CallBack_t)(void);

/*
------------------------------------------------------------------------------
TAxOUTx ����ӳ�䣺
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
��ʱ������ܽ����ú���
GPIO_PORT ����Ϊ GPIO_PORT_Px �� GPIO_PORT_P1 GPIO_PORT_P2 GPIO_PORT_P3�ȵ�
GPIO_PIN  ����Ϊ GPIO_PINx ��GPIO_PIN0 GPIO_PIN1�ȵȣ�
���ҿ�����GPIO_PIN0 + GPIO_PIN1�ȷ�ʽһ�����ö���ܽ�
------------------------------------------------------------------------------
*/
#define User_TimerA_SetAFIO(PORT, PIN) \
GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_##PORT, GPIO_##PIN)

/*
----------------------------------------------------------------------------------------------------------
User_TimerA_OutPutPWM : �����ҪTimerA���PWMʱ���ô˺�������
AxΪ��ʱ��ʵ���������� A0 A1 A2

TIMER_A_CLOCKSOURCE ѡ��ʱ��ʱ��Դ ����Ϊ��
                    TIMER_A_CLOCKSOURCE_SMCLK
                    TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK
                    TIMER_A_CLOCKSOURCE_ACLK
                    TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
TIMER_A_CLOCKSOURCE_DIVIDER ��ʱ�ӷ�Ƶϵ������Ϊ��
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
TIMER_PERIOD ��ʱ�����ڣ�����ʱ�䣬���Ǽ������ڡ����� TIMER_PERIOD = 1000ʱ��ʱ������ = 1000 * ����Ƶ��
TIMER_A_CAPTURECOMPARE_REGISTER  ����Ĵ��� ����Ϊ��
                    TIMER_A_CAPTURECOMPARE_REGISTER_0
                    TIMER_A_CAPTURECOMPARE_REGISTER_1
                    TIMER_A_CAPTURECOMPARE_REGISTER_2
                    TIMER_A_CAPTURECOMPARE_REGISTER_3
                    TIMER_A_CAPTURECOMPARE_REGISTER_4
                    TIMER_A_CAPTURECOMPARE_REGISTER_5
                    TIMER_A_CAPTURECOMPARE_REGISTER_6
TIMER_A_OUTPUTMODE  ������
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
ֹͣ��ʱ����
Timer Ϊ��ʱ��ʵ������ User_Timer_Stop(A1)
---------------------------------------------------------------------------
*/
#define User_Timer_Stop(Timer) Timer_A_stop (TIMER_##Timer##_BASE)

/*
---------------------------------------------------------------------------
��������ʼ������
Timer Ϊ��ʱ��ʵ����
Mode Ϊ����ģʽ������Ϊ��
TIMER_A_STOP_MODE
TIMER_A_UP_MODE
TIMER_A_CONTINUOUS_MODE [Default]
TIMER_A_UPDOWN_MODE
---------------------------------------------------------------------------
*/
#define User_Timer_startCounter(Timer,Mode) Timer_A_startCounter(TIMER_##Timer##_BASE, Mode)

/*
---------------------------------------------------------------------------
��ʱ������
�ú���ʹ��TA0CCR0��ʱ�����ж�ʱ����ʱ Clk ��SMCLK���ڣ�CallBackΪ��ʱ���ص�������
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

/*ֹͣ��ʱ��*/
#define User_Timer_StopAlarm() TA0CCR0 = 0
/*������ʱ����ClkΪ�¶�ʱ�����ڣ� �������������User_Timer_StopAlarm�Ѿ�
�����õ�����µ��ò���Ч*/
#define User_Timer_ReStartAlarm(Clk) TA0CCR0 = Clk


#endif