/*
 * Suspension.cpp
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#include "Suspension.h"

Suspension::Suspension(): isLocked(false) {
	servo = new CalibratableServo();
}

Suspension::Suspension(int servoPin, int feedbackPin) : isLocked(false) {
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
	isLocked = true;
}

void Suspension::release() {
	servo->writeMin();
	isLocked = false;
}

void Suspension::toggle() {
	isLocked ? release() : lock();
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
