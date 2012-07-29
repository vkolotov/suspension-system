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
#include <StandardCplusplus.h>
#include <vector>

static const unsigned short MINIMUM_REVOLUTION_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per second
static const unsigned short MAXIMUM_REVOLUTION_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per second
static const unsigned short AVERAGE_REVOLUTION_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per second

using namespace std;

class CadenceSystem: public DebounceActivity {
public:
	CadenceSystem(unsigned char pin) :
			DebounceActivity(MINIMUM_REVOLUTION_TIME), pin(pin), pedalling(false), timing() {
		pinMode(pin, INPUT);
	};

	inline bool isPedalling() {
		return pedalling;
	}

	unsigned short getAverageTime() {
		if (timing.size() < 2) {
			return 0;
		}
		int sum = 0;
		for (int i = 0; i < timing.size() - 1; i++) {
			sum += timing[i + 1] - timing[i];
		}
		return sum / (timing.size() - 1);
	}

	void debounce(bool event) {
		long currentTime = millis();
		if (event) {
			pedalling = true;
			if (currentTime - timing[timing.size() - 1] > MAXIMUM_REVOLUTION_TIME) {
				timing.clear();
			}
			if (timing.size() == 0) {
				timing.push_back(currentTime - AVERAGE_REVOLUTION_TIME);
			}
			timing.push_back(currentTime);
			if (timing.size() > 10) {
				timing.erase(timing.begin());
			}
		} else {
			if (timing.size() < 2) {
				pedalling = false;
			} else {
				pedalling = currentTime - timing[timing.size() - 1]
						<= getAverageTime() + 300;
			}
		}
	};

	bool event() {
		return digitalRead(pin) == HIGH;
	}

protected:
	unsigned char pin;
	bool pedalling;
	vector<long> timing;
};

#endif /* CADENCESYSTEM_H_ */
