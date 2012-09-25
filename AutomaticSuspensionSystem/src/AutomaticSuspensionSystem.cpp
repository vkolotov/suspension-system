#include "AutomaticSuspensionSystem.h"

Configuration* config = loadConfiguration();
Automaton automaton;
Application app(config, &automaton);

void setup() {

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
//	automaticState->activeStates = new State*[4];
	automaton.automaticState = automaticState;
//	automaticState->activeStates[0] = automaton.absorbState = new AbsorbState();
//	automaticState->activeStates[1] = automaton.burnOutState = new BurnOutState();
//	automaticState->activeStates[2] = automaton.pedalState = new PedalState();
//	automaticState->activeStates[3] = automaton.standOverState = new StandOverState();
	automaton.idleState = new IdleState();

	app.setup();
}

void loop() {
	app.update();
}
