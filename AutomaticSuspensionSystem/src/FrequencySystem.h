/*
 * FrequencySystem.h
 *
 *  Created on: 06.08.2012
 *      Author: vkolotov
 */

#ifndef FREQUENCYSYSTEM_H_
#define FREQUENCYSYSTEM_H_

#include <DebounceActivity.h>
#include <Arduino.h>
#include <Settings.h>
#include <BasicQueue.h>

class FrequencySystem: public DebounceActivity {
public:
	FrequencySystem(FrequencySystemConfig frequencySystemConfig) :
			DebounceActivity(frequencySystemConfig.minTime),
			frequencySystemConfig(frequencySystemConfig), processing(false), timing() {
		pinMode(frequencySystemConfig.pin, INPUT);
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(frequencySystemConfig.pin) == frequencySystemConfig.referenceValue;
	}

	virtual void reset(unsigned long currentTime) = 0;

	virtual void start(unsigned long currentTime) = 0;

	void stop(unsigned long duration) {}

	virtual bool isProcessing() {
		return processing;
	}

	unsigned short getAverageTime() {
		if (timing.size() < 1) {
			return 0;
		}
		unsigned long sum = 0;
		timing.iteratorReset();
		while (timing.iteratorHasNext()) {
			sum += timing.iteratorNext();
		}
		return sum / timing.size();
	}

protected:
	FrequencySystemConfig frequencySystemConfig;

	bool processing;
	BasicQueue<3, unsigned long> timing;
};

#endif /* FREQUENCYSYSTEM_H_ */
