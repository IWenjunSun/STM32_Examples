/*
 * File      : led.c
 * This is used for encoder input to TIM3
 * TIM3 support hardware level encoder input
    It has build in filter to eliminate jitter
    We register the counter count on TI1 and TI2 edge (4 count per spin)
    It can generate direction signal as well (DIR in TIMx_CR1)
 * Change Logs:
 * Date           Author       Notes
 * 2016-09-26     Wenjun Sun   the first version
 */
#include "encoder.h"

void InitializeEncoder(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	/* Encoder unit connected to TIM3, 4X mode */

	/* TIM3 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Enable GPIOA, clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA.06,07 as encoder input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);


	/* Enable the TIM1 Update Interrupt */


	/* Timer configuration in Encoder mode */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_Period = 1000;
	//TIM_TimeBaseStructure.TIM_Period = (4*ENCODER_PPR)-1;
	//TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
	TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter =10;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	// Clear all pending interrupts
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//ENC_Clear_Speed_Buffer();
	//Reset counter
	TIM3->CNT = 0;
	//CurrentCount = TIM1->CNT;
	TIM_Cmd(TIM3, ENABLE);
//	System.Device.Encoder.Enc_GetCount = Enc_GetCount;

}
