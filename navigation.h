/*
	navigation.h

	Contains functions for returning navigation information.
 */

#pragma once

#include "type_defs.h"

namespace Navigation
{
	/**
	 * Returns the desired heading from one GPSPosition to another GPSPosition.
	 * 
	 * @param  curr_pos    		The current GPS Position of the boat
	 * @param  destination 		The destination of the boat.
	 * @return             		The heading the boat should follow.
	 */
	int GetDesiredHeading(GPSPosition curr_pos, GPSPosition destination);
}