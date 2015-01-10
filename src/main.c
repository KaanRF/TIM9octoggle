#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

//DEÐÝSKENLERÝMÝZ
__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 20480;
uint16_t PrescalerValue = 0;

void Tim9_Init(void);

int main(void)
{
	Tim9_Init();
	PrescalerValue = (uint16_t) (SystemCoreClock / 15000000) - 1;
	TIM_TimeBaseStructure.TIM_Prescaler=PrescalerValue;
	TIM_TimeBaseStructure.TIM_Period=65535;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);

	//KANAL1 COMPARE OUT MODE AYARLARI(OC=OUTPUT COMPARE)
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=CCR1_Val;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_Low;
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Disable);
	//KANAL2 OC AYARLAMALARI
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=CCR2_Val;
	TIM_OC2Init(TIM9,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM9,TIM_OCPreload_Disable);

	 /* TIM enable counter */
	  TIM_Cmd(TIM9, ENABLE);
	  /* TIM IT enable */
	  TIM_ITConfig(TIM9, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

	 while (1)
	 {

	 }

}

void Tim9_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//TIMER 9 YOLU AKTIF EDILIYOR

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	//A PORTU YOLU AKTIF EDILIYOR
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	 /* GPIOA PORT BÝLGÝSÝ: TIM9 CH1 (PA2) and TIM9 CH2 (PA3) */

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //TIMER BACAKLARININ YONLENDIRILMESÝ DATASHEETTEN BAKIYORUZ AF3 E BAGLI TIMER9

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9);

    //TIMER 9 ICIN GENEL KESMELER

    NVIC_InitStructure.NVIC_IRQChannel= TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
    return 0; // ses çipini kullanýyorsanýz tek sample veriyi burada return ile döndürün.
}
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
    return; // ses çipini kullanýyorsanýz  burada çipe veri aktarýmý DMA sona ermiþ
}

