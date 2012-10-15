/*
 * SprungAccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef SPUNGACCELEROMETERSYSTEM_H_
#define SPUNGACCELEROMETERSYSTEM_H_


class SprungAccelerometerSystem: public AccelerometerSystem {
public:
	SprungAccelerometerSystem(Configuration* config, SprungAccelerometerSystemConfig* sprungAccelerometerSystemConfig)
		: AccelerometerSystem(config, &sprungAccelerometerSystemConfig->accelerometerSystemConfig),
		  lastMeasurement(0), averageGradient(0.0f),
		  filter(sprungAccelerometerSystemConfig->angleFilterAlpha, sprungAccelerometerSystemConfig->angleFilterBeta) {
	}

	bool detectActivity() {
		return currentX - idleX <= accelerometerSystemConfig->severityThreshold;
	}

	void activity(unsigned long currentTime) {

	}

	unsigned long getTimeout() {
		// TODO return cadence / 2
		return 1000;
	}

	void update(unsigned long currentTime) {
		AccelerometerSystem::update(currentTime);
		int16_t delta = getDelta();
		if (delta < 15 && delta > -10 && currentTime - lastMeasurement > 50) {
			averageGradient = filter.next(getInstantGradient(), currentTime - lastMeasurement);
			lastMeasurement = currentTime;
		}
	}

	int16_t getDelta() {
		return abs(currentX) + abs(currentZ) - (abs(idleX) + abs(idleZ));
	}

	int16_t getAverageDegreeGradient() {
		return averageGradient * 180.0f / PI;
	}

	float getInstantGradient() {
		return getRawGradient() - config->system.headTubeGradient;
	}

	float getRawGradient() {
		return atan((double)currentZ / (double)currentX);
	}

	float getAverageGradient() {
		return averageGradient;
	}

	bool isActive() {
		return false;
	}

protected:

	unsigned long lastMeasurement;
	float averageGradient;
	ComplementaryFilter<float> filter;
};

#endif /* SPUNGACCELEROMETERSYSTEM_H_ */
