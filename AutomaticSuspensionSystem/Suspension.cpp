/*
 * Suspension.cpp
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#include "Suspension.h"

Suspension::Suspension() {
	servo = new CalibratableServo();
}

Suspension::Suspension(int servoPin, int feedbackPin) {
	servo = new CalibratableServo();
	attach(servoPin, feedbackPin);
}

Suspension::~Suspension() {

}

void Suspension::attach(int servoPin, int feedbackPin) {
	servo->attach(servoPin);
	pinMode(feedbackPin, INPUT);
}


void Suspension::lock() {
	servo->writeMax();
}

void Suspension::release() {
	servo->writeMin();
}

void Suspension::calibrate() {
	Serial.println("Suspension: calibrating... ");
	servo->calibrate();
	Serial.print("Calibrated. Min =  ");
	Serial.print(servo->getMinAngle());
	Serial.print(". Max = ");
	Serial.print(servo->getMaxAngle());
}

bool Suspension::isCalibrated() {
	return servo->isCalibrated();
}
