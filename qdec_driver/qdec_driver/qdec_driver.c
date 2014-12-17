/*
 * qdec_driver.c
 *
 * Created: 11/2/2014 8:26:52 PM
 *  Author: Nick
 */ 
#include <avr/io.h>
#include "qdec_driver.h"

bool QDEC_EVSYS_Setup(uint8_t qEvMux, 
					  EVSYS_CHMUX_t qPinInput,
					  bool useIndex,
					  EVSYS_QDIRM_t qIndexState)
{
	switch (qEvMux){
			case 0:
	        /* Configure event channel 0 for quadrature decoding of pins. */
		    EVSYS.CH0MUX = qPinInput;
	        EVSYS.CH0CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_2SAMPLES_gc;
		    if(useIndex){
			        /*  Configure event channel 1 as index channel. Note
			         *  that when enabling Index in channel n, the channel
			         *  n+1 must be configured for the index signal.*/
			         EVSYS.CH1MUX = qPinInput + 2;
			         EVSYS.CH1CTRL = EVSYS_DIGFILT_2SAMPLES_gc;
			         EVSYS.CH0CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;
			                     
		    }
		    break;
		    case 2:
		    EVSYS.CH2MUX = qPinInput;
		    EVSYS.CH2CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_2SAMPLES_gc;
		    if(useIndex){
			         EVSYS.CH3MUX = qPinInput + 2;
			         EVSYS.CH3CTRL = EVSYS_DIGFILT_2SAMPLES_gc;
			         EVSYS.CH2CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;
		    }
		    break;
		    case 4:
		    EVSYS.CH4MUX = qPinInput;
		    EVSYS.CH4CTRL = EVSYS_QDEN_bm | EVSYS_DIGFILT_2SAMPLES_gc;
		    if(useIndex){
			         EVSYS.CH5MUX = qPinInput + 2;
			         EVSYS.CH5CTRL = EVSYS_DIGFILT_2SAMPLES_gc;
			         EVSYS.CH4CTRL |= (uint8_t) qIndexState | EVSYS_QDIEN_bm;
		    }
		    break;
		    default:
		    return false;
	}
	return true;
 }
 
 uint8_t QDEC_Get_Direction(TC0_t* qTimer)
 {
	if (qTimer->CTRLFSET & TC0_DIR_bm){
		return CW_DIR;
	}else{
		return CCW_DIR;
	}
}

bool QDEC_Port_Setup(PORT_t *qPort, uint8_t qPin, bool useIndex, bool invIO)
{
	/* Make setup depending on if Index signal is used. */
	if(useIndex){
		if(qPin > 5){
			return false;
		}
		qPort->DIRCLR = (0x07<<qPin);
		
		/* Configure Index signal sensing. */
		PORTCFG.MPCMASK = (0x04<<qPin);
		qPort->PIN0CTRL = (qPort->PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_BOTHEDGES_gc
		                  | (invIO ? PORT_INVEN_bm : 0);
		
		
	}else{
			if(qPin > 6){
				return false;
			}
		    qPort->DIRCLR = (0x03<<qPin);
	}
	
	/* Set QDPH0 and QDPH1 sensing level. */
	PORTCFG.MPCMASK = (0x03<<qPin);
	qPort->PIN0CTRL = (qPort->PIN0CTRL & ~PORT_ISC_gm) | PORT_ISC_LEVEL_gc
						| (invIO ? PORT_INVEN_bm : 0);
	
	return true;
}

				  
void QDEC_TC_Dec_Setup(TC0_t * qTimer, TC_EVSEL_t qEventChannel, uint8_t lineCount)
{	
	/* Configure TC as a quadrature counter. */
	qTimer->CTRLD = (uint8_t) TC_EVACT_QDEC_gc | qEventChannel;
	qTimer->PER = (lineCount * 4) - 1;
	qTimer->CTRLA = TC_CLKSEL_DIV64_gc;
	
	//Watch for overflow
	qTimer->CTRLB = TC0_CCAEN_bm | TC_WGMODE_NORMAL_gc;
	qTimer->INTCTRLB = (uint8_t) TC_CCAINTLVL_MED_gc;
	qTimer->CCA = (lineCount * 4) - 1;
}


void QDEC_TC_Freq_Setup(TC0_t* qTimer, TC_EVSEL_t qEventChannel, EVSYS_CHMUX_t qPinInput, TC_CLKSEL_t clksel)
{
	
	///* Configure channel 2 to input pin for freq calculation. */
	//EVSYS.CH2MUX = qPinInput;
	//EVSYS.CH2CTRL = EVSYS_DIGFILT_4SAMPLES_gc;
		//
	///* Configure TC to capture frequency. */
	//qTimer->CTRLD = (uint8_t) TC_EVACT_FRW_gc | qEventChannel;
	//qTimer->PER = 0xFFFF;
	//qTimer->CTRLB = TC0_CCAEN_bm;
	//qTimer->CTRLA = clksel;
}

bool QDEC_Total_Setup(PORT_t* qPort,
					  uint8_t qPin,
					  bool invIO,
					  uint8_t qEvMux,
					  EVSYS_CHMUX_t qPinInput,
					  bool useIndex,
					  EVSYS_QDIRM_t qIndexState,
					  TC0_t* qTimer,
					  TC_EVSEL_t qEventChannel,
					  uint8_t lineCount)
{
	if( !QDEC_Port_Setup(qPort, qPin, useIndex, invIO) )
		return false;
	if( !QDEC_EVSYS_Setup(qEvMux, qPinInput, useIndex, qIndexState ) )
		return false;
	QDEC_TC_Dec_Setup(qTimer, qEventChannel, lineCount);
	
	return true;
}						  
