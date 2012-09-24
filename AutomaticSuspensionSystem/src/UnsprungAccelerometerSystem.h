/*
 * ForkAccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef UNSPUNGACCELEROMETERSYSTEM_H_
#define UNSPUNGACCELEROMETERSYSTEM_H_

#include <Wire.h>
#include <ADXL345.h>

struct AccelerationEvent {
	unsigned long time;
	uint16_t severity;
};

class UnsprungAccelerometerSystem {
public:
	UnsprungAccelerometerSystem(Configuration* systemConfig, AccelerometerSystemConfig* accelerometerSystemConfig)
			: systemConfig(systemConfig), accelerometerSystemConfig(accelerometerSystemConfig),
			  active(false), balance(65), current(0), direction(false), accel(accelerometerSystemConfig->address),
			  lastActivity(0), timing(), lastMeasureTime(0) {
	}

	void init() {
		Wire.begin();
		accel.initialize();

		accel.setRange(0x2);

	}

	uint8_t getSeverityLavel() {
		if (systemConfig->system.mode == MODE_MANUAL) {
			return active ? systemConfig->rearSuspension.modes : 0;
		} else {
			return 0;
		}
	}



	void update(unsigned long currentTime) {
		if (accel.testConnection()) {

			uint16_t next = accel.getAccelerationX();

			if (next - balance < accelerometerSystemConfig->noiseThreshold) {
				return;
			}


				updatePerformanceMode(currentTime, next);


		} else {
			active = false;
			//Serial.println("Accel is not connected - unsprung");
		}
	}

	void updatePerformanceMode(unsigned long currentTime, uint16_t next) {
		current = next;
		if (current - balance > accelerometerSystemConfig->severityThreshold) {
			active = true;
			timing.push(currentTime - lastActivity);
			lastActivity = currentTime;
		} else if (currentTime - lastActivity < getTimeout()) {
			active = true;
		} else {
			active = false;
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

	unsigned long getAverageActivityTime() {
		if (timing.size() < 1) {
			return 0;
		}
		unsigned long sum = 0;
		timing.iteratorReset();
		while (timing.iteratorHasNext()) {
			sum += timing.iteratorNext();
		}
		return sum / timing.size();
	}

	unsigned long getTimeout() {
//		long averageActivityTime = getAverageActivityTime();
//		long result = 10000L - (3L * averageActivityTime) / 10L;
//		if (result < 0) {
//			result = 1000;
//		} else if (result > 10000) {
//			result = 10000;
//		}
//		return result;
		return 3000;
	}

	void increaseThreshold(short threshold) {
		accelerometerSystemConfig->severityThreshold = constrain(accelerometerSystemConfig->severityThreshold + threshold, 0, 600);
	}


//protected:
	Configuration* systemConfig;
	AccelerometerSystemConfig* accelerometerSystemConfig;
	bool active;
	int16_t balance;
	int16_t current;
	bool direction;
	ADXL345 accel;
	unsigned long lastActivity;
	BasicQueue<5, unsigned long> timing;

	unsigned long lastMeasureTime;



};

#endif /* UNSPUNGACCELEROMETERSYSTEM_H_ */
