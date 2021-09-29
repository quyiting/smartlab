#ifndef __SCCB_H
	#define __SCCB_H
	#include "sys.h"
	//IO方向设置
	#define SCCB_SDA_IN() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;} //PD7输入
	#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} //PD7输出

	//IO操作函数
	#define SCCB_SCL PDout(6) //PD6配置为SCL(仅输出)
	#define SCCB_SDA PDout(7) //SDA输出
	#define SCCB_READ_SDA PDin(7) //SDA输出

	#define SCCB_ID 0X60 //OV2640的ID

	void SCCB_Init(void);//SCCB接口初始化
	void SCCB_Start(void);//起始信号
	void SCCB_Stop(void);//停止信号
	void SCCB_No_Ack(void);//NA信号
	u8 SCCB_WR_Byte(u8 dat);//写数据
	u8 SCCB_RD_Byte(void);//读数据
	u8 SCCB_WR_Reg(u8 reg,u8 data);//写寄存器
	u8 SCCB_RD_Reg(u8 reg);//读寄存器
#endif
