/*
 * CalibratableServo.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "CalibratableServo.h"

static const int CALIBRATION_DELAY = 200;
static const int CALIBRATION_THRESHOLD = 5;
static const int CALIBRATION_STEP = 10;

CalibratableServo::CalibratableServo()
		: Servo(), pin(FRONT_SUSPENSION_CONTROL_PIN), feedbackPin(FRONT_SUSPENSION_FEADBACK_PIN),
		  calibrationStep(CALIBRATION_STEP), calibrationThreshold(CALIBRATION_THRESHOLD),
		  calibrationDelay(CALIBRATION_DELAY), calibrated(false),
		  minAngle(0), maxAngle(180) {
}

CalibratableServo::CalibratableServo(int pin, int feedbackPin)
		: Servo(), pin(pin), feedbackPin(feedbackPin),
		  calibrationStep(CALIBRATION_STEP), calibrationThreshold(CALIBRATION_THRESHOLD),
		  calibrationDelay(CALIBRATION_DELAY), calibrated(false),
		  minAngle(0), maxAngle(180) {
	pinMode(feedbackPin, INPUT);
}

CalibratableServo::~CalibratableServo() {

}

void CalibratableServo::bind() {
	this->attach(pin);
}

void CalibratableServo::calibrate() {

	int readAngle = read();
	int feedback = getRawFeedback();

	for (int angle = readAngle + calibrationStep; ; ) {

		write(angle);
		delay(calibrationDelay);

		int currentFeedback = getRawFeedback();

		if (currentFeedback - feedback < calibrationThreshold) {
			maxAngle = angle - calibrationStep;
			write(angle - calibrationStep * 2);
			break;
		}
		feedback = currentFeedback;

		if (angle == 180) {
			maxAngle = 180;
			break;
		}
		angle = constrain(angle + calibrationStep, 0, 180);
	}

	feedback = getRawFeedback();
	readAngle = read();
	for (int angle = readAngle - calibrationStep; ; ) {

		write(angle);
		delay(calibrationDelay);

		int currentFeedback = getRawFeedback();
		if (feedback - currentFeedback < calibrationThreshold) {
			minAngle = angle + calibrationStep;
			write(angle + calibrationStep * 2);
			break;
		}
		feedback = currentFeedback;

		if (angle == 0) {
			minAngle = 0;
			break;
		}
		angle = constrain(angle - calibrationStep, 0, 180);
	}
	calibrated = true;
}

bool CalibratableServo::isCalibrated() {
	return calibrated;
}

void CalibratableServo::writeMax() {
	write(getMaxAngle());
}
void CalibratableServo::writeMin() {
	write(getMinAngle());
}

int CalibratableServo::getMaxAngle() {
	return maxAngle;
}
int CalibratableServo::getMinAngle() {
	return minAngle;
}

int CalibratableServo::getRawFeedback() {
	return analogRead(feedbackPin);
}

