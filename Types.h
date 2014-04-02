
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
	uint32_t Date;
	uint32_t Time;
};

#endif