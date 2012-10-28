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
	AccelerometerSystem(Configuration* config, AccelerometerSystemConfig* accelerometerSystemConfig)
			: config(config), accelerometerSystemConfig(accelerometerSystemConfig),
			  accel(accelerometerSystemConfig->address),
			  active(false),
			  currentX(0), currentY(0), currentZ(0),
			  lastActivity(0), timeout(0), instantActivity(false), readingsX(), lastReadingTime(0), moduleXZ(0) {
	}

	virtual ~AccelerometerSystem() {};

	void init() {
		accel.initialize();
		accel.setRange(accelerometerSystemConfig->range);
		accel.setRate(12);
	}

	void update(unsigned long currentTime) {
		readAccelerometer();
		moduleXZ = getModuleXZ();
		if (currentTime - lastReadingTime > 50) {
			readingsX.push(moduleXZ);
			lastReadingTime = currentTime;
		}
		if (detectActivity()) {
			if (!instantActivity) {
				activity(currentTime);
				lastActivity = currentTime;
				timeout = getTimeout();
				instantActivity = true;
			}
			active = true;
		} else if (currentTime - lastActivity <= timeout) {
			active = true;
			instantActivity = false;
		} else {
			active = false;
			instantActivity = false;
		}
	}

	virtual void readAccelerometer() {
		accel.getAcceleration(&currentX, &currentY, &currentZ);
	}

	void calibrate() {

	}

	bool isActive() {
		return active;
	}

	uint16_t getAccelerationX() {
		return currentX;
	}
	uint16_t getAccelerationZ() {
		return currentZ;
	}

	uint16_t getCurrentTimeout() {
		return timeout;
	}


	unsigned long getCalculatedTimeout() {
		return timeout;
	}

	virtual unsigned long getTimeout() = 0;

	virtual bool detectActivity() = 0;

	virtual void activity(unsigned long currentTime) = 0;


	BasicQueue<20, int16_t>* getReadingsX() {
		return &readingsX;
	}

	int16_t getModuleXZ() {
		return (currentX >= 0 ? 1 : -1) * sqrt(currentX * currentX + currentZ * currentZ);
	}

protected:
	Configuration* config;
	AccelerometerSystemConfig* accelerometerSystemConfig;
	ADXL345 accel;
	bool active;

	int16_t currentX;
	int16_t currentY;
	int16_t currentZ;

	unsigned long lastActivity;
	uint16_t timeout;
	bool instantActivity;

	BasicQueue<20, int16_t> readingsX;
	unsigned long lastReadingTime;

	int16_t moduleXZ;

};

#endif /* ACCELEROMETERSYSTEM_H_ */
