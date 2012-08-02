/*
 * AutomaticSuspention.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "AutomaticSuspension.h"

void (*resetArduino)(void) = 0; //declare reset function @ address 0

AutomaticSuspension::AutomaticSuspension() {
}

AutomaticSuspension::~AutomaticSuspension() {
}

void AutomaticSuspension::init() {

	frontSuspension = new Suspension(FRONT_SUSPENSION_CONTROL_PIN,
			FRONT_SUSPENSION_FEADBACK_PIN, true);

	rearSuspension = new Suspension(REAR_SUSPENSION_CONTROL_PIN,
			REAR_SUSPENSION_FEADBACK_PIN, false);

	cadenceSystem = new CadenceSystem(CADENCE_PIN);

	forkAccelerometerSystem = new ForkAccelerometerSystem();

	frontButton = new Button(FRONT_BUTTON_PIN, false);

	modeButton = new Button(MODE_BUTTON_PIN, true);

	rearButton = new Button(REAR_BUTTON_PIN, false);

}

void AutomaticSuspension::update(unsigned long currentTime) {

	if (modeButton->isPushed(3000)) {
		resetArduino();
		return;
	}
	if (frontButton->isPushed(3000)) {
		forkAccelerometerSystem->calibrate();
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
			forkAccelerometerSystem->threshold += 50;
			if (forkAccelerometerSystem->threshold > 600) {
				forkAccelerometerSystem->threshold = 600;
			}

		} else if (rearButton->isPushed()) {
			forkAccelerometerSystem->threshold -= 50;
			if (forkAccelerometerSystem->threshold < 0) {
				forkAccelerometerSystem->threshold = 0;
			}

		}

		if (cadenceSystem->isPedalling() && !forkAccelerometerSystem->isActive()) {
			rearSuspension->lock();
			frontSuspension->lock();
			return;
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
