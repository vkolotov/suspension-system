/*
 * BluetoothSystem.h
 *
 *  Created on: 05.09.2012
 *      Author: Vlad
 */

#ifndef BLUETOOTHSYSTEM_H_
#define BLUETOOTHSYSTEM_H_


class BluetoothSystem : public Activity {
public:
	BluetoothSystem(Application* app) : Activity(), app(app), lastActivity(0) {};
	~BluetoothSystem() {};

	void init() {
		lastActivity = millis();
	}

	void update(unsigned long time) {

		if (app->config->powerSave.isBluetoothSleepEnabled
				&& time > lastActivity
				&& time - lastActivity >= app->config->powerSave.bluetoothSleepTimeout) {
			app->setSerialPower(false);
		}

		if (Serial.available()) {
			if (Serial.available() == 1) {
				lastActivity = time;
				uint8_t requestId = Serial.read();

				switch (requestId) {
					case 0: Serial.write((uint8_t)0); return;
					case 1: resetArduino(); break;
					case 2: app->automaton->setState(SLEEP_STATE); break;

					case 10: saveConfiguration(app->config); break;
					case 11: reloadConfiguration(app->config); break;
					case 12: resetConfiguration(app->config); break;
					case 13: Serial.write((uint8_t*)app->config, sizeof(Configuration)); Serial.flush(); break;
					case 14: receiveConfiguration(); break;
					case 15: receiveSuspensionConfig(&app->config->frontSuspension); break;
					case 16: receiveSuspensionConfig(&app->config->rearSuspension); break;
					case 17: receiveCdtBoardMessage(); break;
					case 18: receiveAutomaticBoardMessage(); break;

					case 50: app->descentSuspensions(); break;
					case 51: app->trailSuspensions(); break;
					case 52: app->climbSuspensions(); break;

					case 53: app->automaton->setState(MANUAL_STATE); break;
					case 54: app->automaton->setState(CDT_STATE); break;
					case 55: app->automaton->setState(AUTOMATIC_STATE); break;
					case 56: app->config->system.headTubeGradient = app->sprungAccelerometerSystem.getRawGradient();
							app->sprungAccelerometerSystem.calibrate();
							break;

					case 60: sendManualTelemetry(); break;
					case 61: sendCDTTelemetry(); break;
					case 62: sendAutomaticTelemetry(); break;

				}

			} else {
				// something strange there, let's skip it
				while (Serial.available()) {
					Serial.read();
				}
			}

		}

	}

	unsigned long getLastActivity() {
		return lastActivity;
	}


private:

	void receiveConfiguration() {
		Configuration* tmp = new Configuration();
		if (receiveMessage((char*)tmp, sizeof(Configuration))) {
			memcpy((void*)app->config, (void*)tmp, sizeof(Configuration));
		}
		delete tmp;
	}

	void receiveSuspensionConfig(SuspensionSystemConfig* config) {
		SuspensionSystemConfig* tmp = new SuspensionSystemConfig();
		if (receiveMessage((char*)tmp, sizeof(SuspensionSystemConfig))) {
			memcpy((void*)config, (void*)tmp, sizeof(SuspensionSystemConfig));
		}
		delete tmp;
	}

	void sendManualTelemetry() {
		ManualTelemetry msg = {app->clockSpeed, app->automaton->current->getId(),
				app->speedSystem.getAverageSpeedKmH(),
				app->cadenceSystem.getCadence(),
				app->config->frontSuspension.mode
		};
		Serial.write((uint8_t*)&msg, sizeof(msg));
		Serial.flush();
	}

	void sendCDTTelemetry() {
		CDTTelemetry msg = {app->clockSpeed, app->automaton->current->getId(),
				app->speedSystem.getAverageSpeedKmH(),
				app->cadenceSystem.getCadence(),
				app->config->frontSuspension.mode,
				app->sprungAccelerometerSystem.getAverageDegreeGradient(),
				app->config->semiautomaticStateConfig.climbGradient * 180.0f / PI,
				app->config->semiautomaticStateConfig.descendGradient * 180.0f / PI
		};

		BasicQueue<20, int16_t>* gradients = app->sprungAccelerometerSystem.getFilteredGradients();
		gradients->copyTo(msg.filteredGradients);
		gradients->clear();

		gradients = app->sprungAccelerometerSystem.getRawGradients();
		gradients->copyTo(msg.rawGradients);

		msg.dataLength = gradients->size();
		gradients->clear();

		Serial.write((uint8_t*)&msg, sizeof(msg));
		Serial.flush();
	}

	void sendAutomaticTelemetry() {
		AutomaticTelemetry msg = {app->clockSpeed, app->automaton->current->getId(),
				app->speedSystem.getAverageSpeedKmH(),
				app->cadenceSystem.getCadence(),
				app->config->sprungAccelerometerSystem.accelerometerSystemConfig.severityThreshold,
				app->config->unsprungAccelerometerSystem.accelerometerSystemConfig.severityThreshold,
				app->unsprungAccelerometerSystem.getCalculatedTimeout()
		};

		BasicQueue<20, int16_t>* readings = app->sprungAccelerometerSystem.getReadingsX();
		readings->copyTo(msg.sprungReadingsX);
		readings->clear();

		readings = app->unsprungAccelerometerSystem.getReadingsX();
		readings->copyTo(msg.unsprungReadingsX);

		msg.dataLength = readings->size();
		readings->clear();

		Serial.write((uint8_t*)&msg, sizeof(msg));
		Serial.flush();
	}

	void receiveCdtBoardMessage() {
		CDTBoardMessage* tmp = new CDTBoardMessage();
		if (receiveMessage((char*)tmp, sizeof(CDTBoardMessage))) {
			app->config->semiautomaticStateConfig.climbGradient = (float) tmp->climbGradient * PI / 180.0f;
			app->config->semiautomaticStateConfig.descendGradient = (float) tmp->descendGradient * PI / 180.0f;
		}
		delete tmp;
	}

	void receiveAutomaticBoardMessage() {
		AutomaticBoardMessage* tmp = new AutomaticBoardMessage();
		if (receiveMessage((char*)tmp, sizeof(AutomaticBoardMessage))) {
			app->config->unsprungAccelerometerSystem.accelerometerSystemConfig.severityThreshold = tmp->unsprungSeverityThreshold;
			app->config->sprungAccelerometerSystem.accelerometerSystemConfig.severityThreshold = tmp->sprungSeverityThreshold;
		}
		delete tmp;
	}

	bool receiveMessage(char* msg, size_t size) {
		// sending status that we are ready
		Serial.write((uint8_t)0);
		Serial.flush();
		Serial.setTimeout(10000);

		size_t result = Serial.readBytes((char*)msg, size);

		// sending how many we received
		Serial.write((uint8_t*) &result, sizeof(size_t));
		Serial.flush();

		return result == size;
	}

private:

	Application* app;
	unsigned long lastActivity;

};

#endif /* BLUETOOTHSYSTEM_H_ */
