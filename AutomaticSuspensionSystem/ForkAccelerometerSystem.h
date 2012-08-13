/*
 * ForkAccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef FORKACCELEROMETERSYSTEM_H_
#define FORKACCELEROMETERSYSTEM_H_

#include <Wire.h>
#include <ADXL345.h>

class ForkAccelerometerSystem {
public:
	ForkAccelerometerSystem() : threshold(400), active(false), balance(65),
			current(0), accel(), lastActivity(0) {
	}

	void init() {
		Wire.begin();
		accel.initialize();
		if (accel.testConnection()) {
			accel.setRange(0x2);
		}
	}

	void update(unsigned long currentTime) {
		if (accel.testConnection()) {
			current = accel.getAccelerationX();

			if (current - balance > threshold || current < 10) {
				active = true;
				lastActivity = currentTime;
			} else if (currentTime - lastActivity < 3000) {
				active = true;
			} else {
				active = false;
			}
		} else {
			active = false;
			//Serial.println("Accel is not connected");
		}
	}

	void calibrate() {
		if (accel.testConnection()) {
			balance = accel.getAccelerationX();
		}
	}

	inline bool isActive() {
		return active;
	}

	short threshold;

protected:
	bool active;
	int16_t balance;
	int16_t current;
	ADXL345 accel;
	unsigned long lastActivity;

};

#endif /* FORKACCELEROMETERSYSTEM_H_ */
