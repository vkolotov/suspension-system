/*
 * CadenceSystem.h
 *
 *  Created on: 09.07.2012
 *      Author: Vlad
 */

#ifndef CADENCESYSTEM_H_
#define CADENCESYSTEM_H_
#include <DebounceActivity.h>
#include <Arduino.h>
#include <Settings.h>

class CadenceSystem: public DebounceActivity {
public:
	CadenceSystem(unsigned char pin) :
			DebounceActivity(MINIMUM_CADENCE_TIME), pin(pin), pedalling(false),
			cursor(0), length(0) {
		pinMode(pin, INPUT);
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == HIGH;
	}

	void idle(unsigned long currentTime) {
		if (length < 1) {
			pedalling = false;
		} else if (pedalling) {
			//TODO introduce setting variable
			pedalling = currentTime - last <= getAverageTime() + 300;
		}
	}

	void start(unsigned long currentTime) {
		pedalling = true;
		if (currentTime - last > MAXIMUM_REVOLUTION_TIME) {
			cursor = 0;
			timing[cursor] = AVERAGE_REVOLUTION_TIME;
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

	inline bool isPedalling() {
		return pedalling;
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
	bool pedalling;
	unsigned char cursor;
	unsigned char length;
	unsigned long timing[5];
};

#endif /* CADENCESYSTEM_H_ */
