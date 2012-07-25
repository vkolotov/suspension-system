/*
 * AutomaticSuspention.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "AutomaticSuspension.h"

void(* resetArduino) (void) = 0; //declare reset function @ address 0


AutomaticSuspension::AutomaticSuspension() {
}

AutomaticSuspension::~AutomaticSuspension() {
}

void AutomaticSuspension::init() {
	rearSuspension = new Suspension(REAR_SUSPENSION_CONTROL_PIN, REAR_SUSPENSION_FEADBACK_PIN);
	frontSuspension = new Suspension(FRONT_SUSPENSION_CONTROL_PIN, FRONT_SUSPENSION_FEADBACK_PIN);

	//cadenceSystem = new CadenceSystem();
	//forkAccelerometerSystem = new ForkAccelerometerSystem();

	//threadListeners.push_back(forkAccelerometerSystem);
	//threadListeners.push_back(cadenceSystem);
	threadListeners.push_back(this);


	pinMode(FRONT_BUTTON_PIN, INPUT);
	pinMode(MODE_BUTTON_PIN, INPUT);
	pinMode(REAR_BUTTON_PIN, INPUT);

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
		} else if (inByte == 't') {
			Serial.println("Set 0");
			frontSuspension->set(10);
			delay(1000);
			rearSuspension->set(10);
			delay(1000);
			Serial.println("Set 180");
			frontSuspension->set(170);
			delay(1000);
			rearSuspension->set(170);
		} else if (inByte == 'q') {
			resetArduino();
		}
	}

	if (digitalRead(MODE_BUTTON_PIN) == HIGH) {
		if (digitalRead(FRONT_BUTTON_PIN) == HIGH) {
			resetArduino();
		}
		Serial.println("Calibrating... ");
		frontSuspension->calibrate();
		rearSuspension->calibrate();
	} else if (digitalRead(FRONT_BUTTON_PIN) == HIGH) {
		frontSuspension->toggle();
	} else if (digitalRead(REAR_BUTTON_PIN) == HIGH) {
		rearSuspension->toggle();
	}

}

int AutomaticSuspension::getPriority() {
	return HIGH_PRIORITY;
}

vector<ThreadListener*> AutomaticSuspension::getThreadListeners() {
	return threadListeners;
}

