/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: P3k
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_
#include "stm32l1xx.h"

void blikaj(uint16_t AD_value);
//uint16_t nacitaj(void);
void adc_init(void);
void led(void);
void irq(void);
void ADC1_IRQHandler(void);

#endif /* VRS_CV5_H_ */
