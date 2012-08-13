#include "AutomaticSuspensionSystem.h"

void (*resetArduino)(void) = 0; //declare reset function @ address 0

unsigned long time = 0;
unsigned long lastTime = 0;

Suspension frontSuspension(FRONT_SUSPENSION_CONTROL_PIN,
		FRONT_SUSPENSION_FEADBACK_PIN, true);
Suspension rearSuspension(REAR_SUSPENSION_CONTROL_PIN,
		REAR_SUSPENSION_FEADBACK_PIN, false);
CadenceSystem cadenceSystem(CADENCE_PIN, MINIMUM_CADENCE_TIME, MAXIMUM_CADENCE_TIME);
SpeedSystem speedSystem(SPEED_PIN, MINIMUM_SPEED_TIME, MAXIMUM_SPEED_TIME);

ForkAccelerometerSystem forkAccelerometerSystem;
Button frontButton(FRONT_BUTTON_PIN, false);
Button rearButton(REAR_BUTTON_PIN, false);
Button modeButton(MODE_BUTTON_PIN, true);

void setup() {
	Serial.begin(9600);
	forkAccelerometerSystem.init();
}

void loop() {
	updateMudules();
	updateApplication();
}

void updateMudules() {
	time = millis();

	frontSuspension.update(time);
	rearSuspension.update(time);
	cadenceSystem.update(time);
	speedSystem.update(time);
	forkAccelerometerSystem.update(time);
	frontButton.update(time);
	modeButton.update(time);
	rearButton.update(time);

//	if (time - lastTime > 1000) {
//		Serial.print("Cadence: ");
//		Serial.print(cadenceSystem.isProcessing());
//		Serial.print(" ");
//		Serial.println(cadenceSystem.getAverageTime());
//		lastTime = time;
//	}
}

void updateApplication() {

	if (modeButton.isPushed(3000)) {
		resetArduino();
		return;
	}
	if (frontButton.isPushed(3000)) {
		forkAccelerometerSystem.calibrate();
		frontSuspension.calibrate();
		frontSuspension.lock();
		delay(500);
		frontSuspension.detach();
		rearSuspension.calibrate();
		rearSuspension.lock();
		return;
	}
	if (rearButton.isPushed(3000)) {
		return;
	}
	if (modeButton.isPushed()) {

		if (frontButton.isPushed()) {
			forkAccelerometerSystem.threshold += 25;
			if (forkAccelerometerSystem.threshold > 600) {
				forkAccelerometerSystem.threshold = 600;
			}
		} else if (rearButton.isPushed()) {
			forkAccelerometerSystem.threshold -= 25;
			if (forkAccelerometerSystem.threshold < 0) {
				forkAccelerometerSystem.threshold = 0;
			}
		}

		if (speedSystem.isProcessing()) {
			if (cadenceSystem.isProcessing() && !forkAccelerometerSystem.isActive()) {
				rearSuspension.lock();
				frontSuspension.lock();
				return;
			} else {
				rearSuspension.release();
				frontSuspension.release();
			}
		} else {
			rearSuspension.release();
			frontSuspension.release();
		}
	} else {

		if (frontButton.isPushed()) {
			frontSuspension.toggle();
		}
		if (rearButton.isPushed()) {
			rearSuspension.toggle();
		}
	}
}

