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
	FrequencySystem(unsigned char pin, unsigned short minimumTime, unsigned short maximumTime) :
			DebounceActivity(minimumTime), pin(pin), processing(false),
			timing(), maximumTime(maximumTime) {
		pinMode(pin, INPUT);
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == HIGH;
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
	unsigned char pin;
	bool processing;
	BasicQueue<3, unsigned long> timing;

	unsigned short maximumTime;
};

#endif /* FREQUENCYSYSTEM_H_ */
