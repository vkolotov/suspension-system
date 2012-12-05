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
			  lastActivity(0), timeout(0), instantActivity(false), readings(), lastReadingTime(0),
			  module(0), maxModule(0), minModule(0), maxTime(0), minTime(0) {
	}

	virtual ~AccelerometerSystem() {};

	void init() {
		accel.initialize();
		accel.setRange(accelerometerSystemConfig->range);
		accel.setRate(15);
		accel.setFullResolution(0);
		accel.setFIFOMode(0);
	}

	void update(unsigned long currentTime) {
		readAccelerometer();

		updateReadings(currentTime);

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
			inactivity();
		} else {
			active = false;
			instantActivity = false;
		}
	}

	virtual void readAccelerometer() {
		accel.getAcceleration(&currentX, &currentY, &currentZ);
	}

	virtual bool isActive() {
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

	virtual void inactivity() = 0;


	BasicQueue<20, int16_t>* getReadings() {
		return &readings;
	}

	int16_t getModuleXZ() {
		return (currentX >= 0 ? 1 : -1) * sqrt((double)currentX * (double)currentX + (double)currentZ * (double)currentZ);
	}

	int16_t getModuleXY() {
		return (currentX >= 0 ? 1 : -1) * sqrt((double)currentX * (double)currentX + (double)currentY * (double)currentY);
	}

	virtual int16_t getModule() = 0;


protected:

	void updateReadings(unsigned long currentTime) {

		module = getModule();

		if (module < minModule) {
			minModule = module;
			minTime = currentTime;
		}

		if (module > maxModule) {
			maxModule = module;
			maxTime = currentTime;
		}

		if (currentTime - lastReadingTime > 100) {
			lastReadingTime = currentTime;
			addMinMaxReadings();
			minModule = module;
			maxModule = module;
		}
	}

	void addMinMaxReadings() {
		if (maxTime >= minTime) {
			readings.push(minModule);
			readings.push(maxModule);
		} else {
			readings.push(maxModule);
			readings.push(minModule);
		}
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

	BasicQueue<20, int16_t> readings;
	unsigned long lastReadingTime;

	int16_t module;
	int16_t maxModule;
	int16_t minModule;

	unsigned long maxTime;
	unsigned long minTime;

};

#endif /* ACCELEROMETERSYSTEM_H_ */
