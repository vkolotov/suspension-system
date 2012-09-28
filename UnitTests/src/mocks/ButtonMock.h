/*
 * ButtonMock.h
 *
 *  Created on: 29.09.2012
 *      Author: Vlad
 */

#ifndef BUTTONMOCK_H_
#define BUTTONMOCK_H_

class ButtonMock : public Button {
public:
	ButtonMock(unsigned char pin, bool isToggle, uint16_t debonceDuration, unsigned short pinReference = HIGH)
			: Button(pin, isToggle, debonceDuration, pinReference), pinValue(false) {};
	~ButtonMock() {};

	bool qualifier(unsigned long currentTime) {
		return pinValue;
	}

	bool pinValue;
};

#endif /* BUTTONMOCK_H_ */
