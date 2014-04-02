
/*
 * HMC6343.h - A library for using the HMC6343 compass with a ardunio.
  * 
 * Author: Jordan Springett
 * GitHub: https://github.com/JSpringett
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

#ifndef HMC6343_H
#define HMC6343_H

#include "Arduino.h"

#define HMC6343_ADDRESS 0x19
#define HMC6343_BEARING_REG 0x50
 #define HMC6343_ACCELEROMETER_REG 0x50

 class HMC6343 {
 public:
 	HMC6343();

 	//=====================================================================
 	/**
 	 * Determines if the compass is functioning correctly.
 	 */
 	bool IsFunctioning();

 	//=====================================================================
 	/**
 	 * Returns the bearing in heading, pitch and roll
 	 */
	void GetBearing(int& heading, int& pitch, int& roll);

	//=====================================================================
	/**
	 * Returns the acceleration in 3 values, accX, accY and acc,Z
	 */
	void GetAcceleration(int& accX, int& accY, int& accZ);
 private:
 	//=====================================================================
 	/**
 	 * Tries to read a particular register from the compass and inserts the values into
 	 * 3 ints which are passed by reference.
 	 * 
 	 * @param  	register Register to access
 	 * @return 	Returns true if the method was able to successfully read compass
 	 *                          data.	
 	 */
 	bool ReadCompass(byte register, int& v0, int& v1, int& v2 );

 	//=====================================================================
 	/**
 	 * Reads a single value from the compass and returns it. The value is built from 
 	 * two bytes which are read from the compass and then combined
 	 */
 	int ReadValue();

 	//=====================================================================
 	/**
 	 * Combines a "high" and "low" byte to form one value.
 	 * @param  high [description]
 	 * @param  low  [description]
 	 * @return      [description]
 	 */
 	int CombineByte(byte high, byte low);

 	//*******************************************************************************//
	// Member Fields               											 
	//*******************************************************************************//
public:
	unsigned int TimeOut;		// The max amount of time to wait around for valid
					// data before giving up in milliseconds.
 };

 #endif
