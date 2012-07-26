/*
 * Button.h
 *
 *  Created on: 26.07.2012
 *      Author: Vlad
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>

class Button {
public:
	Button();
	Button(int pin);
	virtual ~Button();

	bool isPushed();

protected:
	int pin;
	long lastTime;
};

#endif /* BUTTON_H_ */
