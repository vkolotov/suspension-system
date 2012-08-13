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
	SpeedSystem(unsigned char pin, unsigned short minimumTime, unsigned short maximumTime)
			: FrequencySystem(pin, minimumTime, maximumTime) {
	}

	void reset(unsigned long currentTime) {
		if (currentTime - lastEvent >= maximumTime) {
			timing.clear();
			return;
		}
	}

	void start(unsigned long currentTime) {
		if (currentTime - lastEvent < maximumTime) {
			timing.push(currentTime - lastEvent);
		}
	}

	bool isProcessing() {
		return (int(getAverageSpeed() * 3.6f)) >= 3;
	}

	float getAverageSpeed() {
		return ((float)WHEEL_LENGTH) / ((float)getAverageTime());
	}
};

#endif /* SPEEDSYSTEM_H_ */
