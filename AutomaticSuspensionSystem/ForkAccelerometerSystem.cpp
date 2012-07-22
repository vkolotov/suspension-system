/*
 * ForkAccelerometerSystem.cpp
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#include "ForkAccelerometerSystem.h"

ForkAccelerometerSystem::ForkAccelerometerSystem() : accel() {
	Wire.begin();

	    // initialize serial communication
	    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
	    // it's really up to you depending on your project)
	    //Serial.begin(38400);

	    // initialize device
	    Serial.println("Initializing I2C devices...");
	    accel.initialize();

	    // verify connection
	    Serial.println("Testing device connections...");
	    Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");
}

ForkAccelerometerSystem::~ForkAccelerometerSystem() {

}

void ForkAccelerometerSystem::update() {

	if (!accel.testConnection()) {
		Serial.println("Not connected");
	} else {
		// read raw accel measurements from device
			int16_t ax, ay, az;
		    accel.getAcceleration(&ax, &ay, &az);

		    // display tab-separated accel x/y/z values
		    Serial.print("accel:\t");
		    Serial.print(ax); Serial.print("\t");
		    Serial.print(ay); Serial.print("\t");
		    Serial.println(az);

	}

}

int ForkAccelerometerSystem::getPriority() {
	return HIGH_PRIORITY;
}

bool ForkAccelerometerSystem::isActive() {
	return false;
}
