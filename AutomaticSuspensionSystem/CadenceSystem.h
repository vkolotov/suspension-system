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
#include <Settings.h>
#include <StandardCplusplus.h>
#include <vector>

using namespace std;

class CadenceSystem : public ThreadListener {
public:
	CadenceSystem(unsigned char pin);
	virtual ~CadenceSystem();

	bool isPedalling();
	void update();

	unsigned short getAverageTime();

protected:

	unsigned char pin;
	unsigned char state;
	long lastRevolution;
	bool pedalling;
	vector<long> timing;
};

#endif /* CADENCESYSTEM_H_ */
