#include "MKL46Z4.h" 
#include "button.h"
#include "stdbool.h"
#include "lcd.h"

#define PORTC_D_IRQ_NBR (IRQn_Type) 31
#define LED_RED 29
#define LED_GREEN 5
#define BUTTON_1 3
#define BUTTON_3 12
bool active_flag = 0;
extern uint16_t step_count;

void PORTC_PORTD_IRQHandler(void);

void led_init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    PORTD->PCR[LED_GREEN] = PORT_PCR_MUX(1);
    PTD->PDDR |= (1 << LED_GREEN);
	PTD->PSOR = (1<< LED_GREEN); 

    PORTE->PCR[LED_RED] = PORT_PCR_MUX(1);
    PTE->PDDR |= (1 << LED_RED);
	PTD->PCOR = (1<<LED_RED); 
}

void button_3_handler_interrupt(void)
{
	if((PTC -> PDIR & (1<<12)) == 0)
	{
		step_count = 0;
		SegLCD_DisplayDecimal(0);
	}
}

void button_1_handler_interrupt(void)
{
		if((PTC -> PDIR & (1<<3)) == 0)
		{
			active_flag = !active_flag;
			PTE->PTOR = (1 << LED_RED);
		}
		PTD->PSOR = (1<<LED_GREEN); 
}

void button_init(void)
{
	SIM->SCGC5 |= (1<<11);
	PORTC->PCR[BUTTON_1] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
	PTC->PDDR &= ~((uint32_t)(1u<<BUTTON_1)); 
	PORTC->PCR[BUTTON_1] &= ~0xF0000;
	PORTC->PCR[BUTTON_1] |= 0xA0000; 
	NVIC_ClearPendingIRQ(PORTC_D_IRQ_NBR); 
	NVIC_EnableIRQ(PORTC_D_IRQ_NBR); 

	SIM->SCGC5 |= (1<<11);
	PORTC->PCR[BUTTON_3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
	PTC->PDDR &= ~((uint32_t)(1u<<BUTTON_3)); 
	PORTC->PCR[BUTTON_3] &= ~0xF0000;
	PORTC->PCR[BUTTON_3] |= 0xA0000;
	NVIC_ClearPendingIRQ(PORTC_D_IRQ_NBR);
	NVIC_EnableIRQ(PORTC_D_IRQ_NBR); 

}
void PORTC_PORTD_IRQHandler(void)
{
	uint32_t i = 0;
	for (i=0; i<1000; i++);
	button_1_handler_interrupt();
	button_3_handler_interrupt();
	PORTC->PCR[BUTTON_1] |= PORT_PCR_ISF_MASK; 
	PORTC->PCR[BUTTON_3] |= PORT_PCR_ISF_MASK; 
}

