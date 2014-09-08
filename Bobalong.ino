// Arduino Libraries
#include <Servo.h>
#include <Wire.h>
#include <stdint.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "type_defs.h"
#include "compass.h"
#include "wind_sensor.h"
#include "gps.h"

/////////////////////////////////////////////////////////////////
// Variables

Boat boat;
Servo rudder(_SV_1);
Servo sail(_SV_2);

//////////////////////////////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	WindSensor::Initialise();
	GPS::Initialise();

	// Wait for everything to get running.
	delay(100);

	// Light up debugger LED
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
}

//////////////////////////////////////////////////////////////////////////
void loop() {
	// LED debugger
	digitalWrite(13, HIGH);
	// Update data
	UpdateCompass();
	UpdateGPS();
	delay(77);
	UpdateWind();
					
	// Log it
	LogData();

	// If we have any waypoints navigate to them.
	if(WaypointMgr::GetWaypointCount > 0) {
		int desired_heading = Navigation::GetDesiredHeading(boat.bearing.heading);
		rudder.write(desired_heading);
	}

	// Causes the debug LED to flash, so we know the code hasn't gotten
	// stuck somewhere.
	digitalWrite(13,LOW);
	
	// Don't update or log for 3 seconds
	delay(300);
}

//////////////////////////////////////////////////////////////////////////
void UpdateCompass() {
	BearingData bearing;

	if(Compass::GetBearing(bearing)) {
		boat.bearing = bearing;
	} else {
		Serial.println("Warning: Failed to read compass!");
	}
}

//////////////////////////////////////////////////////////////////////////
void UpdateWind() {
	WindData wind;
	if(WindSensor::Read(wind)) {
		boat.wind = wind;
	} else {
		Serial.println("Warning: Failed to read wind sensor");
	}
}

//////////////////////////////////////////////////////////////////////////
void UpdateGPS(){
	boat.has_fix = GPS::HasFix();
	if(boat.has_fix) {
		boat.position = GPS::GetPosition();
		boat.course = GPS::GetCourse();
	}
	boat.date_time = GPS::GetDateTime();  
}

//////////////////////////////////////////////////////////////////////////
void LogData() {
	// Boat Heading
	Serial.print("bhead="); Serial.print(boat.bearing.heading); Serial.print(" ");
	// GPS Heading
	Serial.print("gpshead="); Serial.print(boat.course.bearing); Serial.print(" ");
	// Speed
	Serial.print("knots="); Serial.print(boat.course.speed); Serial.print(" ");
	// Boat Wind Dir
	Serial.print("wind="); Serial.print(boat.wind.direction); Serial.print(" ");
	// Wind speed
	Serial.print("windSpd="); Serial.print(boat.wind.speed); Serial.print(" ");
	// Lat
	Serial.print("lat="); Serial.print(boat.position.latitude,5); Serial.print(" ");
	// Long
	Serial.print("lon="); Serial.print(boat.position.longitude,5); Serial.print(" ");
	// Date
	Serial.print("date="); Serial.print(boat.date_time.date); Serial.print(" ");
	// Time
	Serial.print("time="); Serial.print(boat.date_time.time); Serial.println(" ");
}
