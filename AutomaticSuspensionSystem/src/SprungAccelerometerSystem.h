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
		  filter(sprungAccelerometerSystemConfig->angleFilterAlpha, sprungAccelerometerSystemConfig->angleFilterBeta),
		  rawGradients(), filteredGradients(), rockingTiming() {
	}

	virtual void readAccelerometer() {
		accel.getAcceleration(&currentX, &currentY, &currentZ);
		currentX = -currentX;
		currentZ = -currentZ;
	}

	bool detectActivity() {
		return abs(module) >= abs(accelerometerSystemConfig->severityThreshold);
	}

	void activity(unsigned long currentTime) {
		//rockingTiming.push(currentTime - lastActivity);
	}

	void inactivity() {
		//rockingTiming.clear();
	}

	unsigned long getTimeout() {
		// TODO return cadence / 2
		return MAXIMUM_CADENCE_TIME;
	}

	bool isActive() {
		return active;// && rockingTiming.size() == 3;
	}

	void update(unsigned long currentTime) {
		AccelerometerSystem::update(currentTime);

		if (module < 40 && module > 20 && currentTime - lastMeasurement > 100) {
			averageGradient = filter.next(getInstantGradient(), currentTime - lastMeasurement);
			lastMeasurement = currentTime;
			filteredGradients.push(getAverageDegreeGradient());
			rawGradients.push(getInstantGradient() * 180.0f / PI);
		}
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

	BasicQueue<10, int16_t>* getFilteredGradients() {
		return &filteredGradients;
	}

	BasicQueue<10, int16_t>* getRawGradients() {
		return &rawGradients;
	}

	int16_t getModule() {
		return currentY;
	}

protected:

	unsigned long lastMeasurement;
	float averageGradient;
	ComplementaryFilter<float> filter;

	BasicQueue<10, int16_t> rawGradients;
	BasicQueue<10, int16_t> filteredGradients;

	NumericQueue<3> rockingTiming;
};

#endif /* SPUNGACCELEROMETERSYSTEM_H_ */
