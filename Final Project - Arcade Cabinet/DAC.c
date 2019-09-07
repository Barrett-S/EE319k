// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "tm4c123gh6pm.h"

void DAC_Init(void){volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x02;
	delay = SYSCTL_RCGC2_R;
  GPIO_PORTB_AMSEL_R &= ~0x0F;        // 3) disable analog on PB3-0
  GPIO_PORTB_DIR_R |= 0x0F;          // 5) PF3-1 out
  GPIO_PORTB_AFSEL_R &= ~0x0F;        // 6) disable alt funct on PF3-0
  GPIO_PORTB_DEN_R |= 0x0F;          // 7) enable digital I/O on PF3-0
	GPIO_PORTB_DR8R_R |=0x0F;
	
}

void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;
}
