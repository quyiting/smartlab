#include "sccb.h"
#include "delay.h"

//初始化SCCB接口 
void SCCB_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟
    
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;//PD6、PD7
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出模式
  	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
  	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//100MHz
  	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//内部上拉
    //应用设置
  	GPIO_Init(GPIOD,&GPIO_InitStructure);
 
	GPIO_SetBits(GPIOD,GPIO_Pin_6|GPIO_Pin_7);//初始为高电平
	SCCB_SDA_OUT();
}

//SCCB起始信号
//时钟为高时,数据线下降沿,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    SCCB_SDA=1;//数据线高电平	   
    SCCB_SCL=1;//时钟线高时数据线下降沿
    delay_us(50);
    SCCB_SDA=0;
    delay_us(50);
    SCCB_SCL=0;//数据线恢复低电平，单操作函数必要	  
}

//SCCB停止信号
//时钟为高时,数据线上升沿,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    SCCB_SDA=0;
    delay_us(50);	 
    SCCB_SCL=1;//数据线高电平
    delay_us(50);
    SCCB_SDA=1;//时钟线高时数据线上升沿
    delay_us(50);
}

//NA信号
void SCCB_No_Ack(void)
{
	delay_us(50);
	SCCB_SDA=1;//SDA、SCL都为高电平
	SCCB_SCL=1;
	delay_us(50);
	SCCB_SCL=0;
	delay_us(50);
	SCCB_SDA=0;
	delay_us(50);
}

//SCCB写入一个字节
//返回值:0,成功;1,失败
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++)//循环8次发送数据
	{
		if(dat&0x80)
            SCCB_SDA=1;	
		else
            SCCB_SDA=0;
		dat<<=1;//发送数据
		delay_us(50);
		SCCB_SCL=1;
		delay_us(50);
		SCCB_SCL=0;		   
	}
	SCCB_SDA_IN();//设置SDA为输入 
	delay_us(50);
	SCCB_SCL=1;//接收第九位,判断是否发送成功
	delay_us(50);
	if(SCCB_READ_SDA)//读取到NA数据
        res=1;//SDA=1发送失败，返回1
	else
        res=0;//SDA=0发送成功，返回0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();//设置SDA为输出 
    
	return res;
}

//SCCB读取一个字节
//SCL上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;
	SCCB_SDA_IN();//设置SDA为输入  
	for(j=8;j>0;j--)//循环8次接收数据
	{
		delay_us(50);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)
            temp++;   
		delay_us(50);
		SCCB_SCL=0;
	}
	SCCB_SDA_OUT();//设置SDA为输出    
	
    return temp;
}

//写寄存器
//返回值:0,成功;1,失败.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start();//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))
        res=1;//写器件ID	  
	delay_us(100);
  	if(SCCB_WR_Byte(reg))
        res=1;//写寄存器地址	  
	delay_us(100);
  	if(SCCB_WR_Byte(data))
       res=1;//写数据	 
  	SCCB_Stop();
  	return
        res;
}

//读寄存器
//返回值:读到的寄存器值
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start();//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);//写器件ID
	delay_us(100);	 
  	SCCB_WR_Byte(reg);//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	
    //设置寄存器地址后进行读操作
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);//发送读命令	  
	delay_us(100);
  	val=SCCB_RD_Byte();//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}
