#ifndef __USER_SYSINIT_H__
#define __USER_SYSINIT_H__

#include "driverlib.h"
#define WatchDogStop()    WDT_A_hold(WDT_A_BASE)
inline void PowerLevel_3(void)
{
     PMM_setVCore(PMM_CORE_LEVEL_1);
    __delay_cycles(10000);

    PMM_setVCore(PMM_CORE_LEVEL_2);
    __delay_cycles(10000);

    PMM_setVCore(PMM_CORE_LEVEL_3);
    __delay_cycles(10000);
}

/*
 * @ 使用外部手表晶振32768Hz，倍频到24Mhz。
 * 初始化 ALCK= 32768Hz
 *     MCLK = 23986176Hz
 *     SMCLK = 2998272Hz
 */
inline void Clk_MCLK_24M_SMCLK_3M_ACLK_32768Hz(void)
{
   PowerLevel_3();
   UCS_initClockSignal (UCS_FLLREF,UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1 );
   UCS_initClockSignal (UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1 );//2998272Hz ~~= 24M
   UCS_initClockSignal (UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_8 );//23986176Hz ~~= 3M
   UCS_initClockSignal (UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1 );//32768Hz
   UCS_initFLLSettle (24000, 732);
}

#endif