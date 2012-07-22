/*
 * CadenceSystem.cpp
 *
 *  Created on: 09.07.2012
 *      Author: Vlad
 */

#include <CadenceSystem.h>

static const long MINIMUM_REVOLUTION_TIME = (60 / 180) * 1000; // 180 revolutions per second

CadenceSystem::CadenceSystem():
		pin(CADENCE_PIN), state(LOW),
		lastRevolution(0), previousRevolution(0) {
	pinMode(pin, INPUT);
}

CadenceSystem::~CadenceSystem() {

}

void CadenceSystem::update() {

	int value = digitalRead(4);
	long currentTime = millis();

	long diff = currentTime - lastRevolution;

	if (state != value && value == HIGH
			&& diff > MINIMUM_REVOLUTION_TIME) {
		previousRevolution = lastRevolution;
		lastRevolution = currentTime;
	}
	state = value;
}

int CadenceSystem::getPriority() {
	return HIGH_PRIORITY;
}

int CadenceSystem::getCadence() {
	return 0;
}

int CadenceSystem::getAverageCadence() {
	return 0;
}

bool CadenceSystem::isPedalling() {
	long currentTime = millis();
	long diff = currentTime - lastRevolution;
	return diff < previousRevolution - lastRevolution + 1000;
}
