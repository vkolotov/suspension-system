/*
 * Suspension.h
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#ifndef SUSPENSION_H_
#define SUSPENSION_H_

#include <Servo.h>
#include <CalibratableServo.h>

class Suspension {
public:
	Suspension();
	Suspension(unsigned char servoPin, unsigned char feedbackPin);
	virtual ~Suspension();

	void lock();
	void release();
	void toggle();
	void calibrate();

	bool locked();

protected:
	CalibratableServo* servo;
	bool isLocked;
};

#endif /* SUSPENSION_H_ */
