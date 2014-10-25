/*
 * QuadDecoder.c
 *
 * Created: 10/18/2014 1:10:44 PM
 *  Author: Nick
 */ 


#include <avr/io.h>

int main(void)
{
    while(1)
    {
		/*
		1. Configure PD0 and PD1 as inputs
		2. Configure PD0 and PD1 input sense control register to level sensing (transparent for events).
		3. Select PD0 as multiplexer input for event channel 0.
		4. Optional for index:
		a. Configure PD2 as input.
		b. Configure PD2 input sense control register to sense both edges.
		c. Select PD2 as multiplexer input for event channel 1.
		d. Set the Quadrature Index Enable bit in event channel 0.
		e. Select the Index Recognition mode for channel 0.
		5. Enable quadrature decoding and digital filtering in event channel 0.
		6. Set Quadrature decoding as event action for TCC0.
		7. Select event channel 0 as event source for TCC0.
		8. Set the period register of TCC0 to (n * 4 – 1), where n is the line count of the
		quadrature encoder.
		9. Enable TCC0 by setting CLKSEL to a CLKSEL_DIV1.
		*/
		
        //TODO:: Please write your application code 
    }
}