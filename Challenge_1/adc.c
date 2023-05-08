#include "stm32f303xc.h"


void initialise_ADC(void){
	// enable the clock for ADC1
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC1->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC1->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01

	ADC1->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

	// select the channel
	ADC1->SQR1 = 0; // clear any existing channel requests
	ADC1->SQR1 |= ADC_SQR1_SQ1_1; // set the request for channel 2

	// single shot mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;

	ADC1->CR |= ADC_CR_ADEN; //Enabling ADC

	while (ADC1->ISR == 0); //Wait the ADC to be ready.

	return;
}

int Read_Trim(void){
	// request the process to start
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	ADC1->CR |= ADC_CR_ADSTART;

	// Wait for the end of conversion
	while((ADC1->ISR &= ADC_ISR_EOS) != ADC_ISR_EOS);

	// read the value from ADC1->DR, full range is 12 bits (0xFFF maximum)
	uint8_t scale = ADC1->DR / (0xfff / 400);  // divide the scale into 8 even partitions (for 8 leds)

	if (scale > 7) {
		scale = 7;
	}
	int temp = 0b00000001;
	scale = scale + 1;
	temp = temp << scale;
	temp = temp - 1;
	//*led_register = pow(2, scale);
	*led_register = temp;

	return scale-1;

}
