/*
 * CalibratableServo.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef CALIBRATABLESERVO_H_
#define CALIBRATABLESERVO_H_

#include "Servo.h"

class CalibratableServo: public Servo {
public:
	CalibratableServo();
	CalibratableServo(int pin, int feedbackPin);
	virtual ~CalibratableServo();

	virtual void calibrate();
	virtual bool isCalibrated();
	virtual void writeMax();
	virtual void writeMin();
	virtual int getMaxAngle();
	virtual int getMinAngle();
	virtual int getRawFeedback();
protected:
	int pin;
	int feedbackPin;

	int calibrationStep;
	int calibrationThreshold;
	unsigned long calibrationDelay;
	bool calibrated;

	int minAngle;
	int maxAngle;
};

#endif /* CALIBRATABLESERVO_H_ */
