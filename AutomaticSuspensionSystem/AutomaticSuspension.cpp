/*
 * AutomaticSuspention.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "AutomaticSuspension.h"


AutomaticSuspension::AutomaticSuspension() {
}

AutomaticSuspension::~AutomaticSuspension() {
}

void AutomaticSuspension::init() {
	frontSuspension = new Suspension(FRONT_SERVO_PIN, FRONT_SERVO_FEEDBACK_PIN);
	rearSuspension = new Suspension(10, 0);
	cadenceSystem = new CadenceSystem();

	threadListeners.push_back(this);
	threadListeners.push_back(cadenceSystem);

	pinMode(13, OUTPUT);
}

void AutomaticSuspension::update() {

	if (Serial.available() > 0) {
		int inByte = Serial.read();
		if (inByte == 'c') {
			Serial.println("Calibrating... ");
			frontSuspension->calibrate();
		} else if (inByte == 'r') {
			Serial.println("Release... ");
			frontSuspension->release();
		} else if (inByte == 'l') {
			Serial.println("Lock... ");
			frontSuspension->lock();
		}
	}

	if (cadenceSystem->isPedalling()) {
		digitalWrite(13, HIGH);
	} else {
		digitalWrite(13, LOW);
	}

}

int AutomaticSuspension::getPriority() {
	return HIGH_PRIORITY;
}

vector<ThreadListener*> AutomaticSuspension::getThreadListeners() {
	return threadListeners;
}
int AutomaticSuspension::getThreadListenersCount() {
	return 2;
}
