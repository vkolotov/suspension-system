	/*
 * AutomaticSuspention.h
 *
 *  Created on: 07.07.2012
 *      Author: Vlad
 */

#ifndef AUTOMATICSUSPENSION_H_
#define AUTOMATICSUSPENSION_H_

#include <Activity.h>
#include <Suspension.h>
#include <CadenceSystem.h>
#include <ForkAccelerometerSystem.h>
#include <Settings.h>
#include <Button.h>

class AutomaticSuspension : public Activity {
public:
	AutomaticSuspension();
	virtual ~AutomaticSuspension();

	void init();

	void update(unsigned long currentTime);

	Suspension* frontSuspension;
	Suspension* rearSuspension;
	CadenceSystem* cadenceSystem;
	ForkAccelerometerSystem* forkAccelerometerSystem;

	Button* frontButton;
	Button* rearButton;
	Button* modeButton;

};

#endif /* AUTOMATICSUSPENSION_H_ */
