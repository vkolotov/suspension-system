/*
 * AutomaticState.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSTATE_H_
#define AUTOMATICSTATE_H_

class AutomaticState : public CommonState {
public:
	AutomaticState() {};
	~AutomaticState() {};

	bool transitable(Application* app) {
		return app->rearButton.isPushed(2000);
	}

	State* transit(Application* app) {
		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		if (app->speedSystem.isProcessing() && app->cadenceSystem.isProcessing()) {

			if (app->unsprungAccelerometerSystem.isActive()) {
				app->frontSuspension.release();
				app->rearSuspension.release();
			} else {
				app->frontSuspension.lock();
				app->rearSuspension.lock();
			}
		} else {
			app->rearSuspension.release();
			app->frontSuspension.release();
		}

		return this;
	}
	String getName() {
		return "Automatic";
	}


};

#endif /* AUTOMATICSTATE_H_ */
