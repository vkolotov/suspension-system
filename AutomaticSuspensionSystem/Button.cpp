/*
 * Button.cpp
 *
 *  Created on: 26.07.2012
 *      Author: Vlad
 */

#include "Button.h"

Button::Button(): pin(0), lastTime(0), pushed(false), isToggle(false) {

}

Button::Button(unsigned char pin, bool isToggle): pin(pin), lastTime(0), pushed(false), isToggle(isToggle) {
	pinMode(pin, INPUT);
}

void Button::update() {
	if (checkNoise()) {
		if (digitalRead(pin) == HIGH) {
			if (isToggle) {
				pushed = !pushed;
			} else {
				pushed = true;
			}
		} else {

		}
	}
	if (digitalRead(pin) == HIGH) {
		long current = millis();
		if (current - lastTime > 500) {
			lastTime = current;
			if (!isToggle) {
				pushed = true;
			} else {
				pushed = !pushed;
			}
		} else if (!isToggle) {
			pushed = false;
		}
	} else if (!isToggle) {
		pushed = false;
	}
}

bool Button::isPushed() {
	if (digitalRead(pin) == HIGH) {
		long current = millis();
		if (current - lastTime > 500) {
			lastTime = current;
			if (!isToggle) {
				pushed = true;
			} else {
				pushed = !pushed;
			}
		} else if (!isToggle) {
			pushed = false;
		}
	} else if (!isToggle) {
		pushed = false;
	}
	return pushed;
}

Button::~Button() {

}

bool Button::checkNoise() {
	long current = millis();
	if (current - lastTime > 500) {
		lastTime = current;
		return true;
	}
	return false;
}

