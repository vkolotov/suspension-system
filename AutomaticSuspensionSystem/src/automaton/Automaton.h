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
			Serial.print("Previous: ");
			Serial.print(tmp->getName());
			Serial.print(" Current: ");
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

	State* current;
	State* previous;
};


//	void updateAutomaticMode() {
//		if (speedSystem.isProcessing() && cadenceSystem.isProcessing()) {
//			if (unsprungAccelerometerSystem.isActive()) {
//				rearSuspension.release();
//			} else {
//				rearSuspension.lock();
//			}
//			if (sprungAccelerometerSystem.isActive()) {
//				frontSuspension.release();
//			} else {
//				frontSuspension.lock();
//			}
//		} else {
//			rearSuspension.release();
//			frontSuspension.release();
//		}
//	}

#endif /* AUTOMATON_H_ */
