/*
 * CalibratableServo.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef CALIBRATABLESERVO_H_
#define CALIBRATABLESERVO_H_

#include <Arduino.h>
#include <Servo.h>
#include <Settings.h>

class CalibratableServo: public Servo {
public:
	CalibratableServo();
	CalibratableServo(unsigned char pin, unsigned char feedbackPin);
	virtual ~CalibratableServo();

	void bind();
	void calibrate();
	void writeMax();
	void writeMin();
	int getRawFeedback();
protected:
	unsigned char pin;
	unsigned char feedbackPin;

	unsigned short minAngle;
	unsigned short maxAngle;
};

#endif /* CALIBRATABLESERVO_H_ */
