/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ�ֶ�ʱ��2���ܵ�ͷ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�

#ifndef _TIMER2_H
#define _TIMER2_H

extern u8  SENG_DATA,MQ2_DATA,printf_time;

void TIM2_ENABLE_30S(void);
extern void TIM2_Int_Init(u16 arr,u16 psc);
extern void TIM2_ENABLE_2S(void);

#endif
