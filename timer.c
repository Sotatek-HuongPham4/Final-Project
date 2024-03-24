#include "mma8451q.h"

int32_t volatile msTicks = 0; 
int32_t volatile msTicksInDelay = 0;
unsigned long long time = 0;	
//	SYSTICK
uint32_t SysTick_Configuration (uint32_t ticks){
	SysTick->LOAD  = ticks -1;                 
	NVIC_SetPriority(SysTick_IRQn, (1<<4) -1); 
	SysTick->VAL   = 0;                        
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk|
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk;                
	return 0;                             
}

void init_SysTick_interrupt(void){
	SysTick->LOAD = SystemCoreClock/ 1000; 
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
}

long long SysTick_Handler(void) { 
	msTicks++;  
	msTicksInDelay++;
	if(msTicks == 1000){
		time++;
		msTicks = 0;
	}
	return time;
}
void Delay (uint32_t ticks) { 
	msTicksInDelay = 0;
	while (msTicksInDelay < ticks); 
	msTicksInDelay = 0; 
}
