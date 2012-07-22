/*
 * AutomaticSuspention.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSUSPENSION_H_
#define AUTOMATICSUSPENSION_H_

//#include <Arduino.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <ThreadListener.h>
#include <Suspension.h>
#include <CadenceSystem.h>
#include <ForkAccelerometerSystem.h>
#include <Settings.h>

using namespace std;

class AutomaticSuspension : public ThreadListener {
public:
	AutomaticSuspension();
	virtual ~AutomaticSuspension();

	virtual void init();

	virtual void update();
	virtual int getPriority();

	virtual vector<ThreadListener*> getThreadListeners();

private:

	Suspension* frontSuspension;
	Suspension* rearSuspension;
	CadenceSystem* cadenceSystem;
	ForkAccelerometerSystem* forkAccelerometerSystem;

	vector<ThreadListener*> threadListeners;

};

#endif /* AUTOMATICSUSPENSION_H_ */
