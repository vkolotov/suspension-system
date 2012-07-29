/*
 * Suspension.cpp
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#include "Suspension.h"

static const unsigned short POWER_SAVE_THRESHOLD = 500;

Suspension::Suspension(): isLocked(false) {
	servo = new CalibratableServo();
}

Suspension::Suspension(unsigned char servoPin, unsigned char feedbackPin, bool isReverse)
		: isLocked(false), isReverse(isReverse), lastTime(0) {
	servo = new CalibratableServo(servoPin, feedbackPin);
}

Suspension::~Suspension() {

}

void Suspension::lock() {
	if (isLocked) {
		return;
	}
	attach();
	isReverse ? servo->writeMin() : servo->writeMax();
	isLocked = true;

}

void Suspension::release() {
	if (!isLocked) {
		return;
	}
	attach();
	isReverse ? servo->writeMax() : servo->writeMin();
	isLocked = false;
}

void Suspension::toggle() {
	isLocked ? release() : lock();
}


void Suspension::calibrate() {
	attach();
	servo->calibrate();
}

bool Suspension::locked() {
	return isLocked;
}

void Suspension::attach() {
	if (!servo->attached()) {
		servo->bind();
	}
	lastTime = millis();
}

void Suspension::detach() {
	servo->detach();
}

void Suspension::update() {
	if (servo->isCalibrating() || !servo->attached()) {
		return;
	}
	long currentTime = millis();
	if (currentTime - lastTime > POWER_SAVE_THRESHOLD) {
		lastTime = currentTime;
		servo->detach();
	}
}
