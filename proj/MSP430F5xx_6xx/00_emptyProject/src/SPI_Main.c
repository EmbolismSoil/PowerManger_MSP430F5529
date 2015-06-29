#include "driverlib.h"
#include "User_Spi.h"
#include "User_SysInit.h"
#include "LCD12864P.h"
#include <msp430.h>

void main(void)
{
     WatchDogStop();
     Init_Lcd();
     Clk_MCLK_24M_SMCLK_3M_ACLK_32768Hz();
/*
     P8REN |= BIT2;                            // Enable P1.4 internal resistance
     P8OUT |= BIT2;                            // Set P1.4 as pull-Up resistance
     P8IES &= ~BIT2;                           // P1.4 Lo/Hi edge
     P8IFG &= ~BIT2;                           // P1.4 IFG cleared
     P8IE |= BIT2;                             // P1.4 interrupt enabled
*/
     double Pi = 1;
     int cnt = 0;
   while(1){
    //
     User_printf(0,0,1,"Pi = %lf",Pi);
     __delay_cycles(10000);
     User_printf(1,0,1, "Hello World");
    __delay_cycles(10000);
     User_printf(2,0,1, "cnt = %d",cnt++);
    __delay_cycles(10000);
     User_printf(3,0,1, "cnt*Pi = %.1lf",Pi *cnt);
     __delay_cycles(10000);
  }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  //P8IFG &= ~BIT2;                         // Clear P1.4 IFG
  //P2IE &= ~ BIT6;                         // Clear P1.4 IE
}