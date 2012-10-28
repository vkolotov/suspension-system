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
		  rawGradients(), filteredGradients() {
	}

	virtual void readAccelerometer() {
		accel.getAcceleration(&currentX, &currentY, &currentZ);
		currentX = -currentX;
	}

	bool detectActivity() {
		return moduleXZ <= accelerometerSystemConfig->severityThreshold;
	}

	void activity(unsigned long currentTime) {

	}

	unsigned long getTimeout() {
		// TODO return cadence / 2
		return 1000;
	}

	void update(unsigned long currentTime) {
		AccelerometerSystem::update(currentTime);

		if (currentTime - lastMeasurement > 50) {
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
		return atan((double)currentZ / -(double)currentX);
	}

	float getAverageGradient() {
		return averageGradient;
	}

	BasicQueue<20, int16_t>* getFilteredGradients() {
		return &filteredGradients;
	}

	BasicQueue<20, int16_t>* getRawGradients() {
		return &rawGradients;
	}

protected:

	unsigned long lastMeasurement;
	float averageGradient;
	ComplementaryFilter<float> filter;

	BasicQueue<20, int16_t> rawGradients;
	BasicQueue<20, int16_t> filteredGradients;
};

#endif /* SPUNGACCELEROMETERSYSTEM_H_ */
