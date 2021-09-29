#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "sys.h" 
#include "delay.h"

uint16_t ack_count = 0;

extern __IO uint16_t USART2_RX_STA;
extern uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
extern uint8_t  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

void esp8266_start_trans(void)
{
	//��ԭģ��Ϊ��������
	esp8266_send_cmd("AT+RESTORE\r\n","OK",500);
	delay_ms(700);         //��ʱ3S�ȴ����óɹ�
	//����Ϊstationģʽ
	esp8266_send_cmd("AT+CWMODE=1\r\n","OK",300);
  delay_ms(100); 
	//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	esp8266_send_cmd("AT+CWJAP_DEF=\"mix3\",\"zhuzhengyu1123\"\r\n","GOT",3000); 
    //APģʽ
	delay_ms(5500);
	esp8266_send_cmd("AT+CIFSR\r\n","OK",300);
	
	esp8266_send_cmd("AT+CIPMUX=1\r\n","OK",300);
	esp8266_send_cmd("AT+RST\r\n","OK",3000);
	esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.43.1\",8001\r\n","OK",300);
}
//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
uint8_t esp8266_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART2_RX_STA=0;
	printf("����ָ��֮ǰ\r\n");
	//u2_printf("%s",cmd);	//��������
	//while(HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);
	HAL_UART_Transmit(&huart2,cmd,sizeof(cmd)-1,1000);
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	   //�ȴ�����ʱ
		{
			printf("%d",ack_count++);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				printf("��ʼ���Ӧ��");
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(uint8_t*)ack);
					break;//�õ���Ч���� 
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0){res=1; printf("δ���ܵ�Ӧ��\r\n");}		
	}
	ack_count = 0;  //

	printf("\r\n");
	return res;
} 
//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
uint8_t* esp8266_check_cmd(uint8_t *str)
{
	uint8_t *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	printf("%s",strx);
	return (uint8_t*)strx;
}



void ESP01_Getweb(void)
{
    delay_ms(50);
    
    printf("AT+CWMODE=1");
    delay_ms(50);
    RST_ON;
    delay_ms(100);
    RST_OFF;
    delay_ms(50);
    
    USART_RX_STA2=0x0000;
    printf("AT+CWJAP=\"WIFI name\",\"password\"");
    delay_ms(1000);  //ע��delayʱ������
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
}

void ESP01_Gettime(void)
{
    printf("AT+CIPMUX=0\r\n");
    delay_ms(500);
    
    printf("AT+CIPSTART=\"TCP\",\"api.k780.com\",80\r\n");
    delay_ms(1600);
    
    printf("AT+CIPMODE=1\r\n");
    delay_ms(500);
    
    printf("AT+CIPSEND\r\n");
    delay_ms(500);
    USART_RX_STA2=0x0000;  //ʹ���ݴ�����ͷ��ʼ��¼
    printf("GET http://api.k780.com:88/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json&HTTP/1.1\r\n");
    delay_ms(1600);
}


