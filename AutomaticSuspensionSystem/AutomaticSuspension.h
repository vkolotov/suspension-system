/*
 * AutomaticSuspention.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSUSPENSION_H_
#define AUTOMATICSUSPENSION_H_
#include <Arduino.h>
#include <ThreadListener.h>
#include <Suspension.h>
#include <CadenceSystem.h>

#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

static const int FRONT_SERVO_PIN = 9;
static const int FRONT_SERVO_FEEDBACK_PIN = 1;
static const int CADENCE_SENSOR_PIN = 4;

class AutomaticSuspension : public ThreadListener {
public:
	AutomaticSuspension();
	virtual ~AutomaticSuspension();

	virtual void init();

	virtual void update();
	virtual int getPriority();

	virtual vector<ThreadListener*> getThreadListeners();
	virtual int getThreadListenersCount();

private:

	Suspension* frontSuspension;
	Suspension* rearSuspension;
	CadenceSystem* cadenceSystem;

	vector<ThreadListener*> threadListeners;

};

#endif /* AUTOMATICSUSPENSION_H_ */
