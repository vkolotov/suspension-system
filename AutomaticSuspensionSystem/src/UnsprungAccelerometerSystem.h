/*
 * UnsprungAccelerometerSystem.h
 *
 *  Created on: 26.09.2012
 *      Author: Vlad
 */

#ifndef UNSPRUNGACCELEROMETERSYSTEM_H_
#define UNSPRUNGACCELEROMETERSYSTEM_H_

class UnsprungAccelerometerSystem : public AccelerometerSystem {
public:
	UnsprungAccelerometerSystem(Configuration* config,
			UnsprungAccelerometerSystemConfig* unsprungAccelerometerSystemConfig,
			SpeedSystem* speedSystem)
			: AccelerometerSystem(config,
					&(unsprungAccelerometerSystemConfig->accelerometerSystemConfig)),
					unsprungAccelerometerSystemConfig(unsprungAccelerometerSystemConfig),
					speedSystem(speedSystem),
					timing(unsprungAccelerometerSystemConfig->measuringPeriod) {};
	~UnsprungAccelerometerSystem() {} ;

	void update(unsigned long currentTime) {
		timing.update(currentTime);
		AccelerometerSystem::update(currentTime);
	}

	bool detectActivity() {
		return module >= accelerometerSystemConfig->severityThreshold;
	}

	void activity(unsigned long currentTime) {
		timing.event();
	}

	void inactivity() {

	}

	unsigned long getTimeout() {
		if (speedSystem->getAverageSpeed() < 0.01 || timing.getSum() == 0) {
			return 0;
		}
		uint16_t timeToRearWheel = config->system.wheelBase / speedSystem->getAverageSpeed();
		uint16_t maxBumpsPerMeasuringPeriod = config->system.maxBumpsPerMeter *
				((speedSystem->getAverageSpeed() * unsprungAccelerometerSystemConfig->measuringPeriod) / 1000);
		return map(constrain(timing.getSum(), 1, maxBumpsPerMeasuringPeriod),
				1, maxBumpsPerMeasuringPeriod,
				timeToRearWheel, config->system.maxUnlockTimeout);
	}

	unsigned long getNumberOfBumps() {
		return timing.getSum();
	}

	int16_t getModule() {
		return getModuleXY();
	}

protected:

	UnsprungAccelerometerSystemConfig* unsprungAccelerometerSystemConfig;
	SpeedSystem* speedSystem;
	FrequencyQueue<20, uint8_t> timing;

};

#endif /* UNSPRUNGACCELEROMETERSYSTEM_H_ */
