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
		if (app->config->powerSave.isSleepEnabled
				&& app->time - app->startTime < app->config->powerSave.sleepTimeout) {
			return false;
		}
		unsigned long timeout = app->time - app->config->powerSave.sleepTimeout;

		if (app->frontButton.getLastActivity() < timeout
				&& app->modeButton.getLastActivity() < timeout
				&& app->rearButton.getLastActivity() < timeout
				&& app->speedSystem.getLastActivity() < timeout
				&& app->cadenceSystem.getLastActivity() < timeout
				&& app->getBluetoothSystem()->getLastActivity() < timeout) {
			return true;
		}

		return false;
	}

	State* transit(Application* app) {
		app->sleep();

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		attachInterrupt(0, wakeup, LOW);
		attachInterrupt(1, wakeup, LOW);

		sleep_mode();

		// Continuing
		sleep_disable();
		detachInterrupt(0);
		detachInterrupt(1);

		app->wakeup();
		app->cadenceSystem.setLastActivity(millis());

		return app->automaton->previous;
	}

	uint8_t getId() {
		return SLEEP_STATE;
	}


};

#endif /* SLEEPSTATE_H_ */
