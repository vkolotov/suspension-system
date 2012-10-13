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

	Suspension(Configuration* systemConfig, SuspensionSystemConfig* suspensionConfig)
			: Servo(), systemConfig(systemConfig), suspensionConfig(suspensionConfig),
			  lastTime(0), currentMode(0) {
		pinMode(suspensionConfig->feedbackPin, INPUT);
	}

	void lock() {
		setMode(0);
	}

	void release() {
		setMode(suspensionConfig->modes - 1);
	}

	void medium() {
		if (suspensionConfig->modes == 2) {
			lock();
		} else {
			setMode(suspensionConfig->modes / 2);
		}
	}

	void setMode(uint8_t mode) {
		if (currentMode == mode || mode < 0 || mode >= suspensionConfig->modes) {
			return;
		}
		bind();
		write(suspensionConfig->angles[mode]);
		currentMode = mode;
		suspensionConfig->mode = mode;
	}

	void toggle() {
		uint8_t nextMode = currentMode + 1;
		if (nextMode == suspensionConfig->modes) {
			nextMode = 0;
		}
		setMode(nextMode);
	}

	void update(unsigned long currentTime) {

		if (suspensionConfig->mode != currentMode) {
			setMode(suspensionConfig->mode);
		}

		if (suspensionConfig->angles[currentMode] != read()) {
			bind();
			write(suspensionConfig->angles[currentMode]);
			return;
		}
		if (attached() && currentTime - lastTime > systemConfig->powerSave.servoStandByTimeout) {
			lastTime = currentTime;
			detach();
			digitalWrite(systemConfig->powerSave.servoRelayPin, LOW);
		}
	}

	SuspensionSystemConfig* getConfig() {
		return suspensionConfig;
	}

	void bind() {
		if (!attached()) {
			digitalWrite(systemConfig->powerSave.servoRelayPin, HIGH);
			attach(suspensionConfig->controlPin);//, suspensionConfig->minAngle, suspensionConfig->maxAngle);
		}
		lastTime = millis();
	}

protected:

	Configuration* systemConfig;
	SuspensionSystemConfig* suspensionConfig;
	unsigned long lastTime;
	int8_t currentMode;

};

#endif /* SUSPENSION_H_ */
