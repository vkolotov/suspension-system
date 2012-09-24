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
		return app->modeButton.isPushed(1000);
	}

	State* transit(Application* app) {

		if (app->frontButton.isPushed(3000)) {
			Serial.print("Climb: ");
			Serial.println(app->sprungAccelerometerSystem.getGradient());
			app->config->semiautomaticStateConfig.climbGradient =
					app->sprungAccelerometerSystem.getGradient();
		}
		if (app->modeButton.isPushed(3000)) {
			Serial.print("Head: ");
			Serial.println(app->sprungAccelerometerSystem.getGradient());
			app->config->system.headTubeGradient =
					app->sprungAccelerometerSystem.getRawGradient();
		}
		if (app->rearButton.isPushed(3000)) {
			Serial.print("Desc: ");
			Serial.println(app->sprungAccelerometerSystem.getGradient());
			app->config->semiautomaticStateConfig.descentGradient =
					app->sprungAccelerometerSystem.getGradient();
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

	String getName() {
		return "Semiautomatic";
	}

};

#endif /* CDTSTATE_H_ */
