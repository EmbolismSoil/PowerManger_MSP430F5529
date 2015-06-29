#include "LCD12864P.h"
/****************************************
              Your header
***************************************/
#include <stdio.h>
#include <stdarg.h>
#include "User_Spi.h"

	static char  printbuffer[100];

/**************************************
              Transplantation
**************************************/

/*
 *  RS = PA01 WR = PA05 E = PA07
 *
 */
#define cyCS_High()        P8OUT |= BIT1

#define cyCS_Low()         P8OUT &=~ BIT1


#define cySID_High()       P3OUT |= BIT3
#define cySID_Low()        P3OUT &=~ BIT3

#define cyCLK_High()       P2OUT |= BIT7
#define cyCLK_Low()        P2OUT &=~ BIT7


inline void IO_Init(void)
{
    P8DIR |= BIT1;                            // Set P8.1
    P3DIR |= BIT3;
    P2DIR |= BIT7;

#ifdef USE_SPI
    SPIInit();
#endif
}
/*******************************************
				Config in header
*******************************************/
#ifdef ucosii
#define delay_1ms() OSTimeDlyHMSM(0,0,0,1)
#define delay_nms(n) OSTimeDlyHMSM(0,0,0,n)
#else
#define delay_1ms() delay_1ms_pro()
#define delay_nms(n) delay_nms_pro(n)

static void delay_1ms_pro()
{
        uint i;
        for(i=0;i<1;i++);
}

static void delay_nms_pro(uint n)
{
    uint i = 0;
    for(i = n;i > 0;i--)
        delay_1ms();
}
#endif

/***********************************************
				Encapsulate
***********************************************/

static void lcd_pos(uchar x,uchar y)//定位
{
         uchar pos;
         switch(x)
          {
            case 0:pos=0x80;break;
            case 1:pos=0x90;break;
            case 2:pos=0x88;break;
            case 3:pos=0x98;break;
          }
          pos += y;
          Send(0,pos);
}

void User_printf (uchar x, uchar y,uchar align, const char *fmt,...)
{
        va_list args;
        char *p = printbuffer;

         x %= 4;

        va_start (args, fmt);

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        vsprintf(printbuffer, fmt, args);
        va_end (args);


/* Print the string */
        lcd_pos(x,y);
        while(*p){
             Send(1,*(p++));
             if ( 16== p - printbuffer + y*2 ||
                  32== p - printbuffer + y*2 ||
                  48== p - printbuffer + y*2 ||
                  64== p - printbuffer + y*2)
                 if (align)  lcd_pos(++x,y);
                 else lcd_pos(++x,0);
        }

}




void Init_Lcd(void)
{
        IO_Init();


        delay_nms(1);                 //延时等待液晶完成复位
        Send(0,0x30);  /*功能设置:一次送8位数据,基本指令集*/
        delay_nms(1);
        Send(0,0x02);  /*DDRAM地址归位*/
        delay_nms(1);
        Send(0,0x0c);  /*显示设定:开显示,不显示光标,不做当前显示位反白闪动*/
        delay_nms(1);
        Send(0,0x01);  /*清屏，将DDRAM的位址计数器调整为“00H”*/
        delay_nms(1);
        Send(0,0x06);  /*功能设置，点设定:显示字符/光标从左到右移位,DDRAM地址加1*/
        delay_nms(1);
}


void Send(uchar type,uchar transdata)//0-写控制命令，1-写数据
{
#ifdef USE_SPI
        cyCS_High(); //拉高片选，选中LCD

        if (type)
            SPITransData(0x02);//如果要发送数据，则先发送一个0x02表示这是一个数据
        else
            SPITransData(0xf8);//首先发送一个0xf8,表示这是一个命令

        SPITransData(transdata);
        cyCS_Low();
#else
        uchar firstbyte = 0xf8;
        uchar temp;
        uchar i,j = 3;

        if(type) firstbyte |= 0x02;

		cyCS_High();
		cyCLK_Low();

        while(j > 0)
        {
                if(j == 3) temp = firstbyte;
                else if(j == 2) temp = transdata&0xf0;
                else  temp = (transdata << 4) & 0xf0;

                for(i = 8;i > 0;i--)
                {
                        if(temp & 0x80)    cySID_High();
                        else    cySID_Low();

						cyCLK_High();
                        temp <<= 1;
						cyCLK_Low();
                }
                //三个字节之间一定要有足够的延时，否则易出现时序问题
                if(j == 3)        delay_nms(10);
                else               delay_nms(10);

                j--;
        }

		cySID_Low();
		cyCS_Low();
#endif

}

/*******************************************
函数名称：Draw_PM
功    能：在整个屏幕上画一个图片
参    数：ptr--指向保存图片位置的指针
返回值  ：无
********************************************/
void Draw_PM(const uchar *ptr)
{
    uchar i,j,k;

	Send(0,0x34);        //打开扩展指令集
	i = 0x80;
	for(j = 0;j < 32;j++)
	{
 	 	Send(0,i++);
 		Send(0,0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    Send(1,*ptr++);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    Send(0,i++);
  		Send(0,0x88);
  		for(k = 0;k < 16;k++)
  		{
   		    Send(1,*ptr++);
   		}
	}
    Send(0,0x36);		        //打开绘图显示
	Send(0,0x30);               //回到基本指令集
}
/*******************************************
函数名称：Draw_TX
功    能：在液晶上描绘一个16*16的图形
参    数：Yaddr--Y地址，
          Xaddr--X地址
          dp--指向保存图形数据的指针
返回值  ：无
********************************************/
void Draw_TX(uchar Yaddr,uchar Xaddr,const uchar * dp)
{
    uchar j;
    uchar k = 0;

    Send(0,0x34);           //使用扩展指令集，关闭绘图显示
    for(j = 0;j < 16;j++)
    {
        Send(0,Yaddr++);     //Y地址
        Send(0,Xaddr);       //X地址
        Send(1,dp[k++]);     //送两个字节的显示数据
        Send(1,dp[k++]);
    }
	Send(0,0x36);		    //打开绘图显示
	Send(0,0x30);		    //回到基本指令集模式
}
