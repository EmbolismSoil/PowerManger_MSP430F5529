#ifndef __USER_SPI_H__
#define __USER_SPI_H__
#include "driverlib.h"


inline void SPIInit(void)
{
        //P3.5,4,0 option select
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN3);

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P2,
        GPIO_PIN7);

    //Initialize Master
    USCI_A_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_A_SPI_CLOCKSOURCE_ACLK; // 32768Hz
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = 1000000;
    param.msbFirst = USCI_A_SPI_MSB_FIRST;
    param.clockPhase = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;//时钟相位
    param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH; // 时钟总线空闲时为高
   USCI_A_SPI_initMaster(USCI_A0_BASE, &param);

    //Enable SPI module
    USCI_A_SPI_enable(USCI_A0_BASE);

    __delay_cycles(100);

    //USCI_A0 TX buffer ready?
    while(!USCI_A_SPI_getInterruptStatus(USCI_A0_BASE,
                                         USCI_A_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }

}

inline void SPITransData(uint8_t Data)
{
    while(USCI_A_SPI_isBusy(USCI_A0_BASE) == USCI_A_SPI_BUSY);
    USCI_A_SPI_transmitData(USCI_A0_BASE, Data);
}

#endif