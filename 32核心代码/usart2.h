//#ifndef __USART2_H
//#define __USART2_H 
//#include "sys.h"
//#include "stdio.h"	  
////////////////////////////////////////////////////////////////////////////////////	   
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32F407开发板
////串口2初始化 
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2014/5/14
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2014-2024
////All rights reserved
////********************************************************************************
////修改说明
////无
//////////////////////////////////////////////////////////////////////////////////// 	
//extern uint16_t OpenMV_X;      
//extern uint16_t open_x;
// extern uint16_t openmv[5];
//void usart2_init(u32 pclk1,u32 bound); 
//#endif	   

#ifndef __USART2_H
#define __USART2_H  
#include "sys.h"  

#define USART2_MAX_RECV_LEN 400 //?????????
#define USART2_MAX_SEND_LEN 400 //?????????
#define USART2_RX_EN 1 //0,???;1,??.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //????,??USART2_MAX_RECV_LEN??
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; //????,??USART2_MAX_SEND_LEN??
extern u16 USART2_RX_STA;   //??????

void usart2_init(u32 bound); //??2??? 
//void TIM7_Int_Init(u16 arr,u16 psc);
void u2_printf(char* fmt, ...);
#endif















