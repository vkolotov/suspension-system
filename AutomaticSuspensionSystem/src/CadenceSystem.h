/*
 * CadenceSystem.h
 *
 *  Created on: 09.07.2012
 *      Author: Vlad
 */

#ifndef CADENCESYSTEM_H_
#define CADENCESYSTEM_H_
#include <FrequencySystem.h>
#include <Arduino.h>
#include <Settings.h>

class CadenceSystem: public FrequencySystem {
public:
	CadenceSystem(CadenceSystemConfig cadenceSystemConfig)
			: FrequencySystem(cadenceSystemConfig.frequencySystemConfig) {
	}

	void reset(unsigned long currentTime) {
		if (processing) {
			if (timing.size() == 0) {
				processing = currentTime - lastEvent <= frequencySystemConfig.maxTime;
			} else {
				processing = currentTime - lastEvent <= getAverageTime() + 300;
			}
			if (!processing) {
				timing.clear();
			}
		}
	}

	void start(unsigned long currentTime) {
		if (processing) {
			timing.push(currentTime - lastEvent);
		}
		processing = true;
	}

	uint16_t getCadence() {
		unsigned short time = getAverageTime();
		if (time == 0) {
			return 0;
		}
		return 60000 / time;
	}
};

#endif /* CADENCESYSTEM_H_ */
