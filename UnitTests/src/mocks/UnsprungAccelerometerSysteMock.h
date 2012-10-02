/*
 * UnsprungAccelerometerSysteMock.h
 *
 *  Created on: 29.09.2012
 *      Author: Vlad
 */

#ifndef UNSPRUNGACCELEROMETERSYSTEMOCK_H_
#define UNSPRUNGACCELEROMETERSYSTEMOCK_H_

class UnsprungAccelerometerSysteMock : public UnsprungAccelerometerSystem {
public:
	UnsprungAccelerometerSysteMock(Configuration* config,
			UnsprungAccelerometerSystemConfig* unsprungAccelerometerSystemConfig,
			SpeedSystem* speedSystem)
			: UnsprungAccelerometerSystem(config, unsprungAccelerometerSystemConfig, speedSystem) {};
	virtual ~UnsprungAccelerometerSysteMock() {};

	virtual void readAccelerometer() {
	}

	void setAccelerationX(uint16_t x) {
		currentX = x;
	}
};

#endif /* UNSPRUNGACCELEROMETERSYSTEMOCK_H_ */
