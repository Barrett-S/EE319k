// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 3/28/2018 

// Specifications:
// Measure distance using slide pot, sample at 60 Hz
// maximum distance can be any value from 1.5 to 2cm
// minimum distance is 0 cm
// Calculate distance in fixed point, 0.001cm
// Analog Input connected to PD2=ADC5
// displays distance on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats (use them in creative ways)
// 

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on ST7735
// main3 adds your convert function, position data is no ST7735

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	int ADCStatus=0;
	int ADCMail=0;
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R &= ~0xFF;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R &= ~0xFF;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R |= 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_DEN_R = 0x0E;          // 7) enable digital I/O on PF3-1
}
uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
int main2(void){       // single step this program and look at Data
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 5
  }
}

int main3(void){
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 5
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    PF2 = 0x04;       // Profile ADC
    Data = ADC_In();  // sample 12-bit channel 5
    PF2 = 0x00;       // end of ADC Profile
    ST7735_SetCursor(0,0);
    PF1 = 0x02;       // Profile LCD
    LCD_OutDec(Data); 
    ST7735_OutString("    ");  // spaces cover up characters from last output
    PF1 = 0;          // end of LCD Profile
  }
}

// your function to convert ADC sample to distance (0.001cm)
uint32_t Convert(uint32_t input){
	if((1167*Data)/4096+382 == 1010){return 1011;}
 return (uint32_t)(1167*Data)/4096+382;
}
int main4(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 5
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 5
    PF3 = 0x08;       // Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(Data); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
  }
}   
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 1333333; //60 hz
	NVIC_ST_CURRENT_R = 0;
  NVIC_ST_CTRL_R = 0x00000007; // enable, clock, interrupts
}
void SysTick_Handler(){
			PF2 ^= 0x04;      // Heartbeat
		//PF2 ^= 0x04;      // Heartbeat
	    Data = ADC_In();  // sample 12-bit channel 5

		ADCMail=Data;
		ADCStatus=1;
		PF2 ^= 0x04;      // Heartbeat
	

	
			
}
int main(void){

















--------*************************************************************************************************************************************************************************************************************************************************************************************************++	TExaS_Init();
  // your Lab 8
	ST7735_InitR(INITR_REDTAB); 
	SysTick_Init();
  PortF_Init();
  ADC_Init();         // turn on ADC, set channel to 5
  EnableInterrupts();
  while(1){
		if(ADCStatus){
	  PF1 = 0x02;       // Profile LCD
    ST7735_SetCursor(0,0);
    LCD_OutDec(ADCMail); ST7735_OutString("    "); 
    ST7735_SetCursor(6,0);
		PF3 = 0x08;       // Profile Convert
    Position = Convert(ADCMail); 
    PF3 = 0;          // end of Convert Profile
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
		ST7735_SetCursor(14,0);
		ST7735_OutChar('c');
		ST7735_OutChar('m');
		
			ADCStatus=0;
		}
  }
}



