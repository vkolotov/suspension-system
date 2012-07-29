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
	Button() : DebounceActivity(500), pin(0), pushed(false), isToggle(false) {};
	Button(unsigned char pin, bool isToggle) : DebounceActivity(500),
			pin(pin), pushed(false), isToggle(isToggle) {
		pinMode(pin, INPUT);
	}

	void debounce(bool event) {
		if (event) {
			isToggle ? pushed = !pushed : pushed = true;
		} else if (!isToggle) {
			pushed = false;
		}
	};

	bool event() {
		return digitalRead(pin) == HIGH;
	}

	inline bool isPushed() {
		return pushed;
	}


protected:
	unsigned char pin;
	bool pushed;
	bool isToggle;
};

#endif /* BUTTON_H_ */
