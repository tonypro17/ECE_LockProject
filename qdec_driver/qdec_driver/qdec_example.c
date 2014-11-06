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
	int position;
	int rotations = 0;
	
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
	
	
	clearlcd();
	position = GetCaptureValue(TCC0);
	sprintf(buffer, "%d", position);
	lcd_puts("what");
		
	
	PORTA.OUT |= (1<<4);
	while(1){
		if((TCC0.INTFLAGS & TC0_CCAIF_bm) != 0){
			PORTA.OUT &= ~(1<<4);
			clearlcd();
			position = GetCaptureValue(TCC0);
			sprintf(buffer, "%d", position);
			lcd_puts(buffer);
			while(1){}
		}
	}
	
	
	
	/*
	while(1) {
		position = GetCaptureValue(TCC0);
		if((TCC0.INTFLAGS & TC0_CCAIF_bm) != 0){
			
			rotations++;
			clearlcd();
			position = GetCaptureValue(TCC0);
			sprintf(buffer, "%d", position);
			lcd_puts(buffer);
			lcd_gotoxy(0,1);
			sprintf(buffer, "%d", rotations);
			lcd_puts(buffer);
			//for(int i=0; i < 100000; i++){}
		}
	}
	*/
	
	/*
	while(1){
		
		while(!(PORTA.IN & 1<<2)){}
		
		PORTA.OUT |= (1<<4);
		for(int i=0;i<40;i++)
		{
			//position = GetCaptureValue(TCC0);
			//clearlcd();
			//sprintf(buffer, "%d", position);
			//lcd_puts(buffer);
			lcd_gotoxy(0,1);
			lcd_puts("locked");
		}
		PORTA.OUT &= ~(1<<4);
		
		while(!(PORTA.IN & 1<<2)){}
		
		PORTA.OUT |= (1<<6);
		for(int i=0;i<40;i++)
		{
			//position = GetCaptureValue(TCC0);
			//clearlcd();
			//sprintf(buffer, "%d", position);
			//lcd_puts(buffer);
			//lcd_gotoxy(0,1);
			lcd_puts("unlocked");
		}
		PORTA.OUT &= ~(1<<6);
		
	}*/

	//while(1){
	//for(int i=0;i<10000;i++){}
	//position = GetCaptureValue(TCC0);
	//if(position==1000)
	//{
	//rotations++;
	//}
	//clearlcd();
	//sprintf(buffer, "%d", position);
	//lcd_puts(buffer);
	//lcd_gotoxy(0,1);
	//sprintf(buffer, "%d", rotations);
	//lcd_puts(buffer);
	//}
	//PORTA.OUT &= ~(1<<4);
	
	//while(1){
	//
	//if((PORTA.IN & (1<<2)) && position<=0)
	//{
	//PORTA.OUT |= (1<<4);
	//while(GetCaptureValue(TCC0)<1000)
	//{
	//clearlcd();
	//sprintf(buffer, "%d", position);
	//lcd_puts(buffer);
	//position = GetCaptureValue(TCC0);
	//}
	//PORTA.OUT &= ~(1<<4);
	//clearlcd();
	//sprintf(buffer, "%d", position);
	//lcd_puts(buffer);
	//lcd_gotoxy(0,1);
	//lcd_puts("locked");
	//}
	//
	//if ((PORTA.IN & (1<<2)) && position>=10000)
	//{
	//PORTA.OUT |= (1<<6);
	//while(GetCaptureValue(TCC0)>0)
	//{
	//clearlcd();
	//sprintf(buffer, "%d", position);
	//lcd_puts(buffer);
	//position = GetCaptureValue(TCC0);
	//}
	//PORTA.OUT &= ~(1<<6);
	//clearlcd();
	//sprintf(buffer, "%d", position);
	//lcd_puts(buffer);
	//lcd_gotoxy(0,1);
	//lcd_puts("unlocked");
	//}
	//
	//}
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
	return;
}

