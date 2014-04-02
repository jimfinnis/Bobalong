// Arduino Libraries
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <stdint.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "Types.h"
#include "HMC6343.h"
#include "TinyGPSPlus.h"
#include "Rowind.h"

/////////////////////////////////////////////////////////////////
// Variables

HMC6343 Compass;
BoatData Boat;
TinyGPSPlus gps;
Rowind rowind(4,5);
SoftwareSerial ss(2,3);


Servo rudder;
// Autonomous sailing
int m_DestHeading;

/////////////////////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	ss.begin(4800);
	
	// Light up debugger LED
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);

	// Check all components are working
	if(!Compass.IsFunctioning()) {
		Serial.println("Error with compass!");
	}
	if(!rowind.IsFunctioning()) {
		Serial.println("Error with Rowind!");
	}
				
	// Get starting boat heading
	UpdateCompass();
	m_DestHeading = Boat.Heading;
	Serial.println(m_DestHeading);
 
	rudder.attach(9);
	rudder.write(90);
}

/////////////////////////////////////////////////////////////////
void loop() {
	// LED debugger
	digitalWrite(13, HIGH);
	ss.listen();
	// Update data
	UpdateCompass();
	UpdateGPS();
	delay(77);

	rowind.GetData(Boat.WindDirection, Boat.WindSpeed);
				
	KeepHeading();
					
	// Log it
	LogData();
	
	digitalWrite(13,LOW);
	
	// Don't update or log for 5 seconds
	delay(500);
}

/////////////////////////////////////////////////////////////////
// Update data from hardware
/////////////////////////////////////////////////////////////////

void UpdateCompass() {
	int pitch = 0;
	Compass.GetBearing(Boat.Heading, Boat.Pitch, Boat.Roll);
}


void UpdateGPS(){
	while (ss.available()>0)
	{  
		delay(1);
		if (gps.encode(ss.read()))
		{
			// Debugger LED
			digitalWrite(13, LOW);

			// Store GPS data
			Boat.Latitude = gps.location.lat();
			Boat.Longitude = gps.location.lng();
			Boat.Date = gps.date.value();
			Boat.Time = gps.time.value();
		}
	}   
}
/////////////////////////////////////////////////////////////
// Logs all the data to the openlogger
void LogData() {
	// Boat Heading
	Serial.print("bhead="); Serial.print(Boat.Heading); Serial.print(" ");
	// Boat Wind Dir
	Serial.print("wind="); Serial.print(Boat.WindDirection); Serial.print(" ");
	// Wind speed
	Serial.print("windSpd="); Serial.print(Boat.WindSpeed); Serial.print(" ");
	// Lat
	Serial.print("lat="); Serial.print(Boat.Latitude); Serial.print(" ");
	// Long
	Serial.print("lon="); Serial.print(Boat.Longitude); Serial.print(" ");
	// TIme
	Serial.print("date="); Serial.print(Boat.Date); Serial.print(" ");
	// TIme
	Serial.print("time="); Serial.print(Boat.Time); Serial.println(" ");
}

/////////////////////////////////////////////////////////////
// Tries to keep the boat heading in the direction it was 

// facing when started.
void KeepHeading() {
	int headingOff = Boat.Heading - m_DestHeading;
	
	if(headingOff > -1 && headingOff < 1) {
		rudder.write(90);
		return;
	}
	
	if(Boat.Heading >= 0 && Boat.Heading < 180) {
		float pOff = (float)headingOff / 180.0f;
		int rot = 90 + (int)(pOff * 90);
		rudder.write(rot);
	} else {
		float pOff = (float)headingOff / 180.0f;
		int rot = 90 - (int)(pOff * 90);
		rudder.write(rot);
	}
}
