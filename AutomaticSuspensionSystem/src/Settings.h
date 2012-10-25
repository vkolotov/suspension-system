#ifndef SUSPENSION_SYSTEM_SETTINGS_H_
#define SUSPENSION_SYSTEM_SETTINGS_H_


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
static const unsigned char CALIBRATION_STEP = 10;
static const unsigned char MIN_ANGLE = 0;
static const unsigned char MAX_ANGLE = 180;

// modes
static const uint8_t MODE_MANUAL = 0;
static const uint8_t MODE_SEMIAUTOMATIC = 1;
static const uint8_t MODE_AUTOMATIC = 2;

// power save
static const uint8_t SERVO_RELAY_PIN = 6;
static const uint8_t SERIAL_RELAY_PIN = 7;
static const uint8_t I2C_RELAY_PIN = 13;
static const uint8_t SLEEP_INTERRUPTION_NUMBER = 0;
static const uint16_t SERO_STANDBY_TIMEOUT = 500;
static const uint32_t SLEEP_TIMEOUT = 300000;


// system
static const uint16_t WHEEL_BASE = 1051;


struct ButtonsSystem {
	uint8_t frontPin;
	uint8_t modePin;
	uint8_t rearPin;
	uint16_t debounceDuration;

	uint8_t frontPinReferenceValue;
	uint8_t modePinReferenceValue;
	uint8_t rearPinReferenceValue;

/*	uint8_t switchModeDelay;
	uint8_t servoCalibrationDelay;
	uint8_t anglesCalibrationDelay;
	uint8_t resetDelay;
	uint8_t toggleSerialDelay;
	uint8_t accelerometerCalibrationDelay;*/
};

struct FrequencySystemConfig {
	uint8_t pin;
	uint16_t minTime;
	uint16_t maxTime;
	uint8_t referenceValue;
};

struct CadenceSystemConfig {
	FrequencySystemConfig frequencySystemConfig;
	uint16_t timeoutCorrection;
};

struct SpeedSystemConfig {
	FrequencySystemConfig frequencySystemConfig;
	uint16_t wheelLength;
};

struct SystemConfig {
	uint16_t wheelBase;
	uint16_t maxUnlockTimeout;
	float headTubeGradient;
	float maxBumpsPerMeter;
};

struct AccelerometerSystemConfig {
	uint16_t address;
	uint8_t range;
	int16_t severityThreshold;
	uint8_t calibrationStep;
	uint16_t noiseThreshold;
};

struct UnsprungAccelerometerSystemConfig {
	AccelerometerSystemConfig accelerometerSystemConfig;
	uint16_t measuringPeriod;
};

struct SprungAccelerometerSystemConfig {
	AccelerometerSystemConfig accelerometerSystemConfig;
	float angleFilterAlpha;
	float angleFilterBeta;
};

struct SuspensionSystemConfig {
	uint8_t controlPin;
	uint8_t feedbackPin;
	uint16_t calibrationDelay;
	uint8_t calibrationThreshold;
	uint8_t calibrationStep;
	uint16_t minAngle;
	uint16_t maxAngle;
	uint8_t modes;
	uint16_t angles[9];
	uint8_t mode;
};

struct PowerSaveSystemConfig {
	uint8_t servoRelayPin;
	uint8_t serialRelayPin;
	uint8_t i2cRelayPin;
	uint8_t sleepInterruption;
	uint16_t servoStandByTimeout;
	bool isSleepEnabled;
	uint32_t sleepTimeout;
	bool isBluetoothSleepEnabled;
	uint32_t bluetoothSleepTimeout;
	bool isServoPowerOn;
	bool isSerialPowerOn;
	bool isi2cPowerOn;
};

struct SemiautomaticStateConfig {

	float climbGradient;
	float descendGradient;
	float transitionGap;
	uint16_t averageDegreeMeasuringPeriod;
};

struct Configuration {
	bool virgin;
	SystemConfig system;
	ButtonsSystem buttons;
	SpeedSystemConfig speed;
	CadenceSystemConfig cadence;
	SuspensionSystemConfig frontSuspension;
	SuspensionSystemConfig rearSuspension;
	SprungAccelerometerSystemConfig sprungAccelerometerSystem;
	UnsprungAccelerometerSystemConfig unsprungAccelerometerSystem;
	PowerSaveSystemConfig powerSave;
	SemiautomaticStateConfig semiautomaticStateConfig;
};

Configuration EEMEM cfg = {
		// reference
		true,
		// SystemConfig
		{WHEEL_BASE, 4000, 0.164, 2.0},
		// ButtonsSystem
		{FRONT_BUTTON_PIN, MODE_BUTTON_PIN, REAR_BUTTON_PIN, BUTTON_DEBOUNCE_DURATION, LOW, LOW, LOW},
		// SpeedSystemConfig
		{{SPEED_PIN, MINIMUM_SPEED_TIME, MAXIMUM_SPEED_TIME, LOW}, WHEEL_LENGTH},
		// CadenceSystemConfig
		{{CADENCE_PIN, MINIMUM_CADENCE_TIME, MAXIMUM_CADENCE_TIME, LOW}, 300},
		// SuspensionSystemConfig frontSuspension
		{FRONT_SUSPENSION_CONTROL_PIN, FRONT_SUSPENSION_FEADBACK_PIN, CALIBRATION_DELAY, CALIBRATION_THRESHOLD, CALIBRATION_STEP, MIN_ANGLE, MAX_ANGLE, 3, {20, 80, 170}, 0},
		// SuspensionSystemConfig rearSuspension
		{REAR_SUSPENSION_CONTROL_PIN, REAR_SUSPENSION_FEADBACK_PIN, CALIBRATION_DELAY, CALIBRATION_THRESHOLD, CALIBRATION_STEP, MIN_ANGLE, MAX_ANGLE, 2, {20, 170}, 0},
		// AccelerometerSystemConfig sprungAccelerometerSystem
		{{ADXL345_ADDRESS_ALT_LOW, 0x3, -20, 15}, /*alpha*/0.02, /*beta*/ 0.00001},
		// AccelerometerSystemConfig unsprungAccelerometerSystem
		{{ADXL345_ADDRESS_ALT_HIGH, 0x3, 60, 15}, 4000},
		// PowerSaveSystemConfig powerSave
		{SERVO_RELAY_PIN, SERIAL_RELAY_PIN, I2C_RELAY_PIN, SLEEP_INTERRUPTION_NUMBER, SERO_STANDBY_TIMEOUT, true, SLEEP_TIMEOUT, false, 120000, /*servo*/true, /*serial*/true, /*i2c*/false},
		// SemiautomaticStateConfig
		{/*11 degrees*/0.20, /*-11 degrees*/-0.20, /*3 degrees*/0.052f, 2000}
};

void initConfiguration() {
	if (eeprom_read_byte((const uint8_t *) &cfg) == true) {
		eeprom_write_byte((uint8_t *) 0, false);
		Configuration* conf = new Configuration();
		eeprom_read_block(conf, &cfg, sizeof(Configuration));
		eeprom_write_block(conf, &cfg + sizeof(Configuration), sizeof(Configuration));
		delete conf;
	}
}

Configuration* loadConfiguration() {
	Configuration* result = new Configuration();
	eeprom_read_block(result, &cfg, sizeof(Configuration));
	return result;
}

void reloadConfiguration(Configuration* config) {
	eeprom_read_block(config, &cfg, sizeof(Configuration));
}

void saveConfiguration(Configuration* config) {
	eeprom_write_block(config, &cfg, sizeof(Configuration));
}

void resetConfiguration(Configuration* config) {
	eeprom_read_block(config, &cfg + sizeof(Configuration), sizeof(Configuration));
}

#endif
