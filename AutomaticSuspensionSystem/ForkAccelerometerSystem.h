/*
 * ForkAccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef FORKACCELEROMETERSYSTEM_H_
#define FORKACCELEROMETERSYSTEM_H_

#include "AccelerometerSystem.h"
#include <Wire.h>
//#include <ADXL345.h>

class ForkAccelerometerSystem: public AccelerometerSystem {
public:
	ForkAccelerometerSystem();
	virtual ~ForkAccelerometerSystem();

	void update();

	bool isActive();

protected:

	//ADXL345 accel;

};

#endif /* FORKACCELEROMETERSYSTEM_H_ */
