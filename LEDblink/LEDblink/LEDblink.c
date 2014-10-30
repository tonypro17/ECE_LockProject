/*
 * LEDblink.c
 *
 * Created: 10/30/2014 5:07:36 PM
 *  Author: Nick
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

int main(void)
{
	   PORTA.DIRSET = 0b00000001 ; // Set pin 0 to be output.
	   
	   while(1){ // loop forever
		   PORTA.OUTSET = 0b00000001 ; // set the output high.
		   _delay_ms( BLINK_DELAY_MS ) ; // wait.
		   PORTA.OUTCLR = 0b00000001 ; // set the output low.
		   _delay_ms( BLINK_DELAY_MS ) ; // wait.
	   }
}