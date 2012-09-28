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
	ManualState() : calibration(false), calibrationSuspension(), currentSuspentionMode(0), mode(true) {};
	~ManualState() {};

	bool transitable(Application* app) {
		return app->frontButton.isPushed(1000);
	}

	State* transit(Application* app) {

		if (calibration) {
			calibratetionMode(app);
		} else {
			manualMode(app);
		}

		State* result = CommonState::transit(app);
		if (result) {
			return result;
		}

		return this;
	}
	String getName() {
		return "Manual";
	}

protected:

	void calibratetionMode(Application* app) {
		if (app->frontButton.isPushed()) {
			calibrationSuspension->bind();
			calibrationSuspension->write(calibrationSuspension->read()
					+ calibrationSuspension->getConfig()->calibrationStep);
		} else if (app->rearButton.isPushed()) {
			calibrationSuspension->bind();
			calibrationSuspension->write(calibrationSuspension->read()
					- calibrationSuspension->getConfig()->calibrationStep);
		} if (app->modeButton.isPushed()) {
			SuspensionSystemConfig* config = calibrationSuspension->getConfig();
			config->angles[currentSuspentionMode] = calibrationSuspension->read();
			currentSuspentionMode++;
			if (currentSuspentionMode >= config->modes) {
				calibration = false;
				saveConfiguration(app->config);
				return;
			}
			calibrationSuspension->setMode(currentSuspentionMode);
		}
	}

	void manualMode(Application* app) {
		if (app->frontButton.isPushed(3000)) {
			Serial.println("Front calibrating");
			beginCalibration(&app->frontSuspension);
			return;
		}

		if (app->rearButton.isPushed(3000)) {
			Serial.println("Rear calibrating");
			beginCalibration(&app->rearSuspension);
			return;
		}

		if (app->modeButton.isPushed(3000)) {
			Serial.println("Set mode");
			mode = !mode;
			return;
		}

		if (mode) {
			if (app->frontButton.isPushed()) {
				app->frontSuspension.lock();
				app->rearSuspension.lock();
			}
			if (app->modeButton.isPushed()) {
				app->frontSuspension.medium();
				app->rearSuspension.medium();
			}
			if (app->rearButton.isPushed()) {
				app->frontSuspension.release();
				app->rearSuspension.release();
			}
		} else {
			if (app->frontButton.isPushed()) {
				Serial.println("front toggle");
				app->frontSuspension.toggle();
			}
			if (app->rearButton.isPushed()) {
				Serial.println("rear toggle");
				app->rearSuspension.toggle();
			}
		}
	}

	void beginCalibration(Suspension* suspension) {
		calibrationSuspension = suspension;
		currentSuspentionMode = 0;
		calibrationSuspension->setMode(0);
		calibration = true;
	}

protected:

	bool calibration;
	Suspension* calibrationSuspension;
	uint8_t currentSuspentionMode;

	bool mode;

};

#endif /* MANUALSTATE_H_ */
