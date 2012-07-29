/*
 * AutomaticSuspention.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "AutomaticSuspension.h"

void(* resetArduino) (void) = 0; //declare reset function @ address 0


AutomaticSuspension::AutomaticSuspension(): threadListeners(), mode(true) {
}

AutomaticSuspension::~AutomaticSuspension() {
}

void AutomaticSuspension::init() {

	frontSuspension = new Suspension(FRONT_SUSPENSION_CONTROL_PIN, FRONT_SUSPENSION_FEADBACK_PIN);
	rearSuspension = new Suspension(REAR_SUSPENSION_CONTROL_PIN, REAR_SUSPENSION_FEADBACK_PIN);

	cadenceSystem = new CadenceSystem(CADENCE_PIN);
	threadListeners.push_back(cadenceSystem);

	//forkAccelerometerSystem = new ForkAccelerometerSystem();

	//threadListeners.push_back(forkAccelerometerSystem);

	frontButton = new Button(FRONT_BUTTON_PIN, false);
	threadListeners.push_back(frontButton);
	modeButton = new Button(MODE_BUTTON_PIN, false);
	threadListeners.push_back(modeButton);
	rearButton = new Button(REAR_BUTTON_PIN, true);
	threadListeners.push_back(rearButton);

	threadListeners.push_back(this);
	Serial.println("DI");

}

void AutomaticSuspension::update() {

	bool lock = false;

	if (modeButton->isPushed()) {
		frontSuspension->calibrate();
		rearSuspension->calibrate();
	} else if (frontButton->isPushed()) {
		if (frontSuspension->locked()) {
			frontSuspension->release();
			rearSuspension->lock();
		} else {
			frontSuspension->lock();
			rearSuspension->release();
		}
	} else if (rearButton->isPushed()) {
		mode = !mode;
	} else {
		if (mode) {
			lock = cadenceSystem->isPedalling();

			if (lock) {
				Serial.println("L");
//				rearSuspension->lock();
//				frontSuspension->release();
			} else {
//				rearSuspension->release();
//				frontSuspension->lock();
			}
		}
	}
}


vector<ThreadListener*> AutomaticSuspension::getThreadListeners() {
	return threadListeners;
}

