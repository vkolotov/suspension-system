/*
 * UnsprungAccelerometerSystem.h
 *
 *  Created on: 26.09.2012
 *      Author: Vlad
 */

#ifndef UNSPRUNGACCELEROMETERSYSTEM_H_
#define UNSPRUNGACCELEROMETERSYSTEM_H_

class UnsprungAccelerometerSystem : public AccelerometerSystem {
public:
	UnsprungAccelerometerSystem(Configuration* systemConfig, AccelerometerSystemConfig* accelerometerSystemConfig)
			: AccelerometerSystem(systemConfig, accelerometerSystemConfig) {};
	~UnsprungAccelerometerSystem() {} ;

	bool detectActivity() {
		return currentX - idleValue >= accelerometerSystemConfig->severityThreshold;
	}

	unsigned long getTimeout() {
		long averageActivityTime = getAverageActivityTime();
		long result = 10000L - (3L * averageActivityTime) / 10L;
		if (result < 0) {
			result = 1000;
		} else if (result > 10000) {
			result = 10000;
		}
		return result;
	}

};

#endif /* UNSPRUNGACCELEROMETERSYSTEM_H_ */
