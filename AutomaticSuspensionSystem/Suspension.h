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
	Suspension(int servoPin, int feedbackPin);
	virtual ~Suspension();

	virtual void attach(int servoPin, int feedbackPin);
	virtual void lock();
	virtual void release();
	virtual void toggle();
	virtual void calibrate();
	virtual bool isCalibrated();

protected:
	CalibratableServo* servo;
	bool isLocked;
};

#endif /* SUSPENSION_H_ */
