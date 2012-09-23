/*
 * CommonState.h
 *
 *  Created on: 11.09.2012
 *      Author: Vlad
 */

#ifndef COMMONSTATE_H_
#define COMMONSTATE_H_

void (*resetArduino)(void) = 0; //declare reset function @ address 0

class CommonState : public State {
public:
	CommonState() {};
	~CommonState() {};

	bool transitable(Application* app) {
		return true;
	}

	State* transit(Application* app) {
		if (app->modeButton.isPushed(5000)) {
			resetArduino();
		}

		if (app->automaton->calibrationState->transitable(app)) {
			return app->automaton->calibrationState;
		}

		if (app->rearButton.isPushed(5000)) {
			app->setSerialPower(!app->config->powerSave.isSerialPowerOn);
		}

		if (app->automaton->manualState->transitable(app)) {
			app->config->system.mode = MODE_MANUAL;
			return app->automaton->manualState;
		}

		if (app->automaton->cdtState->transitable(app)) {
			app->config->system.mode = MODE_SEMIAUTOMATIC;
			return app->automaton->cdtState;
		}

		if (app->automaton->automaticState->transitable(app)) {
			app->config->system.mode = MODE_AUTOMATIC;
			return app->automaton->automaticState;
		}





		if (app->config->system.mode == MODE_AUTOMATIC) {
			if (app->frontButton.isPushed()) {
				app->unsprungAccelerometerSystem.increaseThreshold(25);
			} else if (app->rearButton.isPushed()) {
				app->unsprungAccelerometerSystem.increaseThreshold(-25);
			}
		} else if (app->config->system.mode == MODE_SEMIAUTOMATIC) {
			if (app->frontButton.isPushed()) {
				app->frontSuspension.increaseMediumAngle(5);
			} else if (app->rearButton.isPushed()) {
				app->frontSuspension.increaseMediumAngle(-5);
			}
		}

		if (app->automaton->sleepState->transitable(app)) {
			return app->automaton->sleepState;
		}
		return 0;
	}

	String getName() {
		return "Common";
	}

};


#endif /* COMMONSTATE_H_ */
