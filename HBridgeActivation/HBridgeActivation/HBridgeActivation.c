/*
 * HBridgeActivation.c
 *
 * Created: 10/14/2014 4:40:31 PM
 *  Author: Nick
 */ 


#include <avr/io.h>

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
	 *	PD7
	 
	 Each motor is controlled by setting to High "ENA"
	 or "ENB" for the corresponding motor. You can then
	 control the direction of each motor by enabling "IN1"
	 or "IN2" for motor A and "IN3" or "IN4" for motor B
	 
	 To "brake" a particular motor, you will need to set
	 "ENA" or "ENB" to High and then set both directional
	 pins (IN*) to either high or low. 
	 
	 */
	
    while(1)
    {
		//If "open door" signal is received, turn
		if(PD7){
			PD3 = 1;	//ENA = HIGH
			PD4 = 1;	//IN1 = HIGH
			PD5 = 0;	//IN2 = LOW
			
			
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
			PD3 = 1;	//ENA = HIGH
			PD4 = 1;	//IN1 = HIGH
			PD5 = 1;	//IN2 = HIGH
		}
		
    }
}