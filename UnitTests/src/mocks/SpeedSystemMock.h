/*
 * SpeedSystemMock.h
 *
 *  Created on: 28.09.2012
 *      Author: Vlad
 */

#ifndef SPEEDSYSTEMMOCK_H_
#define SPEEDSYSTEMMOCK_H_

class SpeedSystemMock : public SpeedSystem {
public:
	SpeedSystemMock(SpeedSystemConfig* speedSystemConfig) : SpeedSystem(speedSystemConfig), pinValue(0), speed(-1) {};
	~SpeedSystemMock() {};

	bool qualifier(unsigned long currentTime) {
		return pinValue;
	}

	float getAverageSpeed() {
		if (speed == -1) {
			return SpeedSystem::getAverageSpeed();
		}
		return speed;
	}

	bool pinValue;
	float speed;

};

#endif /* SPEEDSYSTEMMOCK_H_ */
