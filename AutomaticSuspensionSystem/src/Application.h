/*
 * Application.h
 *
 *  Created on: 05.09.2012
 *      Author: Vlad
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

class BluetoothSystem;

class Application {
public:
	Application(Configuration* config, Automaton* automaton) : config(config), automaton(automaton),
			frontSuspension(config, &config->frontSuspension),
			rearSuspension(config, &config->rearSuspension),
			cadenceSystem(&config->cadence),
			speedSystem(&config->speed),
			unsprungAccelerometerSystem(config, &config->unsprungAccelerometerSystem, &speedSystem),
			sprungAccelerometerSystem(config, &config->sprungAccelerometerSystem),
			frontButton(config->buttons.frontPin, false, config->buttons.debounceDuration, config->buttons.frontPinReferenceValue),
			rearButton(config->buttons.rearPin, false, config->buttons.debounceDuration, config->buttons.rearPinReferenceValue),
			modeButton(config->buttons.modePin, false, config->buttons.debounceDuration, config->buttons.modePinReferenceValue),
			bluetoothSystem(),
			time(0), lastTime(0), startTime(0) {

	}
	virtual ~Application() {};

	void setup() {
		startTime = millis();

		pinMode(config->powerSave.servoRelayPin, OUTPUT);
		pinMode(config->powerSave.serialRelayPin, OUTPUT);
		pinMode(config->powerSave.i2cRelayPin, OUTPUT);

		// activating pullups
		digitalWrite(config->buttons.frontPin, HIGH);
		digitalWrite(config->buttons.modePin, HIGH);
		digitalWrite(config->buttons.rearPin, HIGH);
		digitalWrite(config->cadence.frequencySystemConfig.pin, HIGH);
		digitalWrite(config->speed.frequencySystemConfig.pin, HIGH);

		digitalWrite(config->powerSave.servoRelayPin, config->powerSave.isServoPowerOn);
		digitalWrite(config->powerSave.i2cRelayPin, config->powerSave.isi2cPowerOn);
		setSerialPower(config->powerSave.isSerialPowerOn);

		unsprungAccelerometerSystem.init();
		sprungAccelerometerSystem.init();
	}

	void update() {
		time = millis();
		frontSuspension.update(time);
		rearSuspension.update(time);
		cadenceSystem.update(time);
		speedSystem.update(time);
		unsprungAccelerometerSystem.update(time);
		sprungAccelerometerSystem.update(time);
		frontButton.update(time);
		modeButton.update(time);
		rearButton.update(time);

		automaton->update(this);

		if (config->powerSave.isSerialPowerOn) {
			bluetoothSystem->update(time);
		}
	}

	void sleep() {
		digitalWrite(config->powerSave.servoRelayPin, LOW);
		digitalWrite(config->powerSave.i2cRelayPin, LOW);
		setSerialPower(false);
	}

	void wakeup() {
		digitalWrite(config->powerSave.servoRelayPin, config->powerSave.isServoPowerOn);
		digitalWrite(config->powerSave.i2cRelayPin, config->powerSave.isi2cPowerOn);
		setSerialPower(config->powerSave.isSerialPowerOn);
	}

	void setSerialPower(bool on) {
		digitalWrite(config->powerSave.serialRelayPin, on ? HIGH : LOW);
		if (on) {
			bluetoothSystem->init();
		}
	}

	bool getSerialPower() {
		return digitalRead(config->powerSave.serialRelayPin) == HIGH;
	}

	void descentSuspensions() {
		frontSuspension.release();
		rearSuspension.release();
	}

	void trailSuspensions() {
		frontSuspension.medium();
		rearSuspension.medium();
	}

	void climbSuspensions() {
		frontSuspension.lock();
		rearSuspension.lock();
	}

	Configuration* config;
	Automaton* automaton;

	Suspension frontSuspension;
	Suspension rearSuspension;
	CadenceSystem cadenceSystem;
	SpeedSystem speedSystem;

	UnsprungAccelerometerSystem unsprungAccelerometerSystem;
	SprungAccelerometerSystem sprungAccelerometerSystem;

	Button frontButton;
	Button rearButton;
	Button modeButton;

	Activity* bluetoothSystem;

	BluetoothSystem* getBluetoothSystem() {
		return (BluetoothSystem*) bluetoothSystem;
	}

	unsigned long time;
	unsigned long lastTime;
	unsigned long startTime;

};



#endif /* APPLICATION_H_ */
