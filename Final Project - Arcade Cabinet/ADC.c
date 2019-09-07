// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void ADC_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x08;
	while((SYSCTL_PRGPIO_R&0x08) == 0){};
	GPIO_PORTD_DIR_R &= ~0x04;
	GPIO_PORTD_AFSEL_R |= 0x04;
	GPIO_PORTD_DEN_R &= ~0x04;
	GPIO_PORTD_AMSEL_R |= 0x04;
	SYSCTL_RCGCADC_R |= 0x01;
	delay = SYSCTL_RCGCADC_R;
	delay++;
	delay++;
	delay++;
	delay++;
	delay++;
	ADC0_PC_R = 0x01;
	ADC0_SSPRI_R = 0x0123;
	ADC0_ACTSS_R &=~0x08;
	ADC0_EMUX_R &= ~0xF000;
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0) + 5;
	ADC0_SSCTL3_R = 0x006;
	ADC0_IM_R &= ~0x0008;
	ADC0_ACTSS_R |= 0x0008;

}

int result=0;
uint32_t ADC_In(void){  
ADC0_PSSI_R = 0x0008;
while((ADC0_RIS_R&0x08)==0){};
	result=ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
	return result;

}
