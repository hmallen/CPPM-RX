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
 *									Use "#define SERIAL_PORT Serial" in the main program before including this library to enable output
 */	
 
 

#ifndef CPPM-RX_H
	#define CPPM-RX_H

	#include "arduino.h"

	// change these index numbers to match the receiver being used
	#define CPPM_AILER_INDEX	0	// Orange RX R615X
	#define CPPM_ELEV_INDEX		1
	#define CPPM_THROT_INDEX	2	
	#define CPPM_RUDD_INDEX		3
	#define CPPM_MODE_INDEX		4
	#define CPPM_AUX1_INDEX		5
	#define CPPM_AUX2_INDEX		6	// not used
	#define CPPM_AUX3_INDEX		7	// not used

	// map channels to more readable names
	#define RC_THROT	CPPM_Channels[CPPM_THROT_INDEX]
	#define RC_AILER	CPPM_Channels[CPPM_AILER_INDEX]
	#define RC_ELEV		CPPM_Channels[CPPM_ELEV_INDEX]
	#define RC_RUDD		CPPM_Channels[CPPM_RUDD_INDEX]
	#define RC_MODE		CPPM_Channels[CPPM_MODE_INDEX]
	#define RC_AUX1		CPPM_Channels[CPPM_AUX1_INDEX]
	#define RC_AUX2		CPPM_Channels[CPPM_AUX2_INDEX]
	#define RC_AUX3		CPPM_Channels[CPPM_AUX3_INDEX]


	byte CPPM_FLAGS = 0;
	int volatile 	CPPM_Channels [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned long	CPPM_TimePrevious = 0;


	void CPPM_ISR()
	{
		byte static CPPM_Pointer = 0;

		unsigned long CPPM_TimeNow = micros();
		int CPPM_Pulse = int(CPPM_TimeNow - CPPM_TimePrevious); // pulse time in micro seconds
		CPPM_TimePrevious = CPPM_TimeNow;

		if(CPPM_Pulse < 2600)		// short pulse indicates channel
		{	
			CPPM_Pointer &= B00000111;				// prevent over-running array if bad signal
			CPPM_Channels[CPPM_Pointer] = CPPM_Pulse;
			bitSet(CPPM_FLAGS, CPPM_Pointer);		// indicate channel has new pulse
			CPPM_Pointer++;
		}
		else						// long pulse indicates beginning of pulse string
		{
			CPPM_Pointer = 0;
		}
	}


	void StartCPPM(int intPin)
	{
		attachInterrupt(digitalPinToInterrupt(intPin), CPPM_ISR, RISING);
	}


	byte RX_Fail()	// check if last observed pulse was more than 0.5s ago
	{
		unsigned long timeNow = micros();
		
		if(timeNow > CPPM_TimePrevious && timeNow - CPPM_TimePrevious > 500000UL) // don't indicate failed link on timer roll-over
		{
			return 1;	// RC link failed
		}
		
		return 0;		// RC link good
	}

	byte RX_Good()	// check if last observed pulse was more than 0.5s ago
	{
		return !RX_Fail();		// RC link good
	}

	byte AilerNew()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_AILER_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_AILER_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte ElevNew()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_ELEV_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_ELEV_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte ThrotNew()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_THROT_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_THROT_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte RuddNew()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_RUDD_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_RUDD_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte ModeNew()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_MODE_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_MODE_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte Aux1New()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_AUX1_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_AUX1_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte Aux2New()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_AUX2_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_AUX2_INDEX);
			return 1;
		}
		
		return 0;
	}


	byte Aux3New()	// return true if a new pulse has been received
	{
		if(bitRead(CPPM_FLAGS, CPPM_AUX3_INDEX))
		{
			bitClear(CPPM_FLAGS, CPPM_AUX3_INDEX);
			return 1;
		}
		
		return 0;
	}




	
	void CPPM_display()
	{
		#ifdef SERIAL_PORT
			
			String outputBuffer;
			
			for(byte i = 0; i < 8; i++)
			{
				outputBuffer += String(CPPM_Channels[i], DEC);
				outputBuffer += '\t';
			}
			
			outputBuffer += '\n';
			
			SERIAL_PORT.print(outputBuffer);
			
		#endif
	}
	

#endif
