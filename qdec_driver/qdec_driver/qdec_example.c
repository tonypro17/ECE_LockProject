/*
* qdec_example.c
*
* Created: 11/2/2014 10:46:24 PM
*  Author: Nick
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "qdec_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include <util/delay.h>

void lcd_setup(void);
void clearlcd(void);
bool EVSYS_SetEventSource( uint8_t eventChannel, EVSYS_CHMUX_t eventSource );

#define CLOCK_DIV_bm  TC_CLKSEL_DIV64_gc
#define CLOCK_DIV     64

uint8_t lineCount = 255;
uint16_t rotations = 0;
uint16_t percent;
char buffer[15];

// Interrupt Handler for door unlock signal
ISR(PORTA_INT0_vect)
{
	
	//Determine direction of motor by switch state
	if(PORTA.IN & 1<<2){
		//turn motor clockwise
		PORTA.OUT |= (1<<6);
		PORTA.OUT &= ~(1<<4);
	}
	else if(!(PORTA.IN & 1<<2)) {
		//turn motor counter-clockwise
		PORTA.OUT &= ~(1<<6);
		PORTA.OUT |= (1<<4);
	}
	
}

// Interrupt Handler to stop motor on overflow
ISR(TCC0_CCA_vect)
{
	
	
	if(TCC0.CTRLFCLR & 1<<0){
		rotations++;
		
		if(rotations >= 250){
			PORTA.OUT &= ~(1<<4);
			PORTA.OUT &= ~(1<<6);
		}
		
	}
	if(!(TCC0.CTRLFCLR & 1<<0)){
		rotations--;
		
		if(rotations <= 0){
			PORTA.OUT &= ~(1<<4);
			PORTA.OUT &= ~(1<<6);
		}
	}
	
}

//LCD Interrupt Handler
ISR(TCE0_CCA_vect)
{
	clearlcd();
	//percent = (rotations / 2500)*100;
	sprintf(buffer, "%d", (int)rotations);
	lcd_puts(buffer);
}

int main(void)
{
	lcd_setup();
	
	//double rotations = 0;
	//double percent = 0;
	//double rotationtotal = 0;
	
	//Enable interrupts
	SREG |= (1<<7);
	PMIC.CTRL |= PMIC_HILVLEN_bm;
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	//Set up Port A inputs / outputs
	PORTA.DIRSET = 0b01110011;
	PORTA.OUT |= 1<<5;
	PORTA.OUT &= ~(1<<6);
	PORTA.OUT &= ~(1<<4);
	
	//Configure PA2 as input, sense on both edges
	PORTA.PIN2CTRL |= PORT_ISC_BOTHEDGES_gc;
	//Configure Interrupt 0 on Port A Pin 2
	PORTA.INTCTRL = (PORTA.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_HI_gc;
	PORTA.INT0MASK = (1<<2);
	
	//LCD Interrupt Prep
	TCE0.PER = 1000;
	TCE0.CTRLA = TC_CLKSEL_DIV256_gc;
	TCE0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_NORMAL_gc;
	TCE0.INTCTRLB = TC_CCAINTLVL_LO_gc;
	TCE0.CCA = 1000;
	
	
	/* Setup PORTD with pin 0 as input for QDPH0, don't invert IO pins.
	*
	* Setup event channel 0, pin 0 on PORTD as input, don't enable index.
	* if index used then state 00 is set as the index reset state.
	*
	* Setup TCC0 with Event channel 0 and lineCount.
	*/
	QDEC_Total_Setup(&PORTD,                    /*PORT_t * qPort*/
	0,                         /*uint8_t qPin*/
	false,                     /*bool invIO*/
	0,                         /*uint8_t qEvMux*/
	EVSYS_CHMUX_PORTD_PIN0_gc, /*EVSYS_CHMUX_t qPinInput*/
	false,                     /*bool useIndex*/
	EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
	&TCC0,                     /*TC0_t * qTimer*/
	TC_EVSEL_CH0_gc,           /*TC_EVSEL_t qEventChannel*/
	lineCount);                /*uint8_t lineCount*/
	
	// initialize system, initial state is unlocked
	clearlcd();
	//position = GetCaptureValue(TCC0);
	//sprintf(buffer, "%d", position);
	lcd_puts("0%");
	lcd_gotoxy(0,1);
	lcd_puts("Unlocked");
	
	//loop forever
	while(1){
		
		//Clear interrupt flag when new value is captured
		if(TCC0.INTFLAGS & TC0_CCAIF_bm){
			TCC0.INTFLAGS |= TC0_CCAIF_bm;
			
		}
		
		//wait for pushbutton
		//while(!(PORTA.IN & 1<<2)){}
		
		/*
		for(int i=0;i<100;i++){
			//turn motor clockwise
			PORTA.OUT |= (1<<4);
			
			//run until rotations is over 20, rotation incremented with tcc0 interrupt
			while(rotations < 25){
				if(TCC0.INTFLAGS & TC0_CCAIF_bm) {	
					rotations++;
				}
			}
			rotationtotal += rotations;
			rotations = 0;
			PORTA.OUT &= ~(1<<4);
			clearlcd();
			percent = (rotationtotal / 2500) * 100;
			sprintf(buffer, "%d", (int)percent);
			lcd_puts(buffer);
			_delay_ms(500);
		}
		
		lcd_gotoxy(0,1);
		lcd_puts("Locked");
		
		//wait for pushbutton
		while(!(PORTA.IN & 1<<2)){}
		
		for(int i=0;i<100;i++){
			
			//run until rotations is over 20, rotation incremented with tcc0 interrupt
			while(rotations < 25){
				if(TCC0.INTFLAGS & TC0_CCAIF_bm) {
					rotations++;
				}
			}
			rotationtotal -= rotations;
			rotations = 0;
			PORTA.OUT &= ~(1<<6);
			clearlcd();
			percent = (rotationtotal / 2500) * 100;
			sprintf(buffer, "%d", (int)percent);
			lcd_puts(buffer);
			_delay_ms(500);
		}
		
		lcd_gotoxy(0,1);
		lcd_puts("Unlocked");
		*/
	}
}

void lcd_setup(void)
{
	/* initialize display, cursor off */
	lcd_init(LCD_DISP_ON);
	
	/* put string to display (line 1) with linefeed */
	lcd_puts("test");

	/* turn off cursor */
	lcd_command(LCD_DISP_ON);
}

void clearlcd(void)
{
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("0123");
	return;
}

bool EVSYS_SetEventSource( uint8_t eventChannel, EVSYS_CHMUX_t eventSource )
{
	volatile uint8_t * chMux;

	/*  Check if channel is valid and set the pointer offset for the selected
	 *  channel and assign the eventSource value.
	 */
	if (eventChannel < 8) {
		chMux = &EVSYS.CH0MUX + eventChannel;
		*chMux = eventSource;

		return true;
	} else {
		return false;
	}
}


