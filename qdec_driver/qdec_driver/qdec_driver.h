/*
 * qdec_driver.h
 *
 * Created: 11/2/2014 10:29:20 PM
 *  Author: Nick
 */ 

#ifndef QDEC_DRIVER_H_
#define QDEC_DRIVER_H_


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define CW_DIR	0
#define CCW_DIR	1

#define GetCaptureValue(_tc)	(_tc.CCA)

/* Prototyping of functions. */

 bool QDEC_Total_Setup(PORT_t * qPort,
                       uint8_t qPin,
                       bool invIO,
                       uint8_t qEvMux,
                       EVSYS_CHMUX_t qPinInput,
                       bool useIndex,
                       EVSYS_QDIRM_t qIndexState,
                       TC0_t * qTimer,
                       TC_EVSEL_t qEventChannel,
                       uint8_t lineCount);
 
 bool QDEC_Port_Setup(PORT_t * qPort, uint8_t qPin, bool useIndex, bool invIO);
 
 bool QDEC_EVSYS_Setup(uint8_t qEvMux,
                        EVSYS_CHMUX_t qPinInput,
                        bool useIndex,
                        EVSYS_QDIRM_t qIndexState );
 
 void QDEC_TC_Dec_Setup(TC0_t * qTimer,
                        TC_EVSEL_t qEventChannel,
                        uint8_t lineCount);
 
 void QDEC_TC_Freq_Setup(TC0_t * qTimer,
                         TC_EVSEL_t qEventChannel,
                         EVSYS_CHMUX_t qPinInput,
                         TC_CLKSEL_t clksel);
 
 uint8_t QDEC_Get_Direction(TC0_t * qTimer);

#endif /* QDEC_DRIVER_H_ */