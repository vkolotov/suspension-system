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
				app->config->semiautomaticStateConfig.climbAngle
						+ app->config->semiautomaticStateConfig.transitionGap;
	}

	State* transit(Application* app) {

		app->frontSuspension.lock();
		app->rearSuspension.lock();

		State* result = CDTState::transit(app);
		if (result) {
			return result;
		}

		return this;
	}

	String getName() {
		return "Climb";
	}
};

#endif /* CLIMBSTATE_H_ */
