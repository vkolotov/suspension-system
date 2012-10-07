/*
 * TrailState.h
 *
 *  Created on: 13.09.2012
 *      Author: Vlad
 */

#ifndef TRAILSTATE_H_
#define TRAILSTATE_H_

class TrailState : public CDTState {
public:
	TrailState() : CDTState() {};
	~TrailState() {};

	bool transitable(Application* app) {
		return app->sprungAccelerometerSystem.getAverageGradient() >=
				app->config->semiautomaticStateConfig.descentGradient
						+ app->config->semiautomaticStateConfig.transitionGap
						&&
				app->sprungAccelerometerSystem.getAverageGradient() <=
						app->config->semiautomaticStateConfig.climbGradient
								- app->config->semiautomaticStateConfig.transitionGap;
	}

	State* transit(Application* app) {

		app->trailSuspensions();

		State* result = CDTState::transit(app);
		if (result) {
			return result;
		}

		return this;
	}
	String getName() {
		return "Trail";
	}
};

#endif /* TRAILSTATE_H_ */
