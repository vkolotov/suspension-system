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

	void update(unsigned long current) {
		initActivity(current);
		if (current - last > threshold) {
			if (qualifier(current)) {
				if (!processEvent) {
					last = current;
					processEvent = true;
					startActivity(current);
				}
			} else {
				if (processEvent) {
					processEvent = false;
					stopActivity(current - last);
				}
			}
		}
	}
	virtual bool qualifier(unsigned long currentTime) = 0;
	virtual void initActivity(unsigned long currentTime);
	virtual void startActivity(unsigned long currentTime);
	virtual void stopActivity(unsigned long duration);
protected:
	unsigned short threshold;
	unsigned long last;
	bool processEvent;
};


#endif /* DEBOUNCEACTIVITY_H_ */
