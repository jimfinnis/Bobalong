
/*

	Types.h

	Contains all the types for Bobalong, I.E structs and enums

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef Types_H
#define Types_H

/// Contains all the boat's sensor data at a particular time.
struct BoatData {
	// Bearing data
	int Heading;
	int Roll;
	int Pitch;

	// Wind data
	float WindSpeed;
	float WindDirection;

	//GPS data
	double Latitude;
	double Longitude;

	// The heading of the boat acoording to the GPS in degrees
	double Course;

	// Speed in knots, gathered from the GPS.
	double Speed;

	// Date
	uint8_t DateDay;
        	uint8_t DateMonth;

        	// Time
	uint8_t TimeHours;
        	uint8_t TimeMinutes;
        	uint8_t TimeSeconds;
};

/// Defines a position used by the boat to navigate.
struct Waypoint {
	double Latitude;
	double Longitude;
};

#endif
