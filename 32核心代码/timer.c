#include "timer.h"
#include "key.h"
#include "delay.h"
#include "adc.h"


u32 t=0,f,T=0;
u8  TIM4CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ
//	float temp;
int time3;
void Tim2IntInit(unsigned int arr,unsigned int psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定义定时器初始化参数结构�?
	NVIC_InitTypeDef NVIC_InitStructure;			//定义嵌套向量中断结构�?
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的�? 计数�?000�?00ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频�?分频系数) 当psc�?200时，72M/7200=10Khz的计数频率（�?0k次为1s�?
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单�?
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//使能tim2的更新中�?
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先�?�?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3�?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使�?
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存�?
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
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
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


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
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


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
////////	adcx=Get_Adc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
//////		LCD_ShowxNum(90,36,adcx2,4,12,0);    //��ʾADCC�������ԭʼֵ
//////		temp=(float)adcx2*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
//////		adcx2=temp;                            //��ֵ�������ָ�adcx��������ΪadcxΪu16����
//////		LCD_ShowxNum(80,48,adcx2,1,12,0);    //��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3
//////		temp-=adcx2;                           //���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111
//////		temp*=1000;                           //С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС����
//////		LCD_ShowxNum(90,48,temp,3,12,0X80); //��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111.

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
//	//���벶�������ʼ��
//	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
//	TIM_ICInitStruct.TIM_Channel=TIM_Channel_3;
//	TIM_ICInitStruct.TIM_ICFilter=0x00;//���ɼ�
//	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
//	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
//	TIM_ICInit(TIM4,&TIM_ICInitStruct);
//	
//	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


//	TIM_Cmd(TIM4, ENABLE);	
//}



void TIM4_IRQHandler(void)
{

	if((TIM4CH1_CAPTURE_STA&0X80)==0)
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
			{	

				TIM4CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
		   		TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //����Ϊ�����ز���
	 			TIM_SetCounter(TIM4,0);

			}		
	}
	if(TIM4CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			t++;
			TIM4CH1_CAPTURE_STA=0;//������һ�β���
		}
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3|TIM_IT_Update); //����жϱ�־λ
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



