/*
 * LCD12864P.h
 *
 *  Created on: 2014-8-3
 *      Author: mabao
 */

#ifndef LCD12864P_H_
#define LCD12864P_H_

//#define USE_SPI 1
//#define ucosii 1
typedef  unsigned int uint;
typedef  unsigned char uchar;

extern void Init_Lcd(void);
extern void User_printf (uchar x, uchar y,uchar align,const char *fmt,...);
extern void Send(uchar type,uchar transdata);

#define LCD_Clear() Send(0,0x01)
#endif /* LCD12864P_H_ */
