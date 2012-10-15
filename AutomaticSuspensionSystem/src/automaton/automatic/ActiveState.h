/*
 * ActiveState.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef ACTIVESTATE_H_
#define ACTIVESTATE_H_

class ActiveState : public AutomaticState {
public:
	ActiveState() : AutomaticState() {};
	~ActiveState() {};

	State* transit(Application* app) {
		handleButtons(app);

		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		if (app->automaton->idleState->transitable(app)) {
			return app->automaton->idleState;
		} else {
			return ActiveState::transitToActive(app);
		}
	}


	static bool transitableToActive(Application* app) {
		return app->speedSystem.getAverageSpeedKmH() >= AutomaticState::getIdleSpeed(
						app->sprungAccelerometerSystem.getAverageDegreeGradient()) + 1;
	}

	static State* transitToActive(Application* app) {

		if (app->automaton->standOverState->transitable(app)) {
			return app->automaton->standOverState;
		} else if (app->automaton->pedalState->transitable(app)) {
			return app->automaton->pedalState;
		} else if (app->automaton->burnOutState->transitable(app)) {
			return app->automaton->burnOutState;
		} else if (app->automaton->absorbState->transitable(app)) {
			return app->automaton->absorbState;
		}
		return app->automaton->standOverState;
	}

	virtual uint8_t getId() {
		return ACTIVE_STATE;
	}

};

#endif /* ACTIVESTATE_H_ */
