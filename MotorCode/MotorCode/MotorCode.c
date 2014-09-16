/*
 * MotorCode.c
 *
 * Created: 8/18/2014 4:38:59 PM
 *  Author: Nick
 */ 


#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	/*
	REGISTER DESCRIPTIONS PAGE 67 in data sheet
	*/
	
	
	/*Data Direction Registers
	
	^ All pins set as outputs
	
	*/
	
	DDRB |= (1<<PB2);
	
	
	/*Timer/Counter Control Registers
	
	^Set OC0A / OCOB on Compare Match when up-counting. 
	^Clear OC0A / OCOB on compare Match when down-counting.
	
	^SETTING IT TO WGM02:0 = 5, for Phase Correct PWM
	
	^WGM00 <------ Bit 0 of TCCR0A
	^WGM01 <------ Bit 1 of TCCR0A
	^WGM02 <------ Bit 3 of TCCR0B
	
	clkIO (no prescaling as of now)
	
	toggle OC0A on compare match.
	*/
	
	TCCR0A = (0<<COM0A1) | (1<<COM0A0) | 
			 (0<<COM0B1) | (0<<COM0B0) | 
			 (0>>WGM01) | (1<<WGM00);
	
	TCCR0B = (1<<WGM02) | (0<<CS02) |
			 (0<<CS01) | (1<<CS00);

	/*
		BOTTOM -> Counter reaches 0x00
		MAX ----> Counter reaches 0xFF
		TOP ----> Counter reaches either 0xFF or OCROA register value
	*/	
	

	while (1)
	{
		//Output Compare Registers
		OCR0A = 0x1F;
	}
}