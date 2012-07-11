/*
 * Suspension.h
 *
 *  Created on: 08.07.2012
 *      Author: Vlad
 */

#ifndef SUSPENSION_H_
#define SUSPENSION_H_

#include <CalibratableServo.h>
#include <Arduino.h>

class Suspension {
public:
	Suspension();
	Suspension(int servoPin, int feedbackPin);
	virtual ~Suspension();

	virtual void attach(int servoPin, int feedbackPin);
	virtual void lock();
	virtual void release();
	virtual void calibrate();
	virtual bool isCalibrated();

protected:
	CalibratableServo* servo;
};

#endif /* SUSPENSION_H_ */
