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

static const unsigned short MINIMUM_REVOLUTION_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per second
static const unsigned short MAXIMUM_REVOLUTION_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per second
static const unsigned short AVERAGE_REVOLUTION_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per second

class CadenceSystem: public DebounceActivity {
public:
	CadenceSystem(unsigned char pin) :
			DebounceActivity(MINIMUM_REVOLUTION_TIME), pin(pin), pedalling(false),
			cursor(0), length(0), sum(0), last(0) {
		pinMode(pin, INPUT);
	};

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == HIGH;
	}

	void initActivity(unsigned long currentTime) {
		if (length < 1) {
			pedalling = false;
		} else if (pedalling) {
			pedalling = currentTime - last <= getAverageTime() + 300;
		}
	}

	void startActivity(unsigned long currentTime) {
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
		last = currentTime;
	}

	void stopActivity(unsigned long duration) {}

	inline bool isPedalling() {
		return pedalling;
	}

	unsigned short getAverageTime() {
		if (length < 1) {
			return 0;
		}
		sum = 0;
		for (i = cursor, j = 0; j < length; j++) {
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
	unsigned long last;
private:
	unsigned long sum;
	unsigned char i, j;
};

#endif /* CADENCESYSTEM_H_ */
