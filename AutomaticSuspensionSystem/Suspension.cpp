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

Suspension::Suspension(unsigned char servoPin, unsigned char feedbackPin) : isLocked(false) {
	servo = new CalibratableServo(servoPin, feedbackPin);
}

Suspension::~Suspension() {

}

void Suspension::lock() {
	if (isLocked) {
		return;
	}
	servo->bind();
	delay(500);
	servo->writeMin();
	delay(500);
	servo->detach();
	isLocked = true;
}

void Suspension::release() {
	if (!isLocked) {
		return;
	}
	servo->bind();
	delay(500);
	servo->writeMax();
	delay(500);
	servo->detach();
	isLocked = false;
}

void Suspension::toggle() {
	isLocked ? release() : lock();
}


void Suspension::calibrate() {
	servo->bind();
	delay(500);
	servo->calibrate();
	delay(500);
	servo->detach();
}

bool Suspension::locked() {
	return isLocked;
}
