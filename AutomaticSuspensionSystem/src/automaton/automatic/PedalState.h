/*
 * PedalState.h
 *
 *  Created on: 25.09.2012
 *      Author: Vlad
 */

#ifndef PEDALSTATE_H_
#define PEDALSTATE_H_

class PedalState : public ActiveState {
public:
	PedalState() : ActiveState() {};
	~PedalState() {};

	bool transitable(Application* app) {
		return app->cadenceSystem.isProcessing()
				&& !app->unsprungAccelerometerSystem.isActive()
				&& !app->sprungAccelerometerSystem.isActive();
	}

	State* transit(Application* app) {
		app->frontSuspension.release();
		app->rearSuspension.lock();
		return ActiveState::transit(app);
	}

	uint8_t getId() {
		return PEDAL_STATE;
	}
};
#endif /* PEDALSTATE_H_ */
