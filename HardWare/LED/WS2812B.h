/************************************************************
Copyright (C), 2013-2020, XINZHIYIKONG.Co.,Ltd.
@FileName: WS2812B.h
@Author  : 糊读虫 QQ:570525287
@Version : 1.0
@Date    : 2020-12-25
@Description: WS2812B全彩LED灯驱动
@Function List:
@History    :
<author> <time> <version > <desc>

***********************************************************/

#ifndef __WS2812B_H
#define	__WS2812B_H

#include "main.h"


#define u8   uint8_t
#define u16   uint16_t
#define WS2812B_Hi()		HAL_GPIO_WritePin(GPIOD, LED_Pin, 1)
#define WS2812B_Low()		HAL_GPIO_WritePin(GPIOD, LED_Pin, 0)

//颜色
typedef struct color{
	u8 G;
	u8 R;
	u8 B;
}Color_TypeDef;

//------------------------
#define PIXEL_NUM	1	//LED灯的个数

void WS2812B_Init(void);
void WS2812B_Reset(void);
void WS2812B_WriteColor(Color_TypeDef *pColor);
void WS2812B_RefreshPixel(void);

void WS2812B_FillColor(u16 start,u16 end,Color_TypeDef *pColor);
void WS2812B_MovePixel(u8 dir);

void WS2812B_Test(void);
#endif /* __WS2812B_H */


