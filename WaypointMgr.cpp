/**
	WaypointMgr.cpp

	The waypoint manager deals with the navigation path of the 		boat.

	Author: Mazhar Shar
	GitHub: https://github.com/emnes

	This code is released under the terms of the LGPLv3 licence.
 */
 
#include "WaypointMgr.h"

/**
* Adds a waypoint to the manager
*
* @param wp Waypoint to add.
*/
void WaypointMgr::Add(Waypoint wp)
{
	m_Waypoints.push(wp);
}

/**
* Returns the first waypoint in the queue.
*
* @return Current target waypoint
*/
Waypoint WaypointMgr::GetCurrentTarget()
{
  	return m_Waypoints.peek();
}

/**
* Removes current waypoint and advances the waypoint manager to the next waypoint.
*
* @return Returns false when no more waypoints exist.
*/
bool WaypointMgr::Advance()
{ 
	if(!m_Waypoints.isEmpty())
	{
		m_Waypoints.pop();
		return true;
	}
	return false;
}

/**
* Returns the number of remaining waypoints.
*
* @return Number of waypoints.
*/
unsigned int WaypointMgr::Count()
{
	return m_Waypoints.count();
}
