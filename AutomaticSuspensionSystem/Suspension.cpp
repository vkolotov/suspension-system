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
	servo = new CalibratableServo(servoPin, feedbackPin);
	servo->bind();
}

Suspension::~Suspension() {

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

void Suspension::set(int angle) {
	servo->write(angle);
}

void Suspension::calibrate() {
	Serial.println("Suspension: calibrating... ");
	servo->calibrate();
	Serial.print("Calibrated. Min =  ");
	Serial.print(servo->getMinAngle());
	Serial.print(". Max = ");
	Serial.println(servo->getMaxAngle());
}

bool Suspension::isCalibrated() {
	return servo->isCalibrated();
}
