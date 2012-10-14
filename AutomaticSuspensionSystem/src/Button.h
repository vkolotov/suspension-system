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

	Button(unsigned char pin, bool isToggle, uint16_t debonceDuration, unsigned short pinReference = HIGH) :
			DebounceActivity(debonceDuration),	pin(pin), pushDuration(0),
			isToggle(isToggle), pinReference(pinReference) {
		pinMode(pin, pinReference == HIGH ? INPUT : INPUT_PULLUP);
		if (pinReference == LOW) {
			digitalWrite(pin, HIGH);
		}
	}

	void reset(unsigned long currentTime) {
		pushDuration = isToggle ? (pushDuration > 0 ? 1 : -1) : 0;
	}

	void start(unsigned long currentTime) {}

	void stop(unsigned long duration) {
		pushDuration = isToggle ? (pushDuration > 0 ? -duration : duration) : duration;
	}

	bool qualifier(unsigned long currentTime) {
		return digitalRead(pin) == pinReference;
	}

	inline bool isPushed(unsigned short duration, bool flush) {
		bool result = labs(pushDuration) >= duration;
		if (result && flush) {
			reset(millis());
		}
		return result;
	}

	inline bool isPushed(unsigned short duration) {
		return isPushed(duration, true);
	}

	inline bool isPushed() {
		return pushDuration > 0;
	}

protected:
	unsigned char pin;
	long pushDuration;
	bool isToggle;
	unsigned short pinReference;
};

#endif /* BUTTON_H_ */
