/*
------------------------------------------------------------------------------
Headers:
------------------------------------------------------------------------------
*/
#include <msp430.h>
#include "driverlib.h"
#include "User_SysInit.h"
#include "User_Config.h"
#include "User_Timer.h"

#if MODULE_TIMER_ENABLE
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
static User_Timer_CallBack_t User_Timer_CallBack;

/*
------------------------------------------------------------------------------
Local Functions
------------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
Module's Functions
------------------------------------------------------------------------------
*/
void User_Timer_SetCallBack(User_Timer_CallBack_t CallBack)
{
    User_Timer_CallBack = CallBack;
    return;
}

User_Timer_CallBack_t UserTimer_GetCallBack(void)
{
    return User_Timer_CallBack;
}

/*
------------------------------------------------------------------------------
Demo Main
------------------------------------------------------------------------------
*/


/*
------------------------------------------------------------------------------
                                    ISR
------------------------------------------------------------------------------
*/
// Timer0 A0 interrupt service routine
//TA0CCR0 = Timer0_A0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if (User_Timer_CallBack)
        User_Timer_CallBack();
#if TIMER_ALARM_EXT_LPMODE != -1
    __bic_SR_register_on_exit(TIMER_ALARM_EXT_LPMODE);
#endif
}

#endif