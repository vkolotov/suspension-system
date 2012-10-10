/*
 * DescentState.h
 *
 *  Created on: 13.09.2012
 *      Author: Vlad
 */

#ifndef DESCENTSTATE_H_
#define DESCENTSTATE_H_

class DescentState : public CDTState {
public:
	DescentState() : CDTState() {};
	~DescentState() {};

	bool transitable(Application* app) {
		return app->sprungAccelerometerSystem.getAverageGradient() <=
				app->config->semiautomaticStateConfig.descendGradient
						- app->config->semiautomaticStateConfig.transitionGap;
	}

	State* transit(Application* app) {

		app->descentSuspensions();

		State* result = CDTState::transit(app);
		if (result) {
			return result;
		}

		return this;
	}

	uint8_t getId() {
		return DESCENT_STATE;
	}

};

#endif /* DESCENTSTATE_H_ */
