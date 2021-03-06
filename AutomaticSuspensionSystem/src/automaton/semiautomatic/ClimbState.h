/*
 * ClimbState.h
 *
 *  Created on: 13.09.2012
 *      Author: Vlad
 */

#ifndef CLIMBSTATE_H_
#define CLIMBSTATE_H_

class ClimbState : public CDTState {
public:
	ClimbState() : CDTState() {};
	~ClimbState() {};

	bool transitable(Application* app) {
		return app->sprungAccelerometerSystem.getAverageGradient() >=
				app->config->semiautomaticStateConfig.climbGradient
						+ app->config->semiautomaticStateConfig.transitionGap;
	}

	State* transit(Application* app) {

		app->climbSuspensions();

		State* result = CDTState::transit(app);
		if (result) {
			return result;
		}

		return this;
	}

	uint8_t getId() {
		return CLIMB_STATE;
	}
};

#endif /* CLIMBSTATE_H_ */
