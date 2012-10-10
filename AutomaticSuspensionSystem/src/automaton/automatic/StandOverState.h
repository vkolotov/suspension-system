/*
 * StandOverState.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef STANDOVERSTATE_H_
#define STANDOVERSTATE_H_

class StandOverState : public ActiveState {
public:
	StandOverState() : ActiveState() {};
	~StandOverState() {};

	bool transitable(Application* app) {
		return !app->cadenceSystem.isProcessing();
	}

	State* transit(Application* app) {
		app->frontSuspension.release();
		app->rearSuspension.release();
		return ActiveState::transit(app);
	}

	uint8_t getId() {
		return STANDOVER_STATE;
	}
};

#endif /* STANDOVERSTATE_H_ */
