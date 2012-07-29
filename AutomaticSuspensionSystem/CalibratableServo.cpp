/*
 * CalibratableServo.cpp
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#include "CalibratableServo.h"

static const unsigned char CALIBRATION_DELAY = 200;
static const unsigned char CALIBRATION_THRESHOLD = 5;
static const unsigned char CALIBRATION_STEP = 10;
static const unsigned char MAX_ANGLE = 180;

CalibratableServo::CalibratableServo()
		: Servo(), pin(FRONT_SUSPENSION_CONTROL_PIN), feedbackPin(FRONT_SUSPENSION_FEADBACK_PIN),
		  minAngle(0), maxAngle(MAX_ANGLE), calibrating(false) {
}

CalibratableServo::CalibratableServo(unsigned char pin, unsigned char feedbackPin)
		: Servo(), pin(pin), feedbackPin(feedbackPin),
		  minAngle(0), maxAngle(MAX_ANGLE), calibrating(false) {
	pinMode(feedbackPin, INPUT);
}

CalibratableServo::~CalibratableServo() {

}

void CalibratableServo::bind() {
	this->attach(pin);
}

void CalibratableServo::calibrate() {
	calibrating = true;
	unsigned short readAngle = read();
	unsigned short feedback = getRawFeedback();

	for (unsigned short angle = readAngle + CALIBRATION_STEP; ; ) {

		write(angle);
		delay(CALIBRATION_DELAY);

		unsigned short currentFeedback = getRawFeedback();

		if (currentFeedback - feedback < CALIBRATION_THRESHOLD) {
			maxAngle = angle - CALIBRATION_STEP;
			write(angle - CALIBRATION_STEP * 2);
			break;
		}
		feedback = currentFeedback;

		if (angle == MAX_ANGLE) {
			maxAngle = MAX_ANGLE;
			break;
		}
		angle = constrain(angle + CALIBRATION_STEP, 0, MAX_ANGLE);
	}

	feedback = getRawFeedback();
	readAngle = read();
	for (unsigned short angle = readAngle - CALIBRATION_STEP; ; ) {

		write(angle);
		delay(CALIBRATION_DELAY);

		int currentFeedback = getRawFeedback();
		if (feedback - currentFeedback < CALIBRATION_THRESHOLD) {
			minAngle = angle + CALIBRATION_STEP;
			write(angle + CALIBRATION_STEP * 2);
			break;
		}
		feedback = currentFeedback;

		if (angle == 0) {
			minAngle = 0;
			break;
		}
		angle = constrain(angle - CALIBRATION_STEP, 0, MAX_ANGLE);
	}
	calibrating = false;
}

void CalibratableServo::writeMax() {
	write(maxAngle);
}
void CalibratableServo::writeMin() {
	write(minAngle);
}

int CalibratableServo::getRawFeedback() {
	return analogRead(feedbackPin);
}

bool CalibratableServo::isCalibrating() {
	return calibrating;
}
