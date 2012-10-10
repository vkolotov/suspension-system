/*
 * State.h
 *
 *  Created on: 11.09.2012
 *      Author: Vlad
 */

#ifndef STATE_H_
#define STATE_H_

class Application;

class State {
public:
	State() {};
	virtual ~State() {};

	virtual bool transitable(Application* app) = 0;

	virtual State* transit(Application* app) = 0;

	virtual uint8_t getId() = 0;

};

#endif /* STATE_H_ */
