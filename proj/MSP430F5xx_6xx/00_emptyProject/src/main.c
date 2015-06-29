/*
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
Headers:
！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
*/
#include "User_SysInit.h"
#include "User_Config.h"
#include "User_ADC.h"
#include "User_Timer.h"
#include "User_I2C.h"
#include <stdio.h>

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
volatile uint8_t data[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
volatile uint8_t RxData[10];

/*
------------------------------------------------------------------------------
Local Functions
------------------------------------------------------------------------------
*/
void Exit_LPM0(void)
{
    return;
}

/*
------------------------------------------------------------------------------
Main
------------------------------------------------------------------------------
*/
#if (!MODULES_DEBUG)
void main(void)
{
    WatchDogStop();
    /*譜崔砿重鹸喘*/
    User_TimerA_SetAFIO(P2,PIN0);
    /*
    -------------------------------------------------------------------------
    PWM襖30KHz議譜崔��
    TIMERA_CLK = 3M 夸TIMER_PERIOD = TA1CCR0 = 3M / 30K = 100
    -------------------------------------------------------------------------
    */
    User_TimerA_OutPutPWM(A1, TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_1,
                 100, 50, TIMER_A_CAPTURECOMPARE_REGISTER_1,TIMER_A_OUTPUTMODE_SET_RESET);

    /*I2C 砿重鹸喘塘崔*/
    User_I2C_SetAFIO(P3,PIN0);
    User_I2C_SetAFIO(P3,PIN1);

#ifdef USER_I2C_MASTER
    User_I2C_MasterInit(TRANSMIT, 0x48);
#else
    /*兜兵晒葎貫字*/
    User_I2C_SlaveInit(RECEIVE, 0x48);
#endif

    User_I2C_ClearInterrupt(TRANSMIT);
    User_I2C_EnableInterrupt(TRANSMIT);

    User_I2C_ClearInterrupt(STOP);
    User_I2C_EnableInterrupt(STOP);

    User_I2C_ClearInterrupt(RECEIVE);
    User_I2C_EnableInterrupt(RECEIVE);

    __bis_SR_register(GIE);

#ifdef USER_I2C_MASTER
    static uint8_t TxData[4] = {'a','b','c','d'};

    User_Timer_StartAlarm(60000, Exit_LPM0);
    User_Timer_StopAlarm();
    while(1){
        User_I2C_MasterSendData(TxData, sizeof(TxData), 0x48);
        User_I2C_MasterReadData((void*)RxData, sizeof(RxData), 0x48);
    }
#else
    static uint8_t SalveRxData[10] = {2,3,4,5,6,7,8,9,10,11};
    static uint8_t SalveRxData1[10] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
    static uint8_t SlaveRx[10];

    while(1){
        User_I2C_SlaveSendData(SalveRxData, sizeof(SalveRxData));
        User_I2C_SlaveReadData(SlaveRx, sizeof(SlaveRx));

        User_Timer_StartAlarm(60000, Exit_LPM0);
        __bis_SR_register(LPM0_bits);
        User_Timer_StopAlarm();
        User_I2C_SlaveSendData(SalveRxData1, sizeof(SalveRxData1));
        User_Timer_StartAlarm(60000, Exit_LPM0);
        __bis_SR_register(LPM0_bits);
        User_Timer_StopAlarm();
    }

#endif
}



#endif



