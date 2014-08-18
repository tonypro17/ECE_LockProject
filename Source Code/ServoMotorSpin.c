/*
 * ServoMotorSpin.c
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
	
	/*
	These 3 bits set Waveform Generation Mode
	for PWM...Table 11-8, page 83 in data sheet

	WGM00 <------ Bit 0 of TCCR0A
	WGM01 <------ Bit 1 of TCCR0A
	WGM02 <------ Bit 3 of TCCR0B
	
	SETTING IT TO WGM02:0 = 5, for Phase Correct PWM
	*/
	
	//Data Direction Registers
	DDRA |= 0xFF;
	DDRB |= 0xFF;
	
	//Output Compare Registers
	OCR0A = 0xFF;
	OCR0B = 0x7F;
	
	//Timer/Counter Control Registers
	TCCR0A = (1<<COM0A1) | (1<<COM0A0) | 
			 (1<<COM0B1) | (1<<COM0B0) | 
			 (0>>WGM01) | (1<<WGM00);
	
	TCCR0B = (1<<WGM02) | (1<<CS02) |
			 (1<<CS01) | (1<<CS00);

	/*
		BOTTOM -> Counter reaches 0x00
		MAX ----> Counter reaches 0xFF
		TOP ----> Counter reaches either 0xFF or OCROA register value
	*/	
	

	while (1)
	{
	}
}