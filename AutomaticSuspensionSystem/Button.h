/*
 * Button.h
 *
 *  Created on: 26.07.2012
 *      Author: Vlad
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <ThreadListener.h>

class Button : public ThreadListener {
public:
	Button();
	Button(unsigned char pin, bool isToggle);
	virtual ~Button();

	void update();

	bool checkNoise();

	bool isPushed();

protected:
	unsigned char pin;
	long lastTime;

	bool pushed;
	bool isToggle;
};

#endif /* BUTTON_H_ */
