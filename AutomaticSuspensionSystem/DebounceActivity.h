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
	DebounceActivity(unsigned short threshold) : threshold(threshold), last(0), processEvent(false) {};

	void update() {
		initActivity();
		long current = millis();
		long duration = current - last;
		if (duration > threshold) {
			if (qualifier()) {
				if (!processEvent) {
					last = current;
					processEvent = true;
					startActivity();
				}
			} else {
				if (processEvent) {
					processEvent = false;
					stopActivity(duration);
				}
			}
		}
	}
	virtual bool qualifier() = 0;
	virtual void initActivity();
	virtual void startActivity();
	virtual void stopActivity(unsigned long duration);
protected:
	unsigned short threshold;
	unsigned long last;
	bool processEvent;
};


#endif /* DEBOUNCEACTIVITY_H_ */
