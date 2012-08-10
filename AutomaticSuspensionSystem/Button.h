/*
 * Button.h
 *
 *  Created on: 26.07.2012
 *      Author: Vlad
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <DebounceActivity.h>

class Button : public DebounceActivity {
public:

	Button(unsigned char pin, bool isToggle) : DebounceActivity(BUTTON_DEBOUNCE_DURATION),
			pin(pin), pushDuration(0), isToggle(isToggle) {
		pinMode(pin, INPUT);
	}

	void idle(unsigned long currentTime) {
		pushDuration = isToggle ? (pushDuration > 0 ? 1 : -1) : 0;
	}

	void start(unsigned long currentTime) {}

	void stop(unsigned long duration) {
		pushDuration = isToggle ? (pushDuration > 0 ? -duration : duration) : duration;
	}

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == HIGH;
	}

	inline bool isPushed(unsigned short duration) {
		return labs(pushDuration) >= duration;
	}

	inline bool isPushed() {
		return pushDuration > 0;
	}

protected:
	unsigned char pin;
	long pushDuration;
	bool isToggle;
};

#endif /* BUTTON_H_ */
