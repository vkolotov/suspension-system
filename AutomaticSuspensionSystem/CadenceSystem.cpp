/*
 * CadenceSystem.cpp
 *
 *  Created on: 09.07.2012
 *      Author: Vlad
 */

#include <CadenceSystem.h>


static const unsigned short MINIMUM_REVOLUTION_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per second
static const unsigned short MAXIMUM_REVOLUTION_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per second
static const unsigned short AVERAGE_REVOLUTION_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per second

CadenceSystem::CadenceSystem(unsigned char pin):
		pin(pin), state(LOW),
		lastRevolution(0), pedalling(false) {
	pinMode(pin, INPUT);
}

CadenceSystem::~CadenceSystem() {

}

void CadenceSystem::update() {

	unsigned short value = digitalRead(pin);
	long currentTime = millis();
	long diff = currentTime - lastRevolution;

	if (diff < MINIMUM_REVOLUTION_TIME) {
		//noise
		pedalling = true;
	} else {
		if (state != value && value == HIGH) {
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
				pedalling = currentTime - timing[timing.size() - 1] <= getAverageTime() + 500;
			}
		}
	}
	state = value;
}

unsigned short CadenceSystem::getAverageTime() {
	if (timing.size() < 2) {
		return 0;
	}
	int sum = 0;
	for(int i = 0; i < timing.size() - 1; i++)	{
		sum += timing[i + 1] - timing[i];
	}
	return sum / (timing.size() - 1);
}

bool CadenceSystem::isPedalling() {
	return pedalling;
}
