/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���1���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�



#define true 1
#define false 0

typedef struct 
{
	char usart_buff[512];
	char usart_index,usart_recv;
	
	char usart3_buff[512];
	char usart3_index,usart3_recv;
	
}Serial;

extern Serial s;
void Usart_Clear(void);


#define USART1_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART1_TXBUFF_SIZE   256    //���崮��1 ���ͻ�������С 256�ֽ�

#if  USART1_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART1_RXBUFF_SIZE   256               //���崮��1 ���ջ�������С 256�ֽ�
extern char Usart1_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart1_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif

void Usart1_Init(unsigned int);     //����1 ��ʼ������
void u1_printf(char*,...) ;         //����1 printf����

extern void USART3_init(u32 bound);

#endif


