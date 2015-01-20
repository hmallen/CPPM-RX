/* 
 *		CPPM Receiver Library
 *		by Phillip Schmidt, 01/15
 *		v1.0
 *
 *		StartCPPM(int) -- Run once in setup.  Pass the Arduino interrupt number (not pin number)
 * 
 *		To Access pulse widths use the following:
 *			RC_THROT
 *			RC_AILER
 *			RC_ELEV
 *			RC_RUDD
 *			RC_MODE
 *			RC_AUX1
 *			RC_AUX2
 *			RC_AUX3
 *		(See below to adjust pulse number if your receiver is different than above)
 *		
 *		RX_Fail() -- this function will return TRUE if last pulse was seen more than 0.5s ago
 *
 *		CPPM_display()  -- this function will output the current pulse values to the default serial port.  Used for testing.
 */	
 
 

#ifndef CPPM-RX_H
#define CPPM-RX_H

#include "arduino.h"

// map channels to more readable names
#define RC_THROT	CPPM_Channels[2]	// Orange RX R615X
#define RC_AILER	CPPM_Channels[0]
#define RC_ELEV		CPPM_Channels[1]
#define RC_RUDD		CPPM_Channels[3]
#define RC_MODE		CPPM_Channels[4]
#define RC_AUX1		CPPM_Channels[5]
#define RC_AUX2		CPPM_Channels[6]	// not used
#define RC_AUX3		CPPM_Channels[7]	// not used


int volatile 	CPPM_Channels [8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long	CPPM_TimePrevious = 0;

void CPPM_ISR()
{
	unsigned long static	CPPM_TimeNow;
	int	static				CPPM_Pulse;
	byte static				CPPM_Pointer = 0;

	CPPM_TimeNow 		= micros();
	CPPM_Pulse   		= int(CPPM_TimeNow - CPPM_TimePrevious);
	CPPM_TimePrevious	= CPPM_TimeNow;

	if(CPPM_Pulse < 4000)
	{	
		CPPM_Pointer &= B00000111;		// prevent over-running array if bad signal
		CPPM_Channels[CPPM_Pointer] = CPPM_Pulse;
		CPPM_Pointer++;
	}
	else
	{
		CPPM_Pointer = 0;
	}
}


void StartCPPM(int intPin)
{
	attachInterrupt(intPin, CPPM_ISR, RISING);
}


byte RX_Fail()	// check if last observed pulse was more than 0.5s ago
{
	if(micros() - CPPM_TimePrevious > 500000UL)
	{
		return 1;	// RC link failed
	}
	return 0;		// RC link good
}

void CPPM_display()
{
	for(byte i = 0; i < 8; i++)
	{
		Serial.print(CPPM_Channels[i]);
		Serial.print("\t");
	}

	Serial.println("");
}









#endif