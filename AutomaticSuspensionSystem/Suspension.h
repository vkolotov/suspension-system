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
#include <Activity.h>

class Suspension : public Activity {
public:
	Suspension();
	Suspension(unsigned char servoPin, unsigned char feedbackPin, bool isReverse);
	virtual ~Suspension();

	void lock();
	void release();
	void toggle();
	void calibrate();
	void update(unsigned long currentTime);
	bool locked();
	void attach();
	void detach();

protected:
	CalibratableServo* servo;
	bool isLocked;
	bool isReverse;

	long lastTime;
};

#endif /* SUSPENSION_H_ */
