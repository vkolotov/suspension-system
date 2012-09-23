#include "AutomaticSuspensionSystem.h"

Configuration* config = loadConfiguration();
Automaton automaton;
Application app(config, &automaton);

void setup() {

	app.bluetoothSystem = new BluetoothSystem(&app);

	automaton.calibrationState = new CalibrationState();
	automaton.sleepState = new SleepState();
	automaton.previous = automaton.current = automaton.manualState = new ManualState();

	automaton.cdtState = new CDTState();
	automaton.descentState = new DescentState();
	automaton.trailState = new TrailState();
	automaton.climbState = new ClimbState();

	automaton.automaticState = new AutomaticState();

	while(!Serial) {
		;
	}

	Serial.begin(57600);
	Serial.println("Starting...");

	app.setup();
}

void loop() {
	app.update();
}
