/*
 * BurnOutState.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef BURNOUTSTATE_H_
#define BURNOUTSTATE_H_

class BurnOutState : public ActiveState {
public:
	BurnOutState() : ActiveState() {};
	~BurnOutState() {};

	bool transitable(Application* app) {
		return app->cadenceSystem.isProcessing()
				&& !app->unsprungAccelerometerSystem.isActive()
				&& app->sprungAccelerometerSystem.isActive();
	}

	State* transit(Application* app) {
		app->frontSuspension.lock();
		app->rearSuspension.lock();
		return ActiveState::transit(app);
	}
};

#endif /* BURNOUTSTATE_H_ */
