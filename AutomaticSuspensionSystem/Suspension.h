/*
 * Suspension.h
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#ifndef SUSPENSION_H_
#define SUSPENSION_H_

#include <Servo.h>
#include <Arduino.h>
#include <Servo.h>
#include <Settings.h>

class Suspension : public Servo {

public:

	Suspension(unsigned char sPin, unsigned char fPin, bool isReverse)
			: Servo(), isLocked(false), isReverse(isReverse), lastTime(0),
			  pin(sPin), feedbackPin(fPin),
			  minAngle(30), maxAngle(160), calibrating(false) {
	}

	void lock() {
		if (isLocked) {
			return;
		}
		bind();
		isReverse ? writeMin() : writeMax();
		isLocked = true;
	}
	void release() {
		if (!isLocked) {
			return;
		}
		bind();
		isReverse ? writeMax() : writeMin();
		isLocked = false;
	}
	void toggle() {
		isLocked ? release() : lock();
	}

	void update(unsigned long currentTime) {
		if (calibrating || !attached()) {
			return;
		}
		if (currentTime - lastTime > POWER_SAVE_THRESHOLD) {
			lastTime = currentTime;
			//detach();
		}
	}

	void calibrate() {
		//pinMode(feedbackPin, INPUT);

		bind();
		calibrating = true;
		write(MAX_ANGLE / 2);
		maxAngle = _calibrate(CALIBRATION_STEP);
		minAngle = _calibrate(-CALIBRATION_STEP);
		calibrating = false;
	}

	void writeMax() {
		write(maxAngle);
	}
	void writeMin() {
		write(minAngle);
	}
	int getRawFeedback() {
		return analogRead(feedbackPin);

	}

protected:

	bool isLocked;
	bool isReverse;
	unsigned long lastTime;

	unsigned char pin;
	unsigned char feedbackPin;

	unsigned short minAngle;
	unsigned short maxAngle;
	bool calibrating;

	unsigned short _calibrate(char calibrationStep) {
		short feedback = getRawFeedback();
		short currentFeedback = 0;

		for (unsigned short angle = read() + calibrationStep; ; ) {
			//Serial.println(angle);
			write(angle);
			delay(CALIBRATION_DELAY);

			currentFeedback = getRawFeedback();
			Serial.print(currentFeedback);
			Serial.print(" ");
			Serial.println(abs(currentFeedback - feedback));

			if (abs(currentFeedback - feedback) < CALIBRATION_THRESHOLD) {
				Serial.println("Thresh");
				write(angle - calibrationStep);
				return angle - calibrationStep;
			}
			feedback = currentFeedback;

			angle = angle + calibrationStep;
			if (angle <= MIN_ANGLE) {
				return MIN_ANGLE;
			}
			if (angle >= MAX_ANGLE) {
				return MAX_ANGLE;
			}
		}
		return 0;
	}

	void bind() {
		if (!attached()) {
			attach(pin);
		}
		lastTime = millis();
	}
};

#endif /* SUSPENSION_H_ */
