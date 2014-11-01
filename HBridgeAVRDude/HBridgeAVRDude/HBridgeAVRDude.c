/*
 * HBridgeAVRDude.c
 *
 * Created: 10/14/2014 4:40:31 PM
 *  Author: Nick
 */ 


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/*
	 *	GPIO Pins:
	 *	PD3 ->ENA
	 *	PD4 ->IN1
	 *	PD5 ->IN2
	 *	PD6 -> Currently unused
	 *	
	 *	Turn Signal Pin:
	 *	PD0
	 
	 Each motor is controlled by setting to High "ENA"
	 or "ENB" for the corresponding motor. You can then
	 control the direction of each motor by enabling "IN1"
	 or "IN2" for motor A and "IN3" or "IN4" for motor B
	 
	 To "brake" a particular motor, you will need to set
	 "ENA" or "ENB" to High and then set both directional
	 pins (IN*) to either high or low. 
	 
	 */
	
	
	//Set PD0 as input, and PD5, PD4, PD3 as output
	PORTD.DIRSET = 0b00011100;
	PORTD.PIN0CTRL = 0x30;
	
    while(1)
    {
		//If "open door" signal is received, turn
		if(PORTD.IN & (PORTD.IN & 1)){
			PORTD.OUTCLR = 0b11111111;	//Clear whatever is there
			PORTD.OUTSET = 0b00001100;	//ENA = HIGH...IN1 = HIGH...IN2 = LOW	
		/*
			For future....
			
			Based on encoder feedback, 
			wait until deadbolt completely opens,
			then enter brake state.
			
			*INTERRUPT?*
		*/
		
		}
		//Rest in "brake" state
		else{
			PORTD.OUTCLR = 0b11111111;
			PORTD.OUTSET = 0b00011100;	//ENA = HIGH //IN1 = HIGH //IN2 = HIGH
		}
		
    }
}