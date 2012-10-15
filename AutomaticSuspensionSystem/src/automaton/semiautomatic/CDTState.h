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
	CDTState() : CommonState(), currentState(0) {};
	~CDTState() {};

	bool transitable(Application* app) {
		return app->modeButton.isPushed(1000);
	}

	State* transit(Application* app) {

		if (app->frontButton.isPushed(3000)) {
			app->config->semiautomaticStateConfig.climbGradient =
					app->sprungAccelerometerSystem.getAverageGradient();
			saveConfiguration(app->config);
		} else if (app->modeButton.isPushed(3000)) {
			app->config->system.headTubeGradient =
					app->sprungAccelerometerSystem.getRawGradient();
			app->sprungAccelerometerSystem.calibrate();
			saveConfiguration(app->config);
		} else if (app->rearButton.isPushed(3000)) {
			app->config->semiautomaticStateConfig.descendGradient =
					app->sprungAccelerometerSystem.getAverageGradient();
			saveConfiguration(app->config);
		}

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

	virtual uint8_t getId() {
		return CDT_STATE;
	}

protected:
	uint8_t currentState;

};

#endif /* CDTSTATE_H_ */
