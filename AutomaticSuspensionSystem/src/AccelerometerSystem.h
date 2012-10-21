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
			  active(false), idleX(65), idleZ(0),
			  currentX(0), currentY(0), currentZ(0),
			  lastActivity(0), timeout(0), instantActivity(false) {
	}

	virtual ~AccelerometerSystem() {};

	void init() {
		accel.initialize();
		accel.setRange(accelerometerSystemConfig->range);
	}

	void update(unsigned long currentTime) {
		readAccelerometer();

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
		idleX = getAccelerationX();
		idleZ = getAccelerationZ();
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

	int16_t getIdleValue() {
		return idleX;
	}

	unsigned long getCalculatedTimeout() {
		return timeout;
	}

	virtual unsigned long getTimeout() = 0;

	virtual bool detectActivity() = 0;

	virtual void activity(unsigned long currentTime) = 0;


protected:
	Configuration* config;
	AccelerometerSystemConfig* accelerometerSystemConfig;
	ADXL345 accel;
	bool active;
	int16_t idleX;
	int16_t idleZ;

	int16_t currentX;
	int16_t currentY;
	int16_t currentZ;

	unsigned long lastActivity;
	uint16_t timeout;
	bool instantActivity;

};

#endif /* ACCELEROMETERSYSTEM_H_ */
