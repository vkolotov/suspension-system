#include "AutomaticSuspensionSystem.h"

Configuration* config = loadConfiguration();
Automaton automaton;
Application app(config, &automaton);

void setup() {
	initConfiguration();
	Serial.begin(38400);
	delay(1000);
	Serial.println("Starting...");

	app.bluetoothSystem = new BluetoothSystem(&app);

	automaton.calibrationState = new CalibrationState();
	automaton.sleepState = new SleepState();
	automaton.previous = automaton.current = automaton.manualState = new ManualState();

	automaton.cdtState = new CDTState();
	automaton.descentState = new DescentState();
	automaton.trailState = new TrailState();
	automaton.climbState = new ClimbState();


	AutomaticState* automaticState = new AutomaticState();
	automaton.automaticState = automaticState;
	automaton.absorbState = new AbsorbState();
	automaton.burnOutState = new BurnOutState();
	automaton.pedalState = new PedalState();
	automaton.standOverState = new StandOverState();
	automaton.idleState = new IdleState();

	app.setup();
}

void loop() {
	app.update();
}
