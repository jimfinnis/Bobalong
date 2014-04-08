
/*
	WaypointMgr.h

	The waypoint manager deals with the navigation path of the boat.

	Author: Jordan Springett
	GitHub: https://github.com/JSpringett

	This code is released under the terms of the LGPLv3 licence.
 */

#ifndef Included_WaypointMgr_H
#define Included_WaypointMgr_H

#include "Types.h"
#include "QueueList.h"

class WaypointMgr {
	//=====================================================================
	/**
	 * Adds a waypoint to the manager
	 * 
	 * @param wp 		Waypoint to add.
	 */
	void Add(Waypoint wp);

	//=====================================================================
	/**
	 * Returns the next waypoint in the queue, I.E the front waypoint.
	 * 
	 * @return 		The next waypoint in the path.
	 */
	Waypoint GetNext();

	//=====================================================================
	/**
	 * Advances the waypoint manager to the next waypoint if there is one.
	 * 
	 * @return 		Returns false when no more waypoints exist.
	 */
	bool Advance();

	//=====================================================================
	/**
	 * Returns the number of remaining waypoints.
	 * 	
	 * @return 		Number of waypoints.
	 */
	unsigned int Count();

	 //*******************************************************************************//
	// Member Fields               											 
	//*******************************************************************************//
private:
	QueueList<Waypoint> 		m_Waypoints		// Queue of waypoints.
}

#endif