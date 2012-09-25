/*
 * AccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef ACCELEROMETERSYSTEM_H_
#define ACCELEROMETERSYSTEM_H_


class AccelerometerSystem {
public:
	AccelerometerSystem(Configuration* systemConfig, AccelerometerSystemConfig* accelerometerSystemConfig)
			: systemConfig(systemConfig), accelerometerSystemConfig(accelerometerSystemConfig),
			  accel(accelerometerSystemConfig->address),
			  active(false), idleValue(65),
			  currentX(0), currentY(0), currentZ(0),
			  lastActivity(0), timing(), last(0) {
	}

	virtual ~AccelerometerSystem() {};

	void init() {
		Wire.begin();
		accel.initialize();
		accel.setRange(accelerometerSystemConfig->range);
	}

	void update(unsigned long currentTime) {
		accel.getAcceleration(&currentX, &currentY, &currentZ);
		if (detectActivity()) {
			if (!active) {
				timing.push(currentTime - lastActivity);
				lastActivity = currentTime;
			}
			active = true;
		} else if (currentTime - lastActivity < getTimeout()) {
			active = true;
		} else {
			active = false;
		}
		last = currentX;
	}



	void calibrate() {
		idleValue = accel.getAccelerationX();
	}

	bool isActive() {
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

	virtual unsigned long getTimeout() = 0;

	virtual bool detectActivity() = 0;


protected:
	Configuration* systemConfig;
	AccelerometerSystemConfig* accelerometerSystemConfig;
	ADXL345 accel;
	bool active;
	int16_t idleValue;

	int16_t currentX;
	int16_t currentY;
	int16_t currentZ;

	unsigned long lastActivity;
	BasicQueue<5, unsigned long> timing;

	int16_t last;

};

#endif /* ACCELEROMETERSYSTEM_H_ */
