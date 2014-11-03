/*
 * qdec_example.c
 *
 * Created: 11/2/2014 10:46:24 PM
 *  Author: Nick
 */ 
#include <avr/io.h>
#include "qdec_driver.h"

//#define GENERATE_TEST_SIGNAL

uint8_t freq = 60;

bool    dir  = 0;

#define CLOCK_DIV_bm  TC_CLKSEL_DIV64_gc
#define CLOCK_DIV     64

uint8_t lineCount = 30;

int main(void)
{
	/* Set up port C as output. */
	PORTC.DIRSET = 0xFF;

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
	
	
	/* Display the angle of rotation on LCD...hooked up to PORT C */
	while (1) {
		PORTC.OUT = GetCaptureValue(TCC0);
	}
}
