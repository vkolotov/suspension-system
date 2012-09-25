/*
 * AutomaticState.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSTATE_H_
#define AUTOMATICSTATE_H_

class AutomaticState : public CommonState {
public:
	AutomaticState() {};
	~AutomaticState() {};

	bool transitable(Application* app) {
		return app->rearButton.isPushed(1000);
	}

	State* transit(Application* app) {
		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		return app->automaton->idleState;
	}
	String getName() {
		return "Automatic";
	}

	static float getIdleSpeed(float gradient) {
		return 5 - map(constrain(gradient, -5, 30), -5, 30, 0, 4);
	}

//	State** activeStates;


};

#endif /* AUTOMATICSTATE_H_ */
