/*
* qdec_example.c
*
* Created: 11/2/2014 10:46:24 PM
*  Author: Nick
*/
#include <avr/io.h>
#include "qdec_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lcd.h"

void lcd_setup(void);
void clearlcd(void);

#define CLOCK_DIV_bm  TC_CLKSEL_DIV64_gc
#define CLOCK_DIV     64

uint8_t lineCount = 255;

int main(void)
{
	lcd_setup();
	
	char buffer[15];
	double rotations = 0;
	double percent = 0;
	
	PORTA.DIRSET = 0b01110011;
	PORTA.PIN2CTRL = 0x30;
	PORTA.OUT |= 1<<5;
	
	//Enable interrupts
	PMIC.CTRL |= PMIC_HILVLEN_bm;
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	

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
		
		//wait for pushbutton
		while(!(PORTA.IN & 1<<2)){}
		
		//turn motor clockwise
		PORTA.OUT |= (1<<4);
		
		//run until rotations is over 2000, rotation incremented with tcc0 interrupt
		while(rotations < 2000){
			if(TCC0.INTFLAGS & TC0_CCAIF_bm) { rotations++; }
		}
		
		//stop motor, lock is now in locked state
		PORTA.OUT &= ~(1<<4);
		clearlcd();
		percent = (rotations / 2000) * 100;
		sprintf(buffer, "%d", (int)percent);
		lcd_puts(buffer);
		lcd_gotoxy(0,1);
		lcd_puts("Locked");
		
		//wait for pushbutton
		while(!(PORTA.IN & 1<<2)){}
			
		//turn motor counterclockwise
		PORTA.OUT |= (1<<6);
		while(rotations >= 0){
			if(TCC0.INTFLAGS & TC0_CCAIF_bm) { rotations--; }
		}
		
		PORTA.OUT &= ~(1<<6);
		clearlcd();
		percent = (rotations / 2000) * 100;
		sprintf(buffer, "%d", (int)percent);
		lcd_puts(buffer);
		lcd_gotoxy(0,1);
		lcd_puts("Unlocked");
		
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

