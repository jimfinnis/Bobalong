// Arduino Libraries
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <stdint.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "HMC6343.h"
#include "TinyGPSPlus.h"
#include "Rowind.h"

/////////////////////////////////////////////////////////////////
// Variables

HMC6343 Compass;
TinyGPSPlus gps;
Rowind rowind(4,5);
SoftwareSerial ss(2,3);

Servo rudder;

// Bearing data
float C_Heading; 
float C_Roll;

// Wind data
float m_WindSpeed;
float m_WindDir;

//GPS data
double gps_lat;
double gps_long;
uint8_t gps_month;
uint8_t gps_day;
uint8_t gps_hours;
uint8_t gps_minutes;

// Autonomous sailing
int m_DestHeading;
//int m_TestHeading = 300;
//int m_TestDest = 360;

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
	m_DestHeading = C_Heading;
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
	rowind.GetData(m_WindDir, m_WindSpeed);
				
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
	float pitch = 0;
	Compass.GetBearing(C_Heading, pitch, C_Roll);
}


void UpdateGPS(){
	while (ss.available()>0)
	{  
		delay(1);
		if (gps.encode(ss.read()))
		{
			// Debugger LED
			digitalWrite(13, LOW);
			gps_lat = gps.location.lat();
			gps_long = gps.location.lng();
			gps_day = gps.date.day();
			gps_month = gps.date.month();
			gps_minutes = gps.time.minute();
			gps_hours = gps.time.hour();
			}
	}   
}
/////////////////////////////////////////////////////////////
// Logs all the data
void LogData() {
	// Boat Heading
	Serial.print("bhead="); Serial.print(C_Heading); Serial.print(" ");
	// Boat Wind Dir
	Serial.print("wind="); Serial.print(m_WindDir); Serial.print(" ");
	// Wind speed
	Serial.print("windSpd="); Serial.print(m_WindSpeed); Serial.print(" ");
	// Lat
	Serial.print("lat="); Serial.print(gps_lat); Serial.print(" ");
	// Long
	Serial.print("lon="); Serial.print(gps_long); Serial.print(" ");
	// TIme
	Serial.print("time="); Serial.print(gps_hours); Serial.print(":"); Serial.println(gps_minutes);
}

/////////////////////////////////////////////////////////////
// Tries to keep the boat heading in the direction it was 

// facing when started.
void KeepHeading() {
	int headingOff = (int)C_Heading - m_DestHeading;
	
	if(headingOff > -1 && headingOff < 1) {
		rudder.write(90);
		return;
	}
	
	if(C_Heading >= 0 && C_Heading < 180) {
		float pOff = (float)headingOff / 180.0f;
		int rot = 90 + (int)(pOff * 90);
		rudder.write(rot);
	} else {
		float pOff = (float)headingOff / 180.0f;
		int rot = 90 - (int)(pOff * 90);
		rudder.write(rot);
	}
}
