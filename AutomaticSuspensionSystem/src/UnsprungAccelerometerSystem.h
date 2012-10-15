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
		return currentX - idleX >= accelerometerSystemConfig->severityThreshold;
	}

	void activity(unsigned long currentTime) {
		timing.event();
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

protected:

	UnsprungAccelerometerSystemConfig* unsprungAccelerometerSystemConfig;
	SpeedSystem* speedSystem;
	FrequencyQueue<20> timing;

};

#endif /* UNSPRUNGACCELEROMETERSYSTEM_H_ */
