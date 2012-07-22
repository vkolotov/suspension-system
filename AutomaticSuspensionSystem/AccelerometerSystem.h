/*
 * AccelerometerSystem.h
 *
 *  Created on: 19.07.2012
 *      Author: Vlad
 */

#ifndef ACCELEROMETERSYSTEM_H_
#define ACCELEROMETERSYSTEM_H_
#include <ThreadListener.h>

class AccelerometerSystem : public ThreadListener {
public:
	AccelerometerSystem();
	virtual ~AccelerometerSystem();

	virtual void update() = 0;
	virtual int getPriority() = 0;

	virtual bool isActive() = 0;
};

#endif /* ACCELEROMETERSYSTEM_H_ */
