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

static const unsigned short DEBOUNCE_DURATION = 200;

class Button : public DebounceActivity {
public:
	Button() : DebounceActivity(DEBOUNCE_DURATION), pin(0), pushDuration(0), isToggle(false) {}
	Button(unsigned char pin, bool isToggle) : DebounceActivity(DEBOUNCE_DURATION),
			pin(pin), pushDuration(0), isToggle(isToggle) {
		pinMode(pin, INPUT);
	}

	void initActivity() {
		pushDuration = isToggle ? (pushDuration > 0 ? 1 : -1) : 0;
	}

	void startActivity() {}

	void stopActivity(unsigned long duration) {
		pushDuration = isToggle ? (pushDuration > 0 ? -duration : duration) : duration;
	}

	bool qualifier() {
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
