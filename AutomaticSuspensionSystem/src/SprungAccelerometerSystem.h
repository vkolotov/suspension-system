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
	SprungAccelerometerSystem(Configuration* config, AccelerometerSystemConfig* accelerometerSystemConfig)
		: AccelerometerSystem(config, accelerometerSystemConfig),
		  lastMeasurement(0), averageGradient(0), gradients() {
	}

	bool detectActivity() {
		return currentX - idleValue <= accelerometerSystemConfig->severityThreshold;
	}

	void activity(unsigned long currentTime) {

	}

	unsigned long getTimeout() {
		// TODO return cadence / 2
		return 1000;
	}

	void update(unsigned long currentTime) {
		AccelerometerSystem::update(currentTime);

		if (currentTime - lastMeasurement >= config->semiautomaticStateConfig.averageDegreeMeasuringPeriod / 5
				&& currentX - idleValue < 10) {

			double current = getGradient();
			lastMeasurement = currentTime;
			gradients.push(current);
			// TODO optimize by calculating sum
			averageGradient = _getAverageGradient();
		}
	}

	int16_t getDegreeGradient() {
		return getGradient() * 180.0f / PI;
	}

	double getGradient() {
		return getRawGradient() - config->system.headTubeGradient;
	}

	double getRawGradient() {
		return atan((float)currentZ / (float)currentX);
	}

	double _getAverageGradient() {
		if (gradients.size() < 1) {
			return 0;
		}
		double sum = 0;
		gradients.iteratorReset();
		while (gradients.iteratorHasNext()) {
			sum += gradients.iteratorNext();
		}
		return sum / (double) gradients.size();
	}

	double getAverageGradient() {
		return averageGradient;
	}

	bool isActive() {
		return false;
	}

protected:
	unsigned long lastMeasurement;
	double averageGradient;
	BasicQueue<5, double> gradients;

};

#endif /* SPUNGACCELEROMETERSYSTEM_H_ */
