#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "timer3.h"  //������Ҫ��ͷ�ļ�
#include "al_debug.h"

u8  cj_flag = 0,open_relay_flag = 0,TIMER_ENABLE_COUNT = 0;
int TIMER3_DATA = 0,open_wind_count = 1;


void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //��ʼ��NVIC�Ĵ���

//  TIM_Cmd(TIM3, ENABLE); //ʹ��TIM3
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
			if(TIMER_ENABLE_COUNT) open_wind_count++;
//			TIMER3_DATA++;
			if(open_wind_count == 5)
			{
				cj_flag = 1;
				open_wind_count = 0;
			}
//			if(open_wind_count == g_sysconfig.open_relay_time)
//			{
//				open_relay_flag = 1;
//				g_sysconfig.open_relay_time = 0; 
//				open_wind_count = 1;
//			}
		}
}




