/*
 * vrs_cv5.c
 *
 *  Created on: 18. 10. 2016
 *      Author: P3k
 */

#include "vrs_cv5.h"

volatile uint16_t vstup;

void blikaj(uint16_t AD_value)
{
	int i = 0;
	for(i=0;i<(4000+50*AD_value);i++);
}

uint16_t nacitaj(void)
{
	ADC_SoftwareStartConv(ADC1);
	uint16_t vstup;
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	vstup=ADC_GetConversionValue(ADC1);
}

void adc_init(void) {
	GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Opravi a upravi
	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//.....
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_16Cycles);
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)   {   }
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1); }






void adc_irq(void){
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v       súbore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

}



void ADC1_IRQHandler(void)
{
	if(ADC1->SR & ADC_SR_EOC){	//osetrenie ci sa jedna o moje prerusenie EOC
		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){


			vstup = ADC1->DR;
			//flag EOC sa resetuje automaticky po precitani hodnoty, preto ho nemusim tu -> prerusnie ma byt rychle
			//ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
		}
	}
}
void led(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	/*Configure LED*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void USART1_IRQHandler(void)		//startup_stm32l1xx_hd.s
{
	static uint8_t buffer[]={"2.93V"};
	//	static uint8_t buffer[]={"2.93V"};
	if(USART1->SR & USART_FLAG_RXNE)
	{
		if(USART_ReceiveData(USART1) == 'm')
			USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

	if(USART1->SR & USART_FLAG_TC){
		//UART fo formáte "3.30V"
		buffer[0] = vstup/1000  + '0';
		buffer[1] = (vstup/100) % 10  + '0';
		buffer[2] = (vstup/10) % 10  + '0';
		buffer[3] = vstup % 10  + '0';
		buffer[4] = '\n';


	}

}
void Usart(void)
{

	/* PA10->RX PA9->TX */
	GPIO_InitTypeDef      GPIO_InitStructure;
	USART_Cmd(USART1, ENABLE);

}
void USART_IRQ(void){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

