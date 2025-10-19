/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���1���ܵ�Դ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�

extern u8 sys_connet_aliyun_flag;

Serial s;

#if  USART1_RX_ENABLE                   //���ʹ�ܽ��չ���
char Usart1_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ�������
unsigned int Usart1_RxCounter = 0;      //����һ����������¼����1�ܹ������˶����ֽڵ�����
char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��1���յ�������
#endif

/*-------------------------------------------------*/
/*����������ʼ������1���͹���                      */
/*��  ����bound��������                            */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart1_Init(unsigned int bound)
{
	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//PA9	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_9;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA10	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = bound;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usart_initstruct.USART_Parity = USART_Parity_No;									//��У��
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART1, &usart_initstruct);
	
	USART_Cmd(USART1, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvic_initstruct.NVIC_IRQChannel = USART1_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic_initstruct);
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
	{		
		if(s.usart_recv == false)
		{
			USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		}				
		
		if(s.usart_index >= sizeof(s.usart_buff))	s.usart_index = 0; //��ֹ���ڱ�ˢ��
		
		s.usart_buff[s.usart_index++] = USART1->DR;
		s.usart_index %= sizeof(s.usart_buff);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	
	{
		s.usart_recv = true;
		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);//�ر�
	}
}


void Usart_Clear(void)
{
    memset(s.usart_buff, 0, sizeof(s.usart_buff));
    s.usart_index = 0;
    s.usart_recv = 0;
}

/*-------------------------------------------------*/
/*������������1 printf����                         */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���    */
/*����ֵ����                                       */
/*-------------------------------------------------*/

__align(8) char Usart1_TxBuff[USART1_TXBUFF_SIZE];

void u1_printf(char* fmt, ...)
{
    unsigned int i, length;

    va_list ap;
    va_start(ap, fmt);
    vsprintf(Usart1_TxBuff, fmt, ap);
    va_end(ap);

    length = strlen((const char*)Usart1_TxBuff);
    while((USART1->SR & 0X40) == 0);
    for(i = 0; i < length; i ++)
    {
        USART1->DR = Usart1_TxBuff[i];
        while((USART1->SR & 0X40) == 0);
    }
}



