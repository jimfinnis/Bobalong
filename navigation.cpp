/*
	navigation.h

	Contains functions for returning navigation information.
 */

#include "navigation.h"
#include "math.h"

#define MATH_DEG_TO_RAD 	3.14 / 180
#define MATH_RAD_TO_DEG		180 / 3.14	

//////////////////////////////////////////////////////////////////////////
int Navigation::GetDesiredHeading(GPSPosition curr_pos, GPSPosition destination)
{
	double lat_rad1 = curr_pos.latitude * MATH_DEG_TO_RAD;
	double lat_rad2 = destination.latitude * MATH_DEG_TO_RAD;
	double lon_rad1 = curr_pos.longitude * MATH_DEG_TO_RAD;
	double lon_rad2 = destination.longitude * MATH_DEG_TO_RAD;

	// Uses the greate circle route
	double lon = (destination.longitude - curr_pos.longitude) * MATH_DEG_TO_RAD;
	double y = sin(lon) * cos(lat_rad2);
	double x = (cos(lat_rad1) * sin(lat_rad2)) - (sin(lat_rad1) * cos(lat_rad2) * cos(lon));
	return atan2(y, x) * MATH_RAD_TO_DEG;
}
