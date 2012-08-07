#ifndef SUSPENSION_SYSTEM_SETTINGS_H_
#define SUSPENSION_SYSTEM_SETTINGS_H_

// pins
static const unsigned char FRONT_BUTTON_PIN = 7;
static const unsigned char MODE_BUTTON_PIN = 4;
static const unsigned char REAR_BUTTON_PIN = 13;
static const unsigned char CADENCE_PIN = 6;
static const unsigned char SPEED_PIN = 10;
static const unsigned char FRONT_SUSPENSION_CONTROL_PIN = 9;
static const unsigned char FRONT_SUSPENSION_FEADBACK_PIN = 0;
static const unsigned char REAR_SUSPENSION_CONTROL_PIN = 12;
static const unsigned char REAR_SUSPENSION_FEADBACK_PIN = 1;

// cadence system
static const unsigned short MINIMUM_CADENCE_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per minute
static const unsigned short MAXIMUM_CADENCE_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per minute
static const unsigned short AVERAGE_CADENCE_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per minute

// speed system
static const unsigned short MINIMUM_SPEED_TIME = (60.0f / 180.0f) * 1000; // 180 revolutions per minute
static const unsigned short MAXIMUM_SPEED_TIME = (60.0f / 40.0f) * 1000; // 40 revolutions per minute
static const unsigned short AVERAGE_SPEED_TIME = (60.0f / 70.0f) * 1000; // 70 revolutions per minute

// buttons
static const unsigned short BUTTON_DEBOUNCE_DURATION = 200;

// suspension
static const unsigned short POWER_SAVE_THRESHOLD = 500;
static const unsigned short CALIBRATION_DELAY = 400;
static const unsigned char CALIBRATION_THRESHOLD = 5;
static const unsigned char CALIBRATION_STEP = 10;
static const unsigned char MIN_ANGLE = 40;
static const unsigned char MAX_ANGLE = 160;

#endif
