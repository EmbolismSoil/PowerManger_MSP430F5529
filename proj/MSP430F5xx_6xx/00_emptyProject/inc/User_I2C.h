#ifndef __USER_I2C_H__
#define __USER_I2C_H__
#include "driverlib.h"
#include "User_Config.h"

/*
-----------------------------------------------------------------------------
I2C ģ��ܽ�ӳ�䣺
UCB0SDA  =  P3.0
UCB0SCL  = P3.1

UCB1SDA = P4.1
UCB1SCL = P4.2
-----------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------------
I2C ����ܽ����ú���
GPIO_PORT ����Ϊ GPIO_PORT_Px �� GPIO_PORT_P1 GPIO_PORT_P2 GPIO_PORT_P3�ȵ�
GPIO_PIN  ����Ϊ GPIO_PINx ��GPIO_PIN0 GPIO_PIN1�ȵȣ�
���ҿ�����GPIO_PIN0 + GPIO_PIN1�ȷ�ʽһ�����ö���ܽ�
------------------------------------------------------------------------------
*/
#define User_I2C_SetAFIO(Port, Pin) \
GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_##Port, GPIO_##Pin)

/*
------------------------------------------------------------------------------
I2C���ù���ģʽ��
Bx ΪI2Cģ��ʵ����������B0 B1
Mode Ϊ����ģʽ��������RECEIVE(����ģʽ)���� TRANSMIT (����ģʽ)
------------------------------------------------------------------------------
*/
#define User_I2C_SetMode(Mode)  USCI_B_I2C_setMode(USER_CONFIG_USCI_Bx_Base, USCI_B_I2C_##Mode##_MODE)

/*
------------------------------------------------------------------------------
I2C������ʼ��
Bx ΪI2C ģ��ʵ��������ʹB0 B1
Mode Ϊ����ģʽ��������RECEIVE(����ģʽ)���� TRANSMIT (����ģʽ)
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
I2C�ӻ���ʼ����
Bx ΪI2C ģ��ʵ��������ʹB0 B1
Mode Ϊ����ģʽ�������� RECEIVE (����ģʽ)����TRANSMIT(����ģʽ)
Adress Ϊ�ӻ���ַ��Ϊ�����7λ��
-------------------------------------------------------------------------------
*/
#define User_I2C_SlaveInit(Mode, Address) do{\
    USCI_B_I2C_initSlave(USER_CONFIG_USCI_Bx_Base, Address);\
    User_I2C_SetMode(Mode);\
    USCI_B_I2C_enable(USER_CONFIG_USCI_Bx_Base);\
}while(0)

/*
------------------------------------------------------------------------------
I2C���ôӻ���ַ��
Bx Ϊ I2C ģ��ʵ����������B0 B1
Address Ϊ�ӻ���ַ
------------------------------------------------------------------------------
*/
#define User_I2C_SetSlaveAddress(Address) USCI_B_I2C_setSlaveAddress(USER_CONFIG_USCI_Bx_Base, Address)

/*
------------------------------------------------------------------------------
I2C ģ��ʹ��:
Bx ΪI2Cģ��ʵ��������ʹB0 ����B1
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
User_I2C_SlaveSendData: �ӻ��������ݺ���
------------------------------------------------------------------------------
*/
extern int User_I2C_SlaveSendData(void *Buf, uint16_t Length);
extern int User_I2C_SlaveReadData(void *Buf, uint16_t Length);

/*
------------------------------------------------------------------------------
User_I2C_isFinished:
�鿴�Ƿ������
����򷵻�1
���򷵻� 0
------------------------------------------------------------------------------
*/
extern uint8_t User_I2C_isTxFinishedFlag;

inline uint8_t User_I2C_isTxFinished(void)
{
    return User_I2C_isTxFinishedFlag;
}

/*
-------------------------------------------------------------------------------
User_I2C_EnableIntrrupt: ʹ������ж�
User_I2C_ClearInturrupt: ����ж�

IntMask ����Ϊ:
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


