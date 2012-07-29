	/*
 * AutomaticSuspention.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSUSPENSION_H_
#define AUTOMATICSUSPENSION_H_

//#include <Arduino.h>
#include <ThreadListener.h>
#include <Suspension.h>
#include <CadenceSystem.h>
#include <ForkAccelerometerSystem.h>
#include <Settings.h>
#include <Button.h>

#include <StandardCplusplus.h>
#include <vector>

using namespace std;

class AutomaticSuspension : public ThreadListener {
public:
	AutomaticSuspension();
	virtual ~AutomaticSuspension();

	void init();

	void update();

	vector<ThreadListener*> getThreadListeners();

private:

	Suspension* frontSuspension;
	Suspension* rearSuspension;
	CadenceSystem* cadenceSystem;
	ForkAccelerometerSystem* forkAccelerometerSystem;

	Button* frontButton;
	Button* rearButton;
	Button* modeButton;

	vector<ThreadListener*> threadListeners;

	bool mode;

};

#endif /* AUTOMATICSUSPENSION_H_ */
