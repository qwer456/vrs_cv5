/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: P3k
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_
#include "stm32l1xx.h"

void blikaj(uint16_t AD_value);			//blikanie led
uint16_t nacitaj(void);
void adc_init(void);					//inicializacia adc
void led(void);							//inicializacia led
void adc_irq(void);						//inicializacia ADC prerusenia
void ADC1_IRQHandler(void);				//prerusenie na naciatanie vstupu-rychlost blikania led

void USART1_IRQHandler(void);			//prerusenie USART

void USART_IRQ(void);					//inicializacia USART preruseni
void Usart(void);						//GPIO USART


void ParseData(void);

#endif /* VRS_CV5_H_ */
