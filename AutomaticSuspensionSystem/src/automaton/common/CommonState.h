/*
 * CommonState.h
 *
 *  Created on: 11.09.2012
 *      Author: Vlad
 */

#ifndef COMMONSTATE_H_
#define COMMONSTATE_H_

class CommonState : public State {
public:
	CommonState() {};
	~CommonState() {};

	bool transitable(Application* app) {
		return true;
	}

	State* transit(Application* app) {
		if (app->modeButton.isPushed(5000, false)) {
			//resetArduino();
		}

		if (app->frontButton.isPushed(5000, false)) {
			app->setSerialPower(!app->config->powerSave.isSerialPowerOn);
		}

		if (app->automaton->manualState->transitable(app)) {
			return app->automaton->manualState;
		}

		if (app->automaton->cdtState->transitable(app)) {
			return app->automaton->cdtState;
		}

		if (app->automaton->automaticState->transitable(app)) {
			return app->automaton->automaticState;
		}

		if (app->automaton->sleepState->transitable(app)) {
			return app->automaton->sleepState;
		}
		return 0;
	}

	uint8_t getId() {
		return COMMON_STATE;
	}

};


#endif /* COMMONSTATE_H_ */
