/*
 * CadenceSystemMock.h
 *
 *  Created on: 28.09.2012
 *      Author: Vlad
 */

#ifndef CADENCESYSTEMMOCK_H_
#define CADENCESYSTEMMOCK_H_

class CadenceSystemMock : public CadenceSystem {
public:
	CadenceSystemMock(CadenceSystemConfig* cfg) : CadenceSystem(cfg), pinValue(false) {};
	virtual ~CadenceSystemMock() {};

	bool qualifier(unsigned long currentTime) {
		return pinValue;
	}

	bool pinValue;
};

#endif /* CADENCESYSTEMMOCK_H_ */
