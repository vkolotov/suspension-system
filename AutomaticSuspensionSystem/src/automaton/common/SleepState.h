/*
 * SleepState.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef SLEEPSTATE_H_
#define SLEEPSTATE_H_

void wakeup() {}

class SleepState : public State {
public:
	SleepState() {};
	~SleepState() {};

	bool transitable(Application* app) {
		if (app->time - app->startTime < SLEEP_TIMEOUT) {
			return false;
		}
		unsigned long timeout = app->time - SLEEP_TIMEOUT;

		if (app->frontButton.getLastActivity() < timeout
				&& app->modeButton.getLastActivity() < timeout
				&& app->rearButton.getLastActivity() < timeout
				&& app->speedSystem.getLastActivity() < timeout
				&& app->cadenceSystem.getLastActivity() < timeout) {
			return true;
		}

		return false;
	}

	State* transit(Application* app) {

		app->sleep();

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		attachInterrupt(SLEEP_INTERRUPTION_NUMBER, wakeup, LOW);

		sleep_mode();
		// Continuing
		sleep_disable();
		detachInterrupt(SLEEP_INTERRUPTION_NUMBER);

		app->wakeup();
		app->cadenceSystem.setLastActivity(millis());

		return app->automaton->previous;
	}
	String getName() {
		return "Sleep";
	}


};

#endif /* SLEEPSTATE_H_ */
