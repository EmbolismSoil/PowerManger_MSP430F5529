/*
����������������������������������������������������������������������������������������������������������������������������������������������������������������
Headers:
����������������������������������������������������������������������������������������������������������������������������������������������������������������
*/
#include "User_I2C.h"
#include "User_Timer.h"
#include "User_Config.h"
/*For NULL*/
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

/*I2C ���ݻ�����*/
typedef struct{
    /*���ݻ�����*/
    uint8_t *Buf;
    /*��ǰ�������ڵ����ݳ��ȣ���λ �ֽ�*/
    uint16_t CurLength;
    /*��ǰ���ݴ洢�ڵ�*/
 //   uint16_t WIndex;
    /*��ǰ���ݶ�ȡ�ڵ�*/
 //   uint16_t RIndex;
    /*������ݳ���*/
    uint16_t MaxLength;
}I2C_DataBuf_t;

/*���ͻ�����*/
static I2C_DataBuf_t TxBuf = {.CurLength = 0};
/*���ջ�����*/
static I2C_DataBuf_t RxBuf = {.CurLength = 0};

/*�ӻ����ͻ�������*/
static uint8_t  SlaveTxBuffer[I2C_MAX_BUF_SIZES];
static uint8_t  SlaveRxBuffer[I2C_MAX_BUF_SIZES];
static I2C_DataBuf_t SlaveTxBuf = {.CurLength = 0};
/*�ӻ����ջ�����*/
static I2C_DataBuf_t SlaveRxBuf = {.CurLength = 0};

static uint8_t User_I2C_SlaveRxLock = 0;
static uint8_t User_I2C_SlaveTxLock = 0;

uint8_t User_I2C_isTxFinishedFlag = 0;
uint8_t User_I2C_isRxFinishedFlag = 0;
/*
------------------------------------------------------------------------------
Local Functions
------------------------------------------------------------------------------
*/

/*
@ __User_I2C_MasterGetTxData: I2C��ȡ���ݺ���,�ɹ��򷵻�1��ʧ�ܷ���0
*/
static int __User_I2C_MasterGetTxData(uint8_t *Data)
{
    if (TxBuf.CurLength == TxBuf.MaxLength ||
                                      !TxBuf.Buf){
        TxBuf.Buf = NULL;
        TxBuf.CurLength = 0;
        TxBuf.MaxLength = 0;
        return 0;
    }

    *Data = TxBuf.Buf[TxBuf.CurLength++];
    return 1;
}

static uint16_t __User_I2C_MasterPushRxData(uint8_t Data)
{
    if (RxBuf.CurLength == RxBuf.MaxLength || !RxBuf.Buf){
        RxBuf.Buf = NULL;
        RxBuf.CurLength = 0;
        RxBuf.MaxLength = 0;
        return 0;
    }

    RxBuf.Buf[RxBuf.CurLength++] = Data;

    return RxBuf.CurLength;
}

static int __User_I2C_SlaveGetTxData(uint8_t *Data)
{
    if (!SlaveTxBuf.Buf ||
        SlaveTxBuf.CurLength == SlaveTxBuf.MaxLength){

        SlaveTxBuf.CurLength = 0;
        SlaveTxBuf.MaxLength = 0;
        SlaveTxBuf.Buf = NULL;
        return 0;
    }

   *Data = SlaveTxBuf.Buf[SlaveTxBuf.CurLength++];
    return 1;
}

static uint16_t __User_I2C_SlavePushRxData(uint8_t Data)
{
    if (SlaveRxBuf.CurLength == SlaveRxBuf.MaxLength ||
                                        !SlaveRxBuf.Buf){
        SlaveRxBuf.Buf = NULL;
        SlaveRxBuf.CurLength = 0;
        SlaveRxBuf.MaxLength = 0;
        return 0;
    }

    SlaveRxBuf.Buf[SlaveRxBuf.CurLength++] = Data;
    return SlaveRxBuf.CurLength;
}


/*
------------------------------------------------------------------------------
Module Functions
------------------------------------------------------------------------------
*/

#ifdef USER_I2C_MASTER
/*
@ User_I2C_MasterSendData: I2C�������ݺ������ɹ�����1��ʧ�ܷ���0
  Buf Ϊ��������ָ��
  Length Ϊ�������ݳ���
  SlaveAddress �ӻ���ַ
  isForce �����Ƿ�ǿ���ش�
*/
int User_I2C_MasterSendData(void *Buf ,uint16_t Length, uint8_t SlaveAddress)
{
    uint8_t tmpdata = 0;

    if (Buf && Length){
        TxBuf.Buf = (uint8_t *)Buf;
        TxBuf.MaxLength = Length;
        TxBuf.CurLength = 0;
    }

    if ( (!USCI_B_I2C_isBusBusy(USER_CONFIG_USCI_Bx_Base) &&
          __User_I2C_MasterGetTxData(&tmpdata))){
            User_I2C_SetSlaveAddress(SlaveAddress);
            USCI_B_I2C_clearInterrupt(USER_CONFIG_USCI_Bx_Base, USCI_B_I2C_TRANSMIT_INTERRUPT);
            USCI_B_I2C_enableInterrupt(USER_CONFIG_USCI_Bx_Base, USCI_B_I2C_TRANSMIT_INTERRUPT);
            /*������ʱ��*/
            User_Timer_StartAlarm(60000, NULL);
            User_I2C_isTxFinishedFlag = 1;
            if (USCI_B_I2C_masterSendMultiByteStartWithTimeout(USER_CONFIG_USCI_Bx_Base, tmpdata,1000))
#if I2C_EXT_LPMODE != -1
                __bis_SR_register(I2C_EXT_LPMODE);
#else
                while(!User_I2C_isTxFinishedFlag);
#endif
            else{
               User_I2C_isTxFinishedFlag = 0;
              return 0;
            }
            User_Timer_StopAlarm();
    }else return 0;

    return 1;
}
#else

int User_I2C_SlaveSendData(void *Buf, uint16_t Length)
{
    if (!Buf || !Length) return 0;

    SlaveTxBuf.Buf = (uint8_t *)Buf;
    SlaveTxBuf.MaxLength = Length;
    SlaveTxBuf.CurLength = 0;

    return 1;
}

#endif

/*
------------------------------------------------------------------------------
User_I2C_ReadData:
��ȡ���ݺ�������ȡ�ɹ��򷵻� 1 ʧ�ܷ��� 0
------------------------------------------------------------------------------
*/

#ifdef USER_I2C_MASTER
int User_I2C_MasterReadData(void *Buf, uint16_t Length, uint8_t SlaveAddress)
{

   if (USCI_B_I2C_isBusBusy(USER_CONFIG_USCI_Bx_Base) || !Buf)
       return 0;

   User_I2C_SetSlaveAddress(SlaveAddress);
   User_I2C_SetMode(RECEIVE);

   RxBuf.MaxLength = Length;
   RxBuf.CurLength = 0;
   RxBuf.Buf = (uint8_t *)Buf;

   User_I2C_isRxFinishedFlag = 0;

#if I2C_EXT_LPMODE != -1
   User_Timer_StartAlarm(60000, NULL);
   USCI_B_I2C_masterReceiveMultiByteStart(USER_CONFIG_USCI_Bx_Base);
   __bis_SR_register(I2C_EXT_LPMODE);
   User_Timer_StopAlarm();
#else
   USCI_B_I2C_masterReceiveMultiByteStart(USER_CONFIG_USCI_Bx_Base);
   while(!User_I2C_isRxFinishedFlag);
#endif
    User_I2C_SetMode(TRANSMIT);

   if (RxBuf.CurLength < RxBuf.MaxLength)
       return 0;

   return 1;
}
#else
int User_I2C_SlaveReadData(void *Buf, uint16_t Length)
{
    if (!Buf || !Length) return 0;

    SlaveRxBuf.Buf = (uint8_t *)Buf;
    SlaveRxBuf.CurLength = 0;
    SlaveRxBuf.MaxLength = Length;

    return 1;
}
#endif

/*
------------------------------------------------------------------------------
ISR
------------------------------------------------------------------------------
*/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif

#ifdef USER_I2C_MASTER

void USCI_B0_ISR(void)
{
    uint8_t tmpData = 0;
    switch(__even_in_range(UCB0IV,12))
    {
        /*��ʼ�����жϣ���ʱ������д�뷢�ͻ�����*/
        case USCI_I2C_UCTXIFG:
        {
             uint8_t tmp;
            if( __User_I2C_MasterGetTxData(&tmp) )
                USCI_B_I2C_masterSendMultiByteNext(USER_CONFIG_USCI_Bx_Base,tmp);
            else
            {
                USCI_B_I2C_masterSendMultiByteStop(USER_CONFIG_USCI_Bx_Base);
                User_I2C_isTxFinishedFlag = 1;
    #if I2C_EXT_LPMODE != -1
                __bic_SR_register_on_exit(I2C_EXT_LPMODE);
    #endif
            }
            break;
        }

        case USCI_I2C_UCRXIFG :

            if (RxBuf.CurLength == RxBuf.MaxLength - 1){
                tmpData = USCI_B_I2C_masterReceiveMultiByteFinish(USER_CONFIG_USCI_Bx_Base);
                __User_I2C_MasterPushRxData(tmpData);
                User_I2C_isRxFinishedFlag = 1;
    #if I2C_EXT_LPMODE != -1
                __bic_SR_register_on_exit(I2C_EXT_LPMODE);
    #endif

            }else{
                tmpData = USCI_B_I2C_masterReceiveMultiByteNext(USER_CONFIG_USCI_Bx_Base);
                 __User_I2C_MasterPushRxData(tmpData);
            }
#if 0
            if ( !__User_I2C_MasterPushRxData(tmpData)){
                User_I2C_isRxFinishedFlag = 1;
   #if I2C_EXT_LPMODE != -1
                __bic_SR_register_on_exit(I2C_EXT_LPMODE);
   #endif
            }
#endif
            break;

        case USCI_I2C_UCSTPIFG :
    #if I2C_EXT_LPMODE != -1
           __bic_SR_register_on_exit(I2C_EXT_LPMODE);
    #endif
           break;

    }
}
#else
void USCI_B0_ISR(void)
{
    uint8_t tmpData = 0;
    switch(__even_in_range(UCB0IV,12))
    {
        /*��ʼ�����жϣ���ʱ������д�뷢�ͻ�����*/
        case USCI_I2C_UCTXIFG:
        {
                __User_I2C_SlaveGetTxData(&tmpData);
                USCI_B_I2C_slavePutData(USER_CONFIG_USCI_Bx_Base, tmpData);
            break;
        }

        case USCI_I2C_UCRXIFG :
            tmpData = USCI_B_I2C_slaveGetData(USER_CONFIG_USCI_Bx_Base);
            __User_I2C_SlavePushRxData(tmpData);
            break;

        case USCI_I2C_UCSTPIFG :
    #if I2C_EXT_LPMODE != -1
           __bic_SR_register_on_exit(I2C_EXT_LPMODE);
    #endif
           break;

    }
}
#endif

