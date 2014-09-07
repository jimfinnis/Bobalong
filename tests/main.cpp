#include <iostream>
#include <cstdio>
using namespace std;

#include "../navigation.h"

/**
 * Tests whether Navigation::GetDesiredHeading() is functioning
 */
void NAV_GetDesiredHeading() {
	GPSPosition boat_pos;
	boat_pos.latitude = 52.4000;
	boat_pos.longitude = 3.8700;
	GPSPosition dest;
	dest.latitude = 20.7486;
	dest.longitude = 30.8500;
	cout << "int Navigation::GetDesiredHeading: " << Navigation::GetDesiredHeading(boat_pos, dest);
}

int main() {
  cout << "Hello, World!" << endl;
  NAV_GetDesiredHeading();
  getchar();
  return 0;
}