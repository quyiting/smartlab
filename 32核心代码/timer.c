#include "timer.h"
#include "key.h"
#include "delay.h"
#include "adc.h"


u32 t=0,f,T=0;
u8  TIM4CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM4CH1_CAPTURE_VAL;	//输入捕获值
//	float temp;
int time3;
void Tim2IntInit(unsigned int arr,unsigned int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//瀹氫箟瀹氭椂鍣ㄥ垵濮嬪寲鍙傛暟缁撴瀯浣?
	NVIC_InitTypeDef NVIC_InitStructure;			//瀹氫箟宓屽鍚戦噺涓柇缁撴瀯浣?
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //鏃堕挓浣胯兘
	
	TIM_TimeBaseStructure.TIM_Period = arr; //璁剧疆鍦ㄤ笅涓�涓洿鏂颁簨浠惰鍏ユ椿鍔ㄧ殑鑷姩閲嶈杞藉瘎瀛樺櫒鍛ㄦ湡鐨勫�? 璁℃暟鍒?000涓?00ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //璁剧疆鐢ㄦ潵浣滀负TIMx鏃堕挓棰戠巼闄ゆ暟鐨勯鍒嗛鍊?鍒嗛绯绘暟) 褰損sc涓?200鏃讹紝72M/7200=10Khz鐨勮鏁伴鐜囷紙鍗?0k娆′负1s锛?
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //璁剧疆鏃堕挓鍒嗗壊:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM鍚戜笂璁℃暟妯″紡
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //鏍规嵁TIM_TimeBaseInitStruct涓寚瀹氱殑鍙傛暟鍒濆鍖朤IMx鐨勬椂闂村熀鏁板崟浣?
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//浣胯兘tim2鐨勬洿鏂颁腑鏂?
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2涓柇
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //鍏堝崰浼樺厛绾?绾?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //浠庝紭鍏堢骇3绾?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ閫氶亾琚娇鑳?
	NVIC_Init(&NVIC_InitStructure);  //鏍规嵁NVIC_InitStruct涓寚瀹氱殑鍙傛暟鍒濆鍖栧璁綨VIC瀵勫瓨鍣?
	
	TIM_Cmd(TIM2, ENABLE);  //浣胯兘TIMx澶栬
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);
	
}
void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);
	
}
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
//	{
//		if(t<2&&time3==0)	t++;
//		if(time3==1)
//		{
//			T++;
//			TIM_Cmd(TIM3, DISABLE);
//		}
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}

//void TIM3_IRQHandler(void)
//{
//	static int i = 0;
//	if(fftInfo.complete_check == 0)
//	{
//		lBufInArray[i] = ((unsigned long)ADC_ConvertedValue[0])<<16;
//		i++;
//	}
//	if(i==NPT) {fftInfo.complete_check = 1;i=0;}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);      
//}
////void TIM3_IRQHandler(void)
////{	
////	  LCD_ShowString(0,100,240,16,16,"have gone into TIM3 IRQ");
////	  f = f+300;
////	  AD9833_AmpSet(1);
////	  AD9833_WaveSeting(f,0,SIN_WAVE,0);
////		adcxmax2=Get_Adc_Max(ADC_Channel_5,1000,10);
////		adcxmin2=Get_Adc_Min(ADC_Channel_5,1000,10);
////		vppadcx=adcxmax2-adcxmin2;
////	  if (vppmax < vppadcx ){vppmax = vppadcx;}
////		LCD_ShowxNum(85,50,vppmax,4,16,0);
////		LCD_ShowString(0,50,240,16,16,"vppmax is      ");

////		
////////	adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
//////		LCD_ShowxNum(90,36,adcx2,4,12,0);    //显示ADCC采样后的原始值
//////		temp=(float)adcx2*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
//////		adcx2=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
//////		LCD_ShowxNum(80,48,adcx2,1,12,0);    //显示电压值的整数部分，3.1111的话，这里就是显示3
//////		temp-=adcx2;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
//////		temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
//////		LCD_ShowxNum(90,48,temp,3,12,0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.

////}

//void TIM3_PWM_Init(u16 arr,u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
//	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
//	
//	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM3,ENABLE);
//	
//	
//}

//void TIM4_PWM_Init(u16 arr,u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
//	
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM4,ENABLE);
//	
//	
//}

//void TIM4_Cap_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_ICInitTypeDef TIM_ICInitStruct;
//	
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//	
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=arr;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
//	
//	//输入捕获参数初始化
//	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
//	TIM_ICInitStruct.TIM_Channel=TIM_Channel_3;
//	TIM_ICInitStruct.TIM_ICFilter=0x00;//不采集
//	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
//	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM4,&TIM_ICInitStruct);
//	
//	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


//	TIM_Cmd(TIM4, ENABLE);	
//}



void TIM4_IRQHandler(void)
{

	if((TIM4CH1_CAPTURE_STA&0X80)==0)
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
			{	

				TIM4CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
		   		TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //设置为上升沿捕获
	 			TIM_SetCounter(TIM4,0);

			}		
	}
	if(TIM4CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			t++;
			TIM4CH1_CAPTURE_STA=0;//开启下一次捕获
		}
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
}


	void TIM4_CC_IRQHandler(void)
{
	vu8 fq;
	vu8 test;
	vu8 timecount,timecount1,state;
   if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
   {
	   TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);    //???????
           if(state==0)    //????????
           {
               state=1;
	       timecount=TIM_GetCapture4(TIM4);        //?????????CNT?
	   }
	   else if(state==1)//????????
	   {
                  state=0;
		  timecount1=TIM_GetCapture4(TIM4);   //?????????CNT?
               if(timecount<timecount1)
               {
                 test=timecount1-timecount;           //????????
	       }
	       else if(timecount>timecount1)
	       {
                 test=(0xffff-timecount)+timecount1;  //????????
	       }
	       else
	         test=0;

	         fq=1000000/test;                     //?????
	   }
   }
}



