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
			frontSuspension(*config, config->frontSuspension),
			rearSuspension(*config, config->rearSuspension),
			cadenceSystem(config->cadence),
			speedSystem(config->speed),
			unsprungAccelerometerSystem(*config, config->unsprungAccelerometerSystem),
			sprungAccelerometerSystem(*config, config->sprungAccelerometerSystem),
			frontButton(config->buttons.frontPin, false),
			rearButton(config->buttons.rearPin, false),
			modeButton(config->buttons.modePin, false),
			bluetoothSystem(),
			time(0), lastTime(0), startTime(0) {

	}
	virtual ~Application() {};

	void setup() {
		startTime = millis();

		// pullup resistor
		// TODO check what is INPUT_PULLUP
		digitalWrite(config->cadence.frequencySystemConfig.pin, HIGH);

//		pinMode(FRONT_BUTTON_PIN, INPUT);
//		digitalWrite(FRONT_BUTTON_PIN, LOW);
//		digitalWrite(MODE_BUTTON_PIN, LOW);
//		digitalWrite(REAR_BUTTON_PIN, LOW);
//

		pinMode(config->powerSave.servoRelayPin, OUTPUT);
		pinMode(config->powerSave.serialRelayPin, OUTPUT);
		pinMode(config->powerSave.i2cRelayPin, OUTPUT);

		digitalWrite(config->cadence.frequencySystemConfig.pin, HIGH);

		digitalWrite(config->powerSave.servoRelayPin, HIGH);
		digitalWrite(config->powerSave.i2cRelayPin, HIGH);

		Serial.print("Setup bluetooth ");
		Serial.println(config->powerSave.isSerialPowerOn);

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
//		digitalWrite(4, LOW);
//		digitalWrite(5, LOW);
//		digitalWrite(6, LOW);
	}

	void wakeup() {

//		digitalWrite(4, HIGH);
//		digitalWrite(5, HIGH);
//		digitalWrite(6, HIGH);
	}

	void setSerialPower(bool on) {
		digitalWrite(config->powerSave.serialRelayPin, on ? HIGH : LOW);
		config->powerSave.isSerialPowerOn = on;
		if (on) {
			bluetoothSystem->init();
		}
	}

	bool getSerialPower() {
		return digitalRead(config->powerSave.serialRelayPin) == HIGH;
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
