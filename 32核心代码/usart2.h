//#ifndef __USART2_H
//#define __USART2_H 
//#include "sys.h"
//#include "stdio.h"	  
////////////////////////////////////////////////////////////////////////////////////	   
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F407������
////����2��ʼ�� 
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2014/5/14
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
////All rights reserved
////********************************************************************************
////�޸�˵��
////��
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















