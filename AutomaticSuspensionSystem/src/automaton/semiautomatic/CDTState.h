/*
 * CDTState.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef CDTSTATE_H_
#define CDTSTATE_H_

class CDTState : public CommonState {
public:
	CDTState() : CommonState() {};
	~CDTState() {};

	bool transitable(Application* app) {
		return app->modeButton.isPushed(2000);
	}

	State* transit(Application* app) {
		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		if (app->automaton->descentState->transitable(app)) {
			return app->automaton->descentState;
		}

		if (app->automaton->trailState->transitable(app)) {
			return app->automaton->trailState;
		}

		if (app->automaton->climbState->transitable(app)) {
			return app->automaton->climbState;
		}

		return this;
	}

	String getName() {
		return "Semiautomatic";
	}

};

#endif /* CDTSTATE_H_ */
