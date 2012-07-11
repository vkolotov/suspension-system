/*
 * CadenceSystem.h
 *
 *  Created on: 09.07.2012
 *      Author: Vlad
 */

#ifndef CADENCESYSTEM_H_
#define CADENCESYSTEM_H_
#include <ThreadListener.h>
#include <Arduino.h>


class CadenceSystem : public ThreadListener {
public:
	CadenceSystem();
	virtual ~CadenceSystem();

	virtual int getCadence();
	virtual int getAverageCadence();
	virtual bool isPedalling();
	virtual void update();
	virtual int getPriority();

protected:

	int pin;
	int state;
	long lastRevolution;
	long previousRevolution;
};

#endif /* CADENCESYSTEM_H_ */
