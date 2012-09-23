/*
 * CalibrationState.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef CALIBRATIONSTATE_H_
#define CALIBRATIONSTATE_H_


class CalibrationState : public State {
public:
	CalibrationState() {};
	~CalibrationState() {};

	bool transitable(Application* app) {
		return app->frontButton.isPushed(5000);
	}

	State* transit(Application* app) {
		app->unsprungAccelerometerSystem.calibrate();
		//app->sprungAccelerometerSystem.calibrate();
		app->frontSuspension.calibrate();
		app->frontSuspension.lock();
		delay(500);
		app->frontSuspension.detach();
		app->rearSuspension.calibrate();
		app->rearSuspension.lock();
		return app->automaton->previous;
	}

	String getName() {
		return "Calibration";
	}
};

#endif /* CALIBRATIONSTATE_H_ */
