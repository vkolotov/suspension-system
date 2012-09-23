/*
 * ManualState.h
 *
 *  Created on: 11.09.2012
 *      Author: Vlad
 */

#ifndef MANUALSTATE_H_
#define MANUALSTATE_H_

class ManualState : public CommonState {
public:
	ManualState() {};
	~ManualState() {};

	bool transitable(Application* app) {
		return app->frontButton.isPushed(2000);
	}

	State* transit(Application* app) {
		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		if (app->frontButton.isPushed()) {
			Serial.println("front toggle");
			app->frontSuspension.toggle();
		}
		if (app->rearButton.isPushed()) {
			Serial.println("rear toggle");
			app->rearSuspension.toggle();
		}
		return this;
	}
	String getName() {
		return "Manual";
	}

};

#endif /* MANUALSTATE_H_ */
