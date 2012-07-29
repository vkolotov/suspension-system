/*
 * DebounceActivity.h
 *
 *  Created on: 29.07.2012
 *      Author: Vlad
 */

#ifndef DEBOUNCEACTIVITY_H_
#define DEBOUNCEACTIVITY_H_

#include <Activity.h>

class DebounceActivity : public Activity {
public:
	DebounceActivity(unsigned short threshold) : threshold(threshold), last(0) {};

	void update() {
		if (event()) {
			long current = millis();
			if (current - last > threshold) {
				last = current;
				debounce(true);
				return;
			}
		}
		debounce(false);
	}
	virtual bool event() = 0;
	virtual void debounce(bool event) = 0;
protected:
	unsigned short threshold;
	unsigned long last;
};


#endif /* DEBOUNCEACTIVITY_H_ */
