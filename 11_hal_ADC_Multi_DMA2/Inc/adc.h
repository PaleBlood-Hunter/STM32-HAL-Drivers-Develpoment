/*
 * adc.h
 *
 *  Created on: Feb 5, 2024
 *      Author: vinicius.ferreira
 */

#ifndef ADC_H_
#define ADC_H_

void adc_start(void);
uint32_t pa0_adc_read(void);
void adc_pa0_single_conv_init(void);
void adc_pa0_interrupt_init(void);
void adc_pa0_dma_init(void);
void adc_pa0pa4_dma_init(void);

#endif /* ADC_H_ */
