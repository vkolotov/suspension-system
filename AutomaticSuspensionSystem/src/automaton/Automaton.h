/*
 * Automaton.h
 *
 *  Created on: 12.09.2012
 *      Author: Vlad
 */

#ifndef AUTOMATON_H_
#define AUTOMATON_H_

#define COMMON_STATE 100
#define SLEEP_STATE 101

#define MANUAL_STATE 0
#define CDT_STATE 10
#define AUTOMATIC_STATE 20

#define CLIMB_STATE 11
#define DESCENT_STATE 12
#define TRAIL_STATE 13

#define ABSORB_STATE 21
#define ACTIVE_STATE 22
#define BURNOUT_STATE 23
#define IDLE_STATE 24
#define PEDAL_STATE 25
#define STANDOVER_STATE 26


class Application;

class Automaton {
public:
	Automaton() {};
	~Automaton() {};

	void update(Application* app) {
		State* tmp = current;
		current = current->transit(app);

		if (tmp != current) {
			previous = tmp;
		}
	}

	State* getState(byte stateId) {
		switch (stateId) {
			case MANUAL_STATE: return manualState;
			case CDT_STATE: return cdtState;
			case AUTOMATIC_STATE: return automaticState;

			case CLIMB_STATE: return climbState;
			case DESCENT_STATE: return descentState;
			case TRAIL_STATE: return trailState;

			case ABSORB_STATE: return absorbState;
			case BURNOUT_STATE: return burnOutState;
			case IDLE_STATE: return idleState;
			case PEDAL_STATE: return pedalState;
			case STANDOVER_STATE: return standOverState;

			case SLEEP_STATE: return sleepState;
		}
		return manualState;
	}

	void setState(uint8_t stateId) {
		previous = current;
		current = getState(stateId);
	}

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
