#ifndef __USER_I2C_H__
#define __USER_I2C_H__
#include "driverlib.h"
#include "User_Config.h"

/*
-----------------------------------------------------------------------------
I2C 模块管脚映射：
UCB0SDA  =  P3.0
UCB0SCL  = P3.1

UCB1SDA = P4.1
UCB1SCL = P4.2
-----------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
I2C 输出管脚配置函数
GPIO_PORT 可以为 GPIO_PORT_Px 如 GPIO_PORT_P1 GPIO_PORT_P2 GPIO_PORT_P3等等
GPIO_PIN  可以为 GPIO_PINx 如GPIO_PIN0 GPIO_PIN1等等，
并且可以用GPIO_PIN0 + GPIO_PIN1等方式一次配置多个管脚
------------------------------------------------------------------------------
*/
#define User_I2C_SetAFIO(Port, Pin) \
GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_##Port, GPIO_##Pin)

/*
------------------------------------------------------------------------------
I2C设置工作模式：
Bx 为I2C模块实例，可以是B0 B1
Mode 为工作模式，可以是RECEIVE(接收模式)或者 TRANSMIT (发送模式)
------------------------------------------------------------------------------
*/
#define User_I2C_SetMode(Mode)  USCI_B_I2C_setMode(USER_CONFIG_USCI_Bx_Base, USCI_B_I2C_##Mode##_MODE)

/*
------------------------------------------------------------------------------
I2C主机初始化
Bx 为I2C 模块实例，可以使B0 B1
Mode 为工作模式，可以是RECEIVE(接收模式)或者 TRANSMIT (发送模式)
------------------------------------------------------------------------------
*/
#define User_I2C_MasterInit(Mode, SlaveAddress) do{\
    USCI_B_I2C_initMasterParam param = {0};\
    param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;\
    param.i2cClk = UCS_getSMCLK();\
    param.dataRate = USCI_B_I2C_SET_DATA_RATE_400KBPS;\
    USCI_B_I2C_initMaster(USER_CONFIG_USCI_Bx_Base, &param);\
    USCI_B_I2C_setSlaveAddress(USER_CONFIG_USCI_Bx_Base, SlaveAddress);\
    User_I2C_SetMode(Mode);\
    USCI_B_I2C_enable(USER_CONFIG_USCI_Bx_Base);\
    __delay_cycles(50);\
}while(0)



/*
-------------------------------------------------------------------------------
I2C从机初始化：
Bx 为I2C 模块实例，可以使B0 B1
Mode 为工作模式，可以是 RECEIVE (接收模式)或者TRANSMIT(发送模式)
Adress 为从机地址，为任意高7位数
-------------------------------------------------------------------------------
*/
#define User_I2C_SlaveInit(Mode, Address) do{\
    USCI_B_I2C_initSlave(USER_CONFIG_USCI_Bx_Base, Address);\
    User_I2C_SetMode(Mode);\
    USCI_B_I2C_enable(USER_CONFIG_USCI_Bx_Base);\
}while(0)

/*
------------------------------------------------------------------------------
I2C设置从机地址：
Bx 为 I2C 模块实例，可以是B0 B1
Address 为从机地址
------------------------------------------------------------------------------
*/
#define User_I2C_SetSlaveAddress(Address) USCI_B_I2C_setSlaveAddress(USER_CONFIG_USCI_Bx_Base, Address)

/*
------------------------------------------------------------------------------
I2C 模块使能:
Bx 为I2C模块实例，可以使B0 或者B1
------------------------------------------------------------------------------
*/
#define User_I2C_Enable()    USCI_B_I2C_enable(USER_CONFIG_USCI_Bx_Base)

/*
------------------------------------------------------------------------------
User_I2C_SendData :
------------------------------------------------------------------------------
*/
extern int User_I2C_MasterSendData(void *Buf ,uint16_t Length, uint8_t SlaveAddress);
extern int User_I2C_MasterReadData(void *Buf, uint16_t Length, uint8_t SlaveAddress);

/*
------------------------------------------------------------------------------
User_I2C_SlaveSendData: 从机发送数据函数
------------------------------------------------------------------------------
*/
extern int User_I2C_SlaveSendData(void *Buf, uint16_t Length);
extern int User_I2C_SlaveReadData(void *Buf, uint16_t Length);

/*
------------------------------------------------------------------------------
User_I2C_isFinished:
查看是否传输完成
完成则返回1
否则返回 0
------------------------------------------------------------------------------
*/
extern uint8_t User_I2C_isTxFinishedFlag;

inline uint8_t User_I2C_isTxFinished(void)
{
    return User_I2C_isTxFinishedFlag;
}

/*
-------------------------------------------------------------------------------
User_I2C_EnableIntrrupt: 使能相关中断
User_I2C_ClearInturrupt: 清除中断

IntMask 可以为:
STOP
START
RECEIVE
TRANSMIT
NAK
-------------------------------------------------------------------------------
*/
#define User_I2C_EnableInterrupt(IntMask)  \
    USCI_B_I2C_enableInterrupt(USER_CONFIG_USCI_Bx_Base,  USCI_B_I2C_##IntMask##_INTERRUPT)
#define User_I2C_ClearInterrupt(IntMask)\
    USCI_B_I2C_clearInterrupt(USER_CONFIG_USCI_Bx_Base, USCI_B_I2C_##IntMask##_INTERRUPT)


#endif


