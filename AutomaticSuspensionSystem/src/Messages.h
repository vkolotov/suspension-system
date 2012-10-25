/*
 * Messages.h
 *
 *  Created on: 10.09.2012
 *      Author: Vlad
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

struct ManualTelemetry {
	uint16_t clockSpeed;
	uint8_t state;
	uint8_t speed;
	uint16_t cadence;
	uint8_t suspensionMode;
};

struct CDTTelemetry {
	uint16_t clockSpeed;
	uint8_t state;
	uint8_t speed;
	uint16_t cadence;
	uint8_t suspensionMode;
	int8_t gradient;
	int8_t climbGradient;
	int8_t descendGradient;
	int16_t rawGradients[20];
	int16_t filteredGradients[20];
	uint8_t dataLength;
};

struct CDTBoardMessage {
	int8_t climbGradient;
	int8_t descendGradient;
};


struct AutomaticTelemetry {
	uint16_t clockSpeed;
	uint8_t state;
	uint8_t speed;
	uint16_t cadence;
	int16_t severityThreshold;
	int16_t idleValue;
	uint16_t timeout;
};

struct AutomaticBoardMessage {
	int16_t severityThreshold;
};
#endif /* MESSAGES_H_ */
