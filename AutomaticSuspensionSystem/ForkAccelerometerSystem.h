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
	ForkAccelerometerSystem() : threshold(400), active(false), balance(255),
			current(0), accel(), lastActivity(0) {
		Wire.begin();
		accel.initialize();
		accel.setRange(0x2);
	}

	void update(unsigned long currentTime) {
		current = accel.getAccelerationX();
		if (current - balance > threshold) {
			active = true;
			lastActivity = currentTime;
//			Serial.print(current);
//			Serial.print(" ");
//			Serial.print(balance);
//			Serial.print(" ");
//			Serial.print(threshold);
//			Serial.print(" ");
//			Serial.print(current - balance);
//			Serial.print(" ");
//			Serial.println(active);
		} else if (currentTime - lastActivity < 3000) {
			//Serial.println("time");
			active = true;
		} else {
			//Serial.println("inactive");
			active = false;
		}
	}

	void calibrate() {
		balance = accel.getAccelerationX();
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
