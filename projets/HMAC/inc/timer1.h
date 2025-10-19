/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ�ֶ�ʱ��1���ܵ�ͷ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"
#include "sys.h"

#ifndef _TIMER1_H
#define _TIMER1_H

void TIM1_ENABLE_60S(void);

#define  MAX_TIMER1            4           				// ���ʱ������
extern volatile unsigned long    g_Timer1[MAX_TIMER1];
#define  SendTimer1        g_Timer1[0]  							// LED��ת��ʱ��
#define  DebugTimer1  		g_Timer1[1]  							// �¶Ȳɼ���ʱ��
#define  CheckTimer1  			g_Timer1[2]  							// �Ųɼ���ʱ��


#define  TIMER1_SEC				(1)              // ��
#define  TIMER1_MIN				(TIMER1_SEC*60)  // ��
#define	 TIMER1_HUOR			(TIMER1_MIN*60)	 // ʱ
#define	 TIMER1_DAY				(TIMER1_HUOR*24)	 // ʱ

extern u8 tft_reflash_falg,tft_reflash_time;

extern void TIM1_Init(uint16_t arr, uint16_t psc);
extern void TIM3_PWM5_Init(u16 arr,u16 psc);
void PWM_Init(void);

#endif
