/*
 * SpeedSystem.h
 *
 *  Created on: 13.08.2012
 *      Author: Vlad
 */

#ifndef SPEEDSYSTEM_H_
#define SPEEDSYSTEM_H_
#include <FrequencySystem.h>

class SpeedSystem : public FrequencySystem {
public:
	SpeedSystem(SpeedSystemConfig* speedSystemConfig)
		: FrequencySystem(&(speedSystemConfig->frequencySystemConfig)),
		  speedSystemConfig(speedSystemConfig) {
	}

	void reset(unsigned long currentTime) {
		if (currentTime - lastEvent >= frequencySystemConfig->maxTime) {
			timing.clear();
			return;
		}
	}

	void start(unsigned long currentTime) {
		if (lastEvent > 0 && currentTime - lastEvent < frequencySystemConfig->maxTime) {
			timing.push(currentTime - lastEvent);
		}
	}

	// meters per hour or mm per msec
	float getAverageSpeed() {
		return ((float)(speedSystemConfig->wheelLength)) / ((float)getAverageTime());
	}

	float getAverageSpeedKmH() {
		return getAverageSpeed() * 3.6f;
	}

protected:
	SpeedSystemConfig* speedSystemConfig;
};

#endif /* SPEEDSYSTEM_H_ */
