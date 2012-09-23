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

	Suspension(Configuration systemConfig, SuspensionSystemConfig suspensionConfig)
			: Servo(), systemConfig(systemConfig), suspensionConfig(suspensionConfig),
			  lastTime(0), calibrating(false), currentMode(0) {
		pinMode(suspensionConfig.feedbackPin, INPUT);
	}

	void lock() {
		setMode(0);
	}

	void release() {
		setMode(suspensionConfig.modes - 1);
	}

	void medium() {
		if (suspensionConfig.modes == 2) {
			lock();
		} else {
			setMode(suspensionConfig.modes / 2);
		}
	}

	void setMode(uint8_t mode) {
		if (currentMode == mode || mode < 0 || mode >= suspensionConfig.modes) {
			return;
		}
		bind();
		write(suspensionConfig.angles[mode]);
		currentMode = mode;
	}

	void toggle() {
		uint8_t nextMode = currentMode + 1;
		if (nextMode == suspensionConfig.modes) {
			nextMode = 0;
		}
		setMode(nextMode);
	}

	void update(unsigned long currentTime) {
		if (calibrating || !attached()) {
			return;
		}
		if (currentTime - lastTime > systemConfig.powerSave.servoStandByTimeout) {
			lastTime = currentTime;
			detach();
		}
	}

	void calibrate() {
		bind();
		calibrating = true;
		write(suspensionConfig.angles[suspensionConfig.modes - 1]);
		delay(500);
		suspensionConfig.angles[suspensionConfig.modes - 1] = _calibrate(suspensionConfig.calibrationStep);
		suspensionConfig.angles[0] = _calibrate(-suspensionConfig.calibrationStep);
		calibrating = false;
	}

	int getRawFeedback() {
		return analogRead(suspensionConfig.feedbackPin);
	}

	void increaseMediumAngle(int8_t angle) {
		uint8_t mediumMode = suspensionConfig.modes / 2;
		suspensionConfig.angles[mediumMode] =
				constrain(suspensionConfig.angles[mediumMode] + angle,
						suspensionConfig.angles[0], suspensionConfig.angles[suspensionConfig.modes - 1]);
		bind();
		write(suspensionConfig.angles[mediumMode]);
	}

protected:

	Configuration systemConfig;
	SuspensionSystemConfig suspensionConfig;
	unsigned long lastTime;
	bool calibrating;
	uint8_t currentMode;

	unsigned short _calibrate(char calibrationStep) {
		short feedback = getRawFeedback();
		short currentFeedback = 0;

		for (unsigned short angle = read() + calibrationStep; ; ) {
			write(angle);
			delay(suspensionConfig.calibrationDelay);
			currentFeedback = getRawFeedback();
			if (abs(currentFeedback - feedback) < suspensionConfig.calibrationDelay) {
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
			attach(suspensionConfig.controlPin);
		}
		lastTime = millis();
	}
};

#endif /* SUSPENSION_H_ */
