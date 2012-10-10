/*
 * AbsorbState.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef ABSORBSTATE_H_
#define ABSORBSTATE_H_

class AbsorbState : public ActiveState {
public:
	AbsorbState() : ActiveState() {};
	~AbsorbState() {};

	bool transitable(Application* app) {
		return app->cadenceSystem.isProcessing()
				&& app->unsprungAccelerometerSystem.isActive();
	}

	State* transit(Application* app) {
		app->descentSuspensions();
		return ActiveState::transit(app);
	}

	uint8_t getId() {
		return ABSORB_STATE;
	}
};

#endif /* ABSORBSTATE_H_ */
