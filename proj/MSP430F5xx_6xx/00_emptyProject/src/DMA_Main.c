/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F552x Demo - DMA0, Single transfer using ADC12 triggered by TimerB
//
//  Description: This software uses TBCCR1 as a sample and convert input into
//  the A0 of ADC12. ADC12IFG is used to trigger a DMA transfer and DMA
//  interrupt triggers when DMA transfer is done. TB1 is set as an output and
//  P1.0 is toggled when DMA ISR is serviced.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = default DCO 1048576Hz
//
//                 MSP430F552x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//            |             P5.7|-->TB1 output
//            |                 |
//            |             P6.0|<--A0
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************

#include <msp430.h>
#include "driverlib.h"


unsigned int DMA_DST;						// ADC conversion result is stored in this variable

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Hold WDT

  P1OUT &= ~BIT0;                           // P1.0 clear
  P1DIR |= BIT0;                            // P1.0 output
  P5SEL |= BIT7;                            // P5.7/TB1 option select
  P5DIR |= BIT7;                            // Output direction
  P6SEL |= BIT0;                            // Enable A/D channel A0

  //Setup Timer B0
  TBCCR0 = 0xFFFE;
  TBCCR1 = 0x8000;
  TBCCTL1 = OUTMOD_3;                       // CCR1 set/reset mode
  TBCTL = TBSSEL_2+MC_1+TBCLR;              // SMCLK, Up-Mode

  // Setup ADC12
  ADC12CTL0 = ADC12SHT0_15+ADC12MSC+ADC12ON;// Sampling time, MSC, ADC12 on
  ADC12CTL1 = ADC12SHS_3+ADC12CONSEQ_2;     // Use sampling timer; ADC12MEM0
                                            // Sample-and-hold source = CCI0B =
                                            // TBCCR1 output
                                            // Repeated-single-channel
  ADC12MCTL0 = ADC12SREF_0+ADC12INCH_0;     // V+=AVcc V-=AVss, A0 channel
  ADC12CTL0 |= ADC12ENC;

    //Disable the DMA from preempting a Read-Modify-Write Operation in the CPU
    DMA_disableTransferDuringReadModifyWrite();

    //Initialize and Setup DMA Channel 0
    /*
     * Configure DMA channel 0
     * Configure channel for repeated single transfers
     * DMA interrupt flag will be set after every 1 transfer
     * Use DMA Trigger Source 24 (ADC12xIFG)
     * Transfer Word-to-Word
     * Trigger upon Rising Edge of Trigger Source Signal
     */
    DMA_initParam initParam = {0};
    initParam.channelSelect = DMA_CHANNEL_0;
    initParam.transferModeSelect = DMA_TRANSFER_REPEATED_SINGLE;
    initParam.transferSize = 1;
    initParam.triggerSourceSelect = DMA_TRIGGERSOURCE_24;
    initParam.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    initParam.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;
    DMA_init(&initParam);
    /*
     * Configure DMA channel 0
     * Use ADC12 Memory Buffer 0 as source
     * Don't move the source address after every transfer
     */
    DMA_setSrcAddress(DMA_CHANNEL_0,
                      ADC12_A_getMemoryAddressForDMA(ADC12_A_BASE,
                                                     ADC12_A_MEMORY_0),
                      DMA_DIRECTION_UNCHANGED);
    /*
     * Configure DMA channel 0
     * Use DMA_DST as destination
     * Increment destination address after every transfer
     */
    DMA_setDstAddress(DMA_CHANNEL_0,
                      (uint32_t)&DMA_DST,
                      DMA_DIRECTION_INCREMENT);

    //Enable DMA channel 0 interrupt
    DMA_clearInterrupt(DMA_CHANNEL_0);
    DMA_enableInterrupt(DMA_CHANNEL_0);

    //Enable transfers on DMA channel 0
    DMA_enableTransfers(DMA_CHANNEL_0);

    //LPM0 w/ interrupts
    __bis_SR_register(LPM0_bits + GIE);
    //For Debugger
    //__no_operation();

    while(1);
}

//------------------------------------------------------------------------------
// DMA Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
  switch(__even_in_range(DMAIV,16))
  {
    case 0: break;
    case 2:                                 // DMA0IFG = DMA Channel 0
      P1OUT ^= BIT0;                        // Toggle P1.0 - PLACE BREAKPOINT HERE AND CHECK DMA_DST VARIABLE
      break;
    case 4: break;                          // DMA1IFG = DMA Channel 1
    case 6: break;                          // DMA2IFG = DMA Channel 2
    case 8: break;                          // DMA3IFG = DMA Channel 3
    case 10: break;                         // DMA4IFG = DMA Channel 4
    case 12: break;                         // DMA5IFG = DMA Channel 5
    case 14: break;                         // DMA6IFG = DMA Channel 6
    case 16: break;                         // DMA7IFG = DMA Channel 7
    default: break;
  }
}
