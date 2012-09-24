#ifndef SUSPENSION_SYSTEM_SETTINGS_H_
#define SUSPENSION_SYSTEM_SETTINGS_H_

#include <avr/eeprom.h>

// pins
static const unsigned char FRONT_BUTTON_PIN = 3;
static const unsigned char MODE_BUTTON_PIN = 12;
static const unsigned char REAR_BUTTON_PIN = 11;
static const unsigned char CADENCE_PIN = 2;
static const unsigned char SPEED_PIN = 10;
static const unsigned char FRONT_SUSPENSION_CONTROL_PIN = 8;
static const unsigned char FRONT_SUSPENSION_FEADBACK_PIN = 1;
static const unsigned char REAR_SUSPENSION_CONTROL_PIN = A2;
static const unsigned char REAR_SUSPENSION_FEADBACK_PIN = 0;
static const unsigned char HEART_RATE_PIN = 9;

// cadence system
static const unsigned short MINIMUM_CADENCE_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per minute
static const unsigned short MAXIMUM_CADENCE_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per minute
static const unsigned short AVERAGE_CADENCE_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per minute

// speed system
static const unsigned short WHEEL_LENGTH = 2133;
static const unsigned short MINIMUM_SPEED_TIME = 96;//((float)(WHEEL_LENGTH * 36)) / ((float)(80 * 10)); // 80 km/h
static const unsigned short MAXIMUM_SPEED_TIME = 3839;//((float)(WHEEL_LENGTH * 36)) / ((float)(2 * 10)); // 2 km/h
static const unsigned short AVERAGE_SPEED_TIME = 512;//((float)(WHEEL_LENGTH * 36)) / ((float)(15 * 10)); // 15 km/h

// buttons
static const unsigned short BUTTON_DEBOUNCE_DURATION = 500;

// suspension
static const unsigned short CALIBRATION_DELAY = 200;
static const unsigned char CALIBRATION_THRESHOLD = 3;
static const unsigned char CALIBRATION_STEP = 5;
static const unsigned char MIN_ANGLE = 20;
static const unsigned char MAX_ANGLE = 170;

// modes
static const uint8_t MODE_MANUAL = 0;
static const uint8_t MODE_SEMIAUTOMATIC = 1;
static const uint8_t MODE_AUTOMATIC = 2;

// power save
static const uint8_t SERVO_RELAY_PIN = 6;
static const uint8_t SERIAL_RELAY_PIN = 7;
static const uint8_t I2C_RELAY_PIN = 13;
static const uint8_t SLEEP_INTERRUPTION_NUMBER = 4;
static const uint16_t SERO_STANDBY_TIMEOUT = 500;
static const uint32_t SLEEP_TIMEOUT = 99999999;


struct ButtonsSystem {
	uint8_t frontPin;
	uint8_t modePin;
	uint8_t rearPin;
	uint8_t debounceDuration;

	uint8_t frontPinReferenceValue;
	uint8_t modePinReferenceValue;
	uint8_t rearPinReferenceValue;
};

struct FrequencySystemConfig {
	uint8_t pin;
	uint16_t minTime;
	uint16_t maxTime;
	int16_t referenceValue;
};

struct CadenceSystemConfig {
	FrequencySystemConfig frequencySystemConfig;
};

struct SpeedSystemConfig {
	FrequencySystemConfig frequencySystemConfig;
	uint16_t wheelLength;
};

struct SystemConfig {
	uint8_t mode;
	float headTubeGradient;
};

struct AccelerometerSystemConfig {
	uint8_t address;
	uint8_t range;
	uint16_t severityThreshold;
	uint16_t noiseThreshold;
};

struct SuspensionSystemConfig {
	uint8_t controlPin;
	uint8_t feedbackPin;
	uint16_t calibrationDelay;
	uint8_t calibrationThreshold;
	uint8_t calibrationStep;
	uint8_t minAngle;
	uint8_t maxAngle;
	uint8_t modes;
	uint8_t angles[9];
};

struct PowerSaveSystemConfig {
	uint8_t servoRelayPin;
	uint8_t serialRelayPin;
	uint8_t i2cRelayPin;
	uint8_t sleepInterruption;
	uint16_t servoStandByTimeout;
	uint32_t sleepTimeout;

	boolean isServoPowerOn;
	boolean isSerialPowerOn;
	boolean isi2cPowerOn;
};

struct SemiautomaticStateConfig {

	float climbGradient;
	float descentGradient;
	float transitionGap;
	uint16_t averageDegreeMeasuringPeriod;
};

struct Configuration {

	SystemConfig system;
	ButtonsSystem buttons;
	SpeedSystemConfig speed;
	CadenceSystemConfig cadence;
	SuspensionSystemConfig frontSuspension;
	SuspensionSystemConfig rearSuspension;
	AccelerometerSystemConfig sprungAccelerometerSystem;
	AccelerometerSystemConfig unsprungAccelerometerSystem;
	PowerSaveSystemConfig powerSave;
	SemiautomaticStateConfig semiautomaticStateConfig;

};

Configuration EEMEM cfg = {
		// SystemConfig
		{MODE_AUTOMATIC, 0.164},
		// ButtonsSystem
		{FRONT_BUTTON_PIN, MODE_BUTTON_PIN, REAR_BUTTON_PIN, BUTTON_DEBOUNCE_DURATION, LOW, LOW, LOW},
		// SpeedSystemConfig
		{{SPEED_PIN, MINIMUM_SPEED_TIME, MAXIMUM_SPEED_TIME, LOW}, WHEEL_LENGTH},
		// CadenceSystemConfig
		{{CADENCE_PIN, MINIMUM_CADENCE_TIME, MAXIMUM_CADENCE_TIME, LOW}},
		// SuspensionSystemConfig frontSuspension
		{FRONT_SUSPENSION_CONTROL_PIN, FRONT_SUSPENSION_FEADBACK_PIN, CALIBRATION_DELAY, CALIBRATION_THRESHOLD, CALIBRATION_STEP, MIN_ANGLE, MAX_ANGLE, 3, {20, 80, 170}},
		// SuspensionSystemConfig rearSuspension
		{REAR_SUSPENSION_CONTROL_PIN, REAR_SUSPENSION_FEADBACK_PIN, CALIBRATION_DELAY, CALIBRATION_THRESHOLD, CALIBRATION_STEP, MIN_ANGLE, MAX_ANGLE, 2, {20, 170}},
		// AccelerometerSystemConfig sprungAccelerometerSystem
		{ADXL345_ADDRESS_ALT_LOW, 0x2, 60, 15},
		// AccelerometerSystemConfig unsprungAccelerometerSystem
		{ADXL345_ADDRESS_ALT_HIGH, 0x2, 60, 15},
		// PowerSaveSystemConfig powerSave
		{SERVO_RELAY_PIN, SERIAL_RELAY_PIN, I2C_RELAY_PIN, SLEEP_INTERRUPTION_NUMBER, SERO_STANDBY_TIMEOUT, SLEEP_TIMEOUT, /*servo*/true, /*serial*/false, /*i2c*/true},
		// SemiautomaticStateConfig
		{/*11 degrees*/0.20, /*-11 degrees*/-0.20, /*3 degrees*/0.052f, 2000}
};

Configuration* loadConfiguration() {
	Configuration* result = new Configuration();
	eeprom_read_block(result, &cfg, sizeof(cfg));
	return result;
}

void saveConfiguration(Configuration* config) {
	eeprom_write_block(config, &cfg, sizeof(cfg));
}

void resetConfiguration(Configuration* config) {
	//TODO calculate address
	eeprom_read_block((void*)config, (void*)sizeof(&config), sizeof(config));
}

#endif
