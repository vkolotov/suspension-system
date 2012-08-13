/*
 * DebounceActivity.h
 *
 *  Created on: 29.07.2012
 *      Author: Vlad
 */

#ifndef DEBOUNCEACTIVITY_H_
#define DEBOUNCEACTIVITY_H_

class DebounceActivity {
public:
	DebounceActivity(unsigned short threshold) : threshold(threshold), lastEvent(0), processEvent(false) {};

	void update(unsigned long current) {
		reset(current);
		if (current - lastEvent < threshold) {
			return;
		}
		if (qualifier(current)) {
			if (!processEvent) {
				processEvent = true;
				start(current);
				lastEvent = current;
				return;
			}
		} else {
			if (processEvent) {
				processEvent = false;
				stop(current - lastEvent);
				return;
			}
		}
	}
	virtual bool qualifier(unsigned long currentTime) = 0;
	virtual void reset(unsigned long currentTime);
	virtual void start(unsigned long currentTime);
	virtual void stop(unsigned long duration);
protected:
	unsigned short threshold;
	unsigned long lastEvent;
	bool processEvent;
};


#endif /* DEBOUNCEACTIVITY_H_ */
