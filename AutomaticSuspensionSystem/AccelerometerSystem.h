/*
 * AccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef ACCELEROMETERSYSTEM_H_
#define ACCELEROMETERSYSTEM_H_
#include <Activity.h>

class AccelerometerSystem : public Activity {
public:
	AccelerometerSystem();
	virtual ~AccelerometerSystem();

	void update() = 0;

	virtual bool isActive() = 0;
};

#endif /* ACCELEROMETERSYSTEM_H_ */
