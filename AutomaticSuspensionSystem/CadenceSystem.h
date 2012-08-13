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
	CadenceSystem(unsigned char pin, unsigned short minimumTime, unsigned short maximumTime)
			: FrequencySystem(pin, minimumTime, maximumTime) {
	}

	void reset(unsigned long currentTime) {
		if (processing) {
			if (timing.size() == 0) {
				processing = currentTime - lastEvent <= maximumTime;
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
};

#endif /* CADENCESYSTEM_H_ */
