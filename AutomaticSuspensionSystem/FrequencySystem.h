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

class FrequencySystem: public DebounceActivity {
public:
	FrequencySystem(unsigned char pin, unsigned short minimumTime,
			unsigned short averageTime, unsigned short maximumTime) :
			DebounceActivity(minimumTime), pin(pin), processing(false),
			cursor(0), length(0), averageTime(averageTime), maximumTime(maximumTime) {
		pinMode(pin, INPUT);
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == HIGH;
	}

	void idle(unsigned long currentTime) {
		if (length < 1) {
			processing = false;
		} else if (processing) {
			//TODO introduce setting variable
			processing = currentTime - last <= getAverageTime() + 300;
		}
	}

	void start(unsigned long currentTime) {
		processing = true;
		if (currentTime - last > maximumTime) {
			cursor = 0;
			timing[cursor] = averageTime;
			length = 1;
		} else {
			if (cursor >= 4) {
				cursor = 0;
			} else {
				cursor++;
			}
			if (length < 5) {
				length++;
			}
			timing[cursor] = currentTime - last;
		}
	}

	void stop(unsigned long duration) {}

	inline bool isProcessing() {
		return processing;
	}

	unsigned short getAverageTime() {
		if (length < 1) {
			return 0;
		}
		unsigned long sum = 0;
		for (unsigned char i = cursor, j = 0; j < length; j++) {
			sum += timing[i];
			if (i == length - 1) {
				i = 0;
			} else {
				i++;
			}
		}
		return sum / length ;
	}

protected:
	unsigned char pin;
	bool processing;
	unsigned char cursor;
	unsigned char length;
	unsigned long timing[5];

	unsigned short averageTime;
	unsigned short maximumTime;
};

#endif /* FREQUENCYSYSTEM_H_ */
