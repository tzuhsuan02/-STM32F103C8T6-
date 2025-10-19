/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ�ֶ�ʱ��1���ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "timer1.h"
#include "al_debug.h"

u8 check_time = 0;
u8 tft_reflash_falg = 0,tft_reflash_time = 0;

volatile unsigned long    g_Timer1[MAX_TIMER1];

/*-------------------------------------------------*/
/*����������ʱ��1ʹ��1���Ӷ�ʱ                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_60S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //ʹ��TIM1ʱ��	
	
  TIM_DeInit(TIM1);                                             //��ʱ��1�Ĵ����ָ�Ĭ��ֵ
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=2-1;          //���ö�ʱ���ظ�����2�Σ�TIM1��8 ����������ܣ�
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //����TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //ʹ��TIM1����ж�    
                       	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //����TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
	
	TIM_Cmd(TIM1,ENABLE);                                         //��TIM1   
}


//TIM1_Init(2000-1, 36000-1); // ��ʱ����1s
void TIM1_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* ��ʱ��TIM1��ʼ�� */
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    /* �ж�ʹ�� */
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    /* �ж����ȼ�NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM1, ENABLE);

// ȫ�ֶ�ʱ����ʼ��
    for(int i = 0; i < MAX_TIMER1; i++)
    {
        g_Timer1[i] = 0;
    }
}

/********************************************************************************************************
** ����: TIM1_IRQHandler,  ��ʱ��1�жϷ������
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��
********************************************************************************************************/
void TIM1_UP_IRQHandler(void)   //TIM1�ж�
{
    uint8_t i;

    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)   // ���TIM1�����жϷ������
    {
        //-------------------------------------------------------------------------------
        // ���ֶ�ʱ������ʱ
        for(i = 0; i < MAX_TIMER1; i++)      // ��ʱʱ��ݼ�
            if(g_Timer1[i]) g_Timer1[i]-- ;
				
				
				if(tft_reflash_falg == true)
				{
					tft_reflash_time ++;
				}
				else
					tft_reflash_time = 0;
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);     //���TIMx�����жϱ�־
    }
}

//void PWM_Init(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//	
//	//��ʱ����ʼ��
//	TIM_TimeBaseStruct.TIM_Period=9999;//��ֵ
//	TIM_TimeBaseStruct.TIM_Prescaler=143;//Ԥ��Ƶ
//	TIM_TimeBaseStruct.TIM_ClockDivision=0;
//	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//����
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStruct);
//	
//	//pwm ��ʼ��
//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
//	
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM4,ENABLE);

//	TIM_SetCompare3(TIM4,1200);
//}

void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	//��ʱ����ʼ��
	TIM_TimeBaseStruct.TIM_Period=9999;//��ֵ
	TIM_TimeBaseStruct.TIM_Prescaler=143;//Ԥ��Ƶ
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//����
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);
	
	//pwm ��ʼ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3,ENABLE);

	TIM_SetCompare3(TIM3,1200);
}

//��ʱ��3�������ƶ��
void TIM3_PWM5_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	
}

