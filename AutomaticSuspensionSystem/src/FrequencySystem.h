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
	FrequencySystem(FrequencySystemConfig* frequencySystemConfig) :
			DebounceActivity(frequencySystemConfig->minTime),
			frequencySystemConfig(frequencySystemConfig), processing(false), timing() {
		pinMode(frequencySystemConfig->pin, frequencySystemConfig->referenceValue == HIGH ? INPUT : INPUT_PULLUP);
		if (frequencySystemConfig->referenceValue == LOW) {
			digitalWrite(frequencySystemConfig->pin, HIGH);
		}
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(frequencySystemConfig->pin) == frequencySystemConfig->referenceValue;
	}

	virtual void reset(unsigned long currentTime) = 0;

	virtual void start(unsigned long currentTime) = 0;

	void stop(unsigned long duration) {}

	virtual bool isProcessing() {
		return processing;
	}

	unsigned long getAverageTime() {
		return timing.getAverage();
	}

protected:
	FrequencySystemConfig* frequencySystemConfig;

	bool processing;
	NumericQueue<3> timing;
};

#endif /* FREQUENCYSYSTEM_H_ */
