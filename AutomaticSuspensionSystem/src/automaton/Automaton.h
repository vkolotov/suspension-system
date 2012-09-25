/*
 * Automaton.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef AUTOMATON_H_
#define AUTOMATON_H_

class Application;

class Automaton {
public:
	Automaton() {};
	~Automaton() {};

	void update(Application* app) {
		State* tmp = current;
		current = current->transit(app);

		if (!current) {
			Serial.println("Current is null!");
		}

		if (tmp != current) {
			previous = tmp;
			Serial.print("Prev: ");
			Serial.print(tmp->getName());
			Serial.print(" Curr: ");
			Serial.println(current->getName());
		}
	}

	State* calibrationState;
	State* sleepState;
	State* manualState;

	State* cdtState;
	State* descentState;
	State* trailState;
	State* climbState;

	State* automaticState;
	State* idleState;
	State* absorbState;
	State* burnOutState;
	State* pedalState;
	State* standOverState;

	State* current;
	State* previous;
};

#endif /* AUTOMATON_H_ */
