/*
 * Idle.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef IDLE_H_
#define IDLE_STATE_H_

class IdleState : public AutomaticState {
public:
	IdleState() : AutomaticState() {};
	~IdleState() {};

	bool transitable(Application* app) {
		return app->speedSystem.getAverageSpeedKmH() <= AutomaticState::getIdleSpeed(
				app->sprungAccelerometerSystem.getDegreeGradient()) - 1;
	}

	State* transit(Application* app) {

		handleButtons(app);

		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		app->frontSuspension.lock();
		app->rearSuspension.lock();

		if (ActiveState::transitableToActive(app)) {
			return ActiveState::transitToActive(app);
		}

		return this;
	}

	uint8_t getId() {
		return IDLE_STATE;
	}

};

#endif /* IDLE_STATE_H_ */
