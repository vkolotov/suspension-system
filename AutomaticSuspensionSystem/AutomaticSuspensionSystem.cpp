#include "AutomaticSuspensionSystem.h"

void (*resetArduino)(void) = 0; //declare reset function @ address 0

unsigned long time = 0;
unsigned long lastTime = 0;

//Suspension frontSuspension(FRONT_SUSPENSION_CONTROL_PIN,
//		FRONT_SUSPENSION_FEADBACK_PIN, true);
Suspension rearSuspension(REAR_SUSPENSION_CONTROL_PIN,
		REAR_SUSPENSION_FEADBACK_PIN, false);
FrequencySystem cadenceSystem(CADENCE_PIN, MINIMUM_CADENCE_TIME, AVERAGE_CADENCE_TIME, MAXIMUM_CADENCE_TIME);
FrequencySystem speedSystem(SPEED_PIN, MINIMUM_CADENCE_TIME, AVERAGE_CADENCE_TIME, MAXIMUM_CADENCE_TIME);

//ForkAccelerometerSystem forkAccelerometerSystem;
Button frontButton(FRONT_BUTTON_PIN, false);
Button rearButton(REAR_BUTTON_PIN, false);
Button modeButton(MODE_BUTTON_PIN, true);

void setup() {
	Serial.begin(9600);
}

void loop() {
	updateMudules();
	updateApplication();
}

void updateMudules() {
	time = millis();
	if (time - lastTime > 1000) {
		Serial.println("w");
		lastTime = time;
	}

	//frontSuspension.update(time);
	rearSuspension.update(time);
	cadenceSystem.update(time);
//	forkAccelerometerSystem.update(time);
	frontButton.update(time);
	modeButton.update(time);
	rearButton.update(time);
}

void updateApplication() {
	if (modeButton.isPushed(3000)) {
		Serial.println("reset");
		resetArduino();
		return;
	}
	if (frontButton.isPushed(3000)) {
		Serial.println("Calibration");
//		forkAccelerometerSystem.calibrate();
		//frontSuspension.calibrate();
		//frontSuspension.lock();
		rearSuspension.calibrate();
		rearSuspension.lock();
		return;
	}
	if (rearButton.isPushed(3000)) {
		return;
	}
	if (modeButton.isPushed()) {
		Serial.println("Mode - true");
		if (frontButton.isPushed()) {
//			forkAccelerometerSystem.threshold += 50;
//			if (forkAccelerometerSystem.threshold > 600) {
//				forkAccelerometerSystem.threshold = 600;
//			}

		} else if (rearButton.isPushed()) {
//			forkAccelerometerSystem.threshold -= 50;
//			if (forkAccelerometerSystem.threshold < 0) {
//				forkAccelerometerSystem.threshold = 0;
//			}
		}

		if (cadenceSystem.isPedalling()/* && !forkAccelerometerSystem.isActive()*/) {
			rearSuspension.lock();
			//frontSuspension.lock();
			return;
		} else {
			rearSuspension.release();
			//frontSuspension.release();
		}
	} else {

		if (frontButton.isPushed()) {
			Serial.println("Front");
//			frontSuspension.toggle();

//			frontSuspension.calibrate();
//			delay(1000);
//			frontSuspension.lock();
//			delay(1000);
//			frontSuspension.detach();
//			delay(1000);
			rearSuspension.calibrate();
			delay(1000);
//			rearSuspension.lock();
//			delay(1000);
			//rearSuspension.detach();
			//delay(1000);
		}
		if (rearButton.isPushed()) {
			Serial.println("Rear");
			rearSuspension.toggle();
		}
	}
}
