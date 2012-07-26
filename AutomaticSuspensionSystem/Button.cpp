/*
 * Button.cpp
 *
 *  Created on: 26.07.2012
 *      Author: Vlad
 */

#include "Button.h"

Button::Button(): pin(0), lastTime(0) {

}

Button::Button(int pin): pin(pin), lastTime(0) {
	pinMode(pin, INPUT);
}

bool Button::isPushed() {
	if (digitalRead(pin) == HIGH) {
		long current = millis();
		if (current - lastTime > 500) {
			lastTime = current;
			return true;
		}
	}
	return false;
}

Button::~Button() {

}

