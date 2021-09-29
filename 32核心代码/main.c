#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "adc.h"
//#include "usmart.h"  
//#include "usart2.h"  
#include "timer.h" 
#include "ov2640.h" 
#include "dcmi.h" 
#include "jidianqi.h" 
u8 ov2640_mode=0;//????:0,RGB565??;1,JPEG??

#define jpeg_buf_size 31*1024
__align(4) u32 jpeg_buf[jpeg_buf_size];
volatile u32 jpeg_data_len=0;
volatile u8 jpeg_data_ok=0;



const u16 jpeg_img_size_tbl[][2]=
{
	176,144,	//QCIF
	160,120,	//QQVGA
	352,288,	//CIF
	320,240,	//QVGA
	640,480,	//VGA
	800,600,	//SVGA
	1024,768,	//XGA
	1280,1024,	//SXGA
	1600,1200,	//UXGA
}; 
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};//7??? 
const u8*JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};//JPEG 9??? 


void jpeg_data_process(void)
{
	if(ov2640_mode)//???JPEG???,??????
	{
		if(jpeg_data_ok==0)//?????????
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//?????? 
			while(DMA_GetCmdStatus(DMA2_Stream1)!=DISABLE);//??DMA2_Stream1???  
			jpeg_data_len=jpeg_buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);//???????????
			jpeg_data_ok=1;//??JPEG??????,????????
		}
        
		if(jpeg_data_ok==2)//??jpeg????????
		{
			DMA2_Stream1->NDTR=jpeg_buf_size;	
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//?????jpeg_buf_size*4??
			DMA_Cmd(DMA2_Stream1,ENABLE);//????
			jpeg_data_ok=0;//???????
		}
	}
}


void jpeg_test(void)
{
	u32 i; 
	u8* p;
	u8 key;
	u8 effect=0,saturation=2,contrast=2;
	u8 size=3;//??QVGA 320*240??
	u8 msgbuf[15];//????? 
	LCD_Clear(WHITE);
	POINT_COLOR=RED; 
	LCD_ShowString(30,70,200,16,16,"OV2640 JPEG Mode");
	LCD_ShowString(30,100,200,16,16,"KEY0:Contrast");//???
	//LCD_ShowString(30,120,200,16,16,"KEY1:Saturation");//?????
	//LCD_ShowString(30,140,200,16,16,"KEY2:Effects");//?? 
	LCD_ShowString(30,160,200,16,16,"KEY_UP:Size");//????? 
	sprintf((char*)msgbuf,"JPEG Size:%s",JPEG_SIZE_TBL[size]);
	LCD_ShowString(30,180,200,16,16,msgbuf);//????JPEG???
	
 	OV2640_JPEG_Mode();//JPEG??
	My_DCMI_Init();//???DCMI
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//???DMA
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//??????
	DCMI_Start();//????
	
    while(1)
	{
		if(jpeg_data_ok==1)//??????????
		{  
			p=(u8*)jpeg_buf;
			LCD_ShowString(30,210,210,16,16,"Sending JPEG data...");//????????
			for(i=0;i<jpeg_data_len*4;i++)//dma??1???4??,??4
			{
				while((USART1->SR&0X40)==RESET);//??????????  		
				USART1->DR=p[i];
				key=KEY_Scan(0);
				if(key)
                    break;//??????????,?????
			}
			if(key)//?????,????
			{  
				LCD_ShowString(30,210,210,16,16,"Quit Sending data");//????????
                
				switch(key)
				{				    
					case KEY0_PRES://?????
						contrast++;
						if(contrast>4)contrast=0;
						OV2640_Contrast(contrast);
						sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-2);
						break;
					case KEY1_PRES://???Saturation
						//saturation++;
						//if(saturation>4)saturation=0;
						//OV2640_Color_Saturation(saturation);
						//sprintf((char*)msgbuf,"Saturation:%d",(signed char)saturation-2);
						break;
					case KEY2_PRES://????				 
						//effect++;
						//if(effect>6)effect=0;
						//OV2640_Special_Effects(effect);//????
						//sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
						break;
					case WKUP_PRES://JPEG??????   
						size++;  
						if(size>8)size=0;   
						OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//??????  
						sprintf((char*)msgbuf,"JPEG Size:%s",JPEG_SIZE_TBL[size]);
						break;
				}
                
				LCD_Fill(30,180,239,190+16,WHITE);
				LCD_ShowString(30,180,210,16,16,msgbuf);//??????
				delay_ms(800); 				  
			}
            else 
                LCD_ShowString(30,210,210,16,16,"Send data complete!!");//???????? 
			jpeg_data_ok=2;	//??jpeg??????,???DMA?????
		}		
	}    
} 


void jpg_test(void)
{
	  LCD_Clear(WHITE);
	  POINT_COLOR=BLACK;
	  LCD_ShowString(70,130,240,24,24,"LOADING.");
	     	 delay_ms(1000);
   	LCD_ShowString(70,130,240,24,24,"LOADING..");
		 delay_ms(1000);
	LCD_ShowString(70,130,240,24,24,"LOADING...");
	 delay_ms(2000);
   	LCD_Clear(WHITE);
	  POINT_COLOR=BLACK;
	  LCD_ShowString(70,130,240,24,24,"WELCOME");
	delay_ms(1000);
	   	LCD_Clear(GREEN);
	  POINT_COLOR=BLACK;
	  LCD_ShowString(70,130,240,24,24,"WELCOME");
	 	delay_ms(1000);
	   	LCD_Clear(WHITE);
	  POINT_COLOR=BLACK;
	  LCD_ShowString(70,130,240,24,24,"WELCOME");
	delay_ms(1000);
	   	LCD_Clear(GREEN);
	  POINT_COLOR=BLACK;
	  LCD_ShowString(70,130,240,24,24,"WELCOME");
}
void rgb565_test(void)
{ 
	u8 key;
	u8 effect=0,saturation=2,contrast=2;
	u8 scale=1;//????????
	u8 msgbuf[15];//?????
    
	LCD_Clear(WHITE);
  POINT_COLOR=BLACK; 
	//LCD_ShowString(30,70,200,16,16,"OV2640 RGB565 Mode");
	//LCD_ShowString(30,100,200,16,16,"KEY0:Contrast");//???
	//LCD_ShowString(30,130,200,16,16,"KEY1:Saturation");//?????
	//LCD_ShowString(30,150,200,16,16,"KEY2:Effects");//?? 
	//LCD_ShowString(30,170,200,16,16,"KEY_UP:FullSize/Scale");//1:1??(??????)/?????
	//delay_ms(2000);
	OV2640_RGB565_Mode();//RGB565??
	My_DCMI_Init();//DCMI??
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,1,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Disable);//DCMI DMA??  
 	OV2640_OutSize_Set(lcddev.width,lcddev.height); 
	DCMI_Start();//????
	while(1)
	{
		key=KEY_Scan(0); 
		if(key)
		{ 
			DCMI_Stop();
			switch(key)
			{				    
				case KEY0_PRES://?????
					   jpg_test();
//					contrast++;
//					if(contrast>4)contrast=0;
//					OV2640_Contrast(contrast);
//					sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-2);
					break;
				case KEY1_PRES:	//???Saturation
					//saturation++;
					//if(saturation>4)saturation=0;
					//OV2640_Color_Saturation(saturation);
					//sprintf((char*)msgbuf,"Saturation:%d",(signed char)saturation-2);
					break;
				case KEY2_PRES://????				 
					//effect++;
					//if(effect>6)effect=0;
					//OV2640_Special_Effects(effect);//????
					//sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
					break;
				case WKUP_PRES://1:1??(??????)/??	    
					scale=!scale;  
					
            if(scale==0)
					{
						OV2640_ImageWin_Set((1600-lcddev.width)/2,(1200-lcddev.height)/2,lcddev.width,lcddev.height);//1:1????
						OV2640_OutSize_Set(lcddev.width,lcddev.height); 
						sprintf((char*)msgbuf,"Full Size 1:1");
					}
            else
					{
						OV2640_ImageWin_Set(0,0,1600,1200);				//?????
						OV2640_OutSize_Set(lcddev.width,lcddev.height); 
						sprintf((char*)msgbuf,"Scale");
					}
					break;
			}
			LCD_ShowString(30,50,210,16,16,msgbuf);//??????
			delay_ms(800);
//			DCMI_Start();//??????
		} 
		delay_ms(100);	
	}    
}


int main(void)
{ 
	u8 key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//???????????2
	delay_init(168);
	uart_init(115200);
	usart2_init(42,115200);
	LED_Init();
	Adc_Init();         //≥ı ºªØADC
 	LCD_Init();
 	KEY_Init();
	jidianqi_Init();
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	//TIM3_Int_Init(10000-1,8400-1);//10Khz??,1??????
// 	usmart_dev.init(84);//???USMART
 	POINT_COLOR=BLACK;//??????? 

	while(OV2640_Init())//???OV2640
	{
		
		LCD_ShowString(30,130,240,16,16,"OV2640 ERR");
		delay_ms(200);
	  LCD_Fill(30,130,239,170,WHITE);
		delay_ms(200);
	}  
	LCD_Clear(WHITE);
	LCD_ShowString(40,130,200,16,16,"UES YOUR FACE TO OPEN");
	LCD_ShowString(50,160,230,24,24,"PLEASE TOUCH");//????????
  delay_ms(3000); 
 	while(1)
	{	
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			ov2640_mode=1;
			break;
		}
		else if(key==KEY0_PRES)
		{
			ov2640_mode=0;   
			break;
		}      
		delay_ms(5);
	}
	if(ov2640_mode)
        jpeg_test();
	        //jpg_test();
	else 
        rgb565_test();
}

