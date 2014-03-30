
/*
 * HMC6343.cpp - A library for using the HMC6343 compass with a ardunio.
 * 
 * Author: Jordan Springett
 * GitHub: https://github.com/JSpringett
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

#include "Arduino.h"
#include "Wire.h"
#include "HMC6343.h"

 #define Null 0

/*=======================================================
*/
HMC6343::HMC6343() {
	Wire.begin();
	TimeOut = 30;
}

/*=======================================================
* Determines if the compass is function correctly.
*/
bool HMC6343::IsFunctioning() {
	float x, y, z;
	if(ReadCompass(HMC6343_BEARING_REG, x, y, z))
		return false;
	return true;
}

/*=======================================================
 * Returns the bearing in degress of a HMC6343 compass.
 * Includes heading, pitch, and roll.
 */
void HMC6343::GetBearing(float& heading, float& pitch, float& roll) {
	ReadCompass(HMC6343_BEARING_REG, heading, pitch, roll);
	heading = heading/10;
	pitch = pitch/10;
	roll = roll/10;
}

/*=======================================================
 * Returns the acceleration in the x,y, and z axis of a 
 * HMC6343 compass. 
 */
void HMC6343::GetAcceleration(float& x, float& y, float& z) {
	ReadCompass(HMC6343_ACCELEROMETER_REG, x, y, z);
	x /= 10;
	y /= 10;
	z /= 10;
}

///////////////////////////////////////////////////////////////////////////////
// HMC6343 interaction


/*=======================================================
 * Returns a byte from a register in the I2C device.
 *
 * @param register 		The register on the HMC6343 we want to 
 *				access.
 * @param v0			The first value that is returned from the HMC.
 * @param v1			The second value that is returned from the HMC.
 * @param v2			The third value that is returned from the HMC.
 */
bool HMC6343::ReadCompass(byte register, float& v0, float& v1, float& v2 ) {
	byte high, low;

	// Start the communication with the I2C device
	Wire.beginTransmission(HMC6343_ADDRESS);

	// Send the address of the registers we want to read
	Wire.write(HMC6343_ACCELEROMETER_REG);
	Wire.endTransmission();
	Wire.requestFrom(HMC6343_ADDRESS, 6);

	float startTime = millis();

	bool gettingData = false;
	 // Wait for the data
	while(Wire.available() < 1 && startTime > millis() - TimeOut && !gettingData) {
		if(Wire.available() > 0) {
			gettingData = true;
		}
	}

	if(!gettingData) {
		return false;
	}

	// Read the data
	v0 = ReadValue();
	v1 = ReadValue();
	v2 = ReadValue();

	return true;
}

/*=======================================================
 * Reads a value from the compass, each value consists of two bytes 
 * which are combined and returned.
 */
float HMC6343::ReadValue() {
	byte high, low;

	 high = Wire.read();
	low = Wire.read();

	return CombineByte(high, low);
}

/*=================================================================
 * Combines a high and low byte together, to form a single number.
 * @param v0			The first byte for a value returned by the HMC.
 * @param v0			The second byte for a value returnd by the HMC.
 */
float HMC6343::CombineByte(byte high, byte low) {
	return ((high << 8)+ low);
}
