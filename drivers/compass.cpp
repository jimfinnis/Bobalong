/*
	compass.cpp

	Author: Jordan Springett
	Created: 10/08/14
 	GitHub: https://github.com/JSpringett

	Provides functions for interacting with Bobalong's HMC6343 electronic compass

	This code is released under the terms of the LGPLv3 licence.
 */

#include "compass.h"
#include "Arduino.h"
#include "Wire.h"


//////////////////////////////////////////////////////////////////////////
bool Compass::Get_Bearing(BearingData& bearing)
{
	return Read(HMC343_BEARING_REG, bearing.heading, bearing.roll, bearing.pitch);
}

//////////////////////////////////////////////////////////////////////////
bool Compass::Read(byte reg, int& v0, int& v1, int& v2 )
{
	byte high, low;
	unsigned long timeout = 100;
	unsigned long last_time;

	// Start the communication with the I2C device
	Wire.beginTransmission(HMC6343_ADDRESS);

	// Send the address of the registers we want to read
	Wire.write(reg);
	Wire.endTransmission();

	// Requests 6 bytes from the compass and then a stop condition is
	// sent releaing the bus.
	Wire.requestFrom(HMC6343_ADDRESS, 6, true);

	 // Wait for the data, return false marking the read as failed if
	 // we go over the waiting time
	last_time = millis();
	while(Wire.available() < 2) {
		if(millis() - last_time > timeout) {
			return false;
		}
	}

	// Read the data
	v0 = ReadValue();
	v1 = ReadValue();
	v2 = ReadValue();

	return true;
}

//////////////////////////////////////////////////////////////////////////
int Compass::Read_Value()
{
	byte high, low;

	// read in the high and low bytes
	high = Wire.read();
	low = Wire.read();

	// Combine them into a single int so we can get one value from the 
	// compass.
	return CombineByte(high, low);
}

//////////////////////////////////////////////////////////////////////////
int Compass::Combine_Byte(byte high, byte low)
{
	return ((high << 8)+ low);
}