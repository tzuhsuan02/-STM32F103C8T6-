#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart3.h"
#include "delay.h"
#include "timer2.h"  	//������Ҫ��ͷ�ļ�
#include "timer3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "al_debug.h"

char Tx_command[8] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D};

//���ڶ���3
char usart3_arrary[512];
uint8_t usart3_index;
uint8_t usart3_data;

//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0; 


void Usart3_Clear(void)
{
	memset(s.usart3_buff,0,sizeof(s.usart3_buff));
	s.usart3_index = 0;
	s.usart3_recv = 0;
}

void USART3_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART3);
//		USART_SendData(USART1,Res);
		if (usart3_index == 0)
		{
			USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
		}
		usart3_arrary[usart3_index] = Res;
		usart3_index++;
		usart3_index %= sizeof(usart3_arrary);
	}
	else if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		usart3_data = 1;
		USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);
	}	 											 
}

//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void Usart3_init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//PB10	TXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PB11	RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	USART_InitStructure.USART_Parity = USART_Parity_No;									//��У��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
}

void Usart3_Send_Data(char *buf, u8 len)
{
	u8 t;
	for (t = 0; t < len; t++) //ѭ����������
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART3, buf[t]);
	}
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		;
}

void USART3_SendCode(u8 *DATA,u8 len)
{
	USART_ClearFlag(USART3,USART_FLAG_TC);    //����֮ǰ��շ��ͱ�־  û����һ�� �����׶��� ��һ���������׶�ʧ
	while(len--)
	{
		USART_SendData(USART3, *DATA++);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}
