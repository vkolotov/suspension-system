/*
 * AutomaticSuspention.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "AutomaticSuspension.h"

void (*resetArduino)(void) = 0; //declare reset function @ address 0

AutomaticSuspension::AutomaticSuspension() :
		threadListeners() {
}

AutomaticSuspension::~AutomaticSuspension() {
}

void AutomaticSuspension::init() {

	frontSuspension = new Suspension(FRONT_SUSPENSION_CONTROL_PIN,
			FRONT_SUSPENSION_FEADBACK_PIN, true);
	threadListeners.push_back(frontSuspension);
	rearSuspension = new Suspension(REAR_SUSPENSION_CONTROL_PIN,
			REAR_SUSPENSION_FEADBACK_PIN, false);
	threadListeners.push_back(rearSuspension);

	cadenceSystem = new CadenceSystem(CADENCE_PIN);
	threadListeners.push_back(cadenceSystem);

	//forkAccelerometerSystem = new ForkAccelerometerSystem();
	//threadListeners.push_back(forkAccelerometerSystem);

	frontButton = new Button(FRONT_BUTTON_PIN, false);
	threadListeners.push_back(frontButton);
	modeButton = new Button(MODE_BUTTON_PIN, true);
	threadListeners.push_back(modeButton);
	rearButton = new Button(REAR_BUTTON_PIN, false);
	threadListeners.push_back(rearButton);

	threadListeners.push_back(this);
	//Serial.println("DI");

}

void AutomaticSuspension::update() {

	if (modeButton->isPushed(3000)) {
		resetArduino();
		return;
	}
	if (frontButton->isPushed(3000)) {
		frontSuspension->calibrate();
		frontSuspension->lock();
		rearSuspension->calibrate();
		rearSuspension->lock();
		return;
	}
	if (rearButton->isPushed(3000)) {
		return;
	}
	if (modeButton->isPushed()) {
		if (frontButton->isPushed()) {

		} else if (rearButton->isPushed()) {

		}
		if (cadenceSystem->isPedalling()) {
			rearSuspension->lock();
			frontSuspension->lock();
		} else {
			rearSuspension->release();
			frontSuspension->release();
		}
	} else {
		if (frontButton->isPushed()) {
			frontSuspension->toggle();
		}
		if (rearButton->isPushed()) {
			rearSuspension->toggle();
		}
	}
}

vector<Activity*> AutomaticSuspension::getThreadListeners() {
	return threadListeners;
}
