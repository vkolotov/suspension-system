/*
 * ForkAccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef SPUNGACCELEROMETERSYSTEM_H_
#define SPUNGACCELEROMETERSYSTEM_H_


class SprungAccelerometerSystem {
public:
	SprungAccelerometerSystem(Configuration config, AccelerometerSystemConfig accelerometerSystemConfig)
		: config(config), accelerometerSystemConfig(accelerometerSystemConfig),
		  accel(accelerometerSystemConfig.address),
		  currentX(0), currentY(0), currentZ(0),
		  lastMeasurement(0), averageGradient(0), gradients() {
	}

	void init() {
		Wire.begin();
		accel.initialize();
		accel.setRange(accelerometerSystemConfig.range);
	}

	void update(unsigned long currentTime) {

		accel.getAcceleration(&currentX, &currentY, &currentZ);

		if (currentTime - lastMeasurement >=
						config.semiautomaticStateConfig.averageDegreeMeasuringPeriod / 5) {
			double current = getGradient();
			lastMeasurement = currentTime;
			gradients.push(current);
			// TODO optimize by calculating sum
			averageGradient = _getAverageGradient();
		}


	}

	int16_t getDegreeGradient() {
		return -atan((float)currentZ / (float)currentX) * 180.0f / PI;
	}

	double getGradient() {
		return -atan((float)currentZ / (float)currentX);
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

protected:

	Configuration config;
	AccelerometerSystemConfig accelerometerSystemConfig;
	ADXL345 accel;

	int16_t currentX;
	int16_t currentY;
	int16_t currentZ;

	unsigned long lastMeasurement;
	double averageGradient;
	BasicQueue<5, double> gradients;


};

#endif /* SPUNGACCELEROMETERSYSTEM_H_ */
