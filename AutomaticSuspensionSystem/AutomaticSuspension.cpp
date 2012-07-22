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
	//frontSuspension = new Suspension(FRONT_SUSPENSION_CONTROL_PIN, FRONT_SUSPENSION_FEADBACK_PIN);
	//rearSuspension = new Suspension(REAR_SUSPENSION_CONTROL_PIN, REAR_SUSPENSION_FEADBACK_PIN);
	//cadenceSystem = new CadenceSystem();
	forkAccelerometerSystem = new ForkAccelerometerSystem();

	threadListeners.push_back(forkAccelerometerSystem);
	//threadListeners.push_back(cadenceSystem);
	//threadListeners.push_back(this);


	//pinMode(FRONT_BUTTON_PIN, INPUT);
	//pinMode(MODE_BUTTON_PIN, INPUT);
	//pinMode(REAR_BUTTON_PIN, INPUT);

}

void AutomaticSuspension::update() {

	if (Serial.available() > 0) {
		int inByte = Serial.read();
		if (inByte == 'c') {
			Serial.println("Calibrating... ");
			frontSuspension->calibrate();
			rearSuspension->calibrate();
		} else if (inByte == 'r') {
			Serial.println("Release... ");
			frontSuspension->release();
			rearSuspension->release();
		} else if (inByte == 'l') {
			Serial.println("Lock... ");
			frontSuspension->lock();
			rearSuspension->lock();
		}
	}

//	if (digitalRead(MODE_BUTTON_PIN) == HIGH) {
//		Serial.println("Calibrating... ");
//		frontSuspension->calibrate();
//		rearSuspension->calibrate();
//	} else if (digitalRead(FRONT_BUTTON_PIN) == HIGH) {
//		frontSuspension->toggle();
//	} else if (digitalRead(REAR_BUTTON_PIN) == HIGH) {
//		rearSuspension->toggle();
//	}


}

int AutomaticSuspension::getPriority() {
	return HIGH_PRIORITY;
}

vector<ThreadListener*> AutomaticSuspension::getThreadListeners() {
	return threadListeners;
}

