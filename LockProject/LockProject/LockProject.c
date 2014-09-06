/*
 * LockProject.c
 *
 * Created: 8/18/2014 7:41:30 PM
 */ 


#include <avr/io.h>

#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>

int main(void)
{
	
	DDRB |= (1<<PB0); // LED on PB0
	
	while(1) {
		PORTB |= (1<<PB0); // Turn LED on
		_delay_ms(500);
		PORTB &= ~(1<<PB0); // Turn LED off
		_delay_ms(500);
	}

	return 1;
}