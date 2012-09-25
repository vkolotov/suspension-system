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
		app->frontSuspension.release();
		app->rearSuspension.release();
		return ActiveState::transit(app);
	}
};

#endif /* ABSORBSTATE_H_ */
