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
	int16_t sprungSeverityThreshold;
	int16_t unsprungSeverityThreshold;
	uint16_t timeout;
	int16_t sprungReadingsX[20];
	int16_t unsprungReadingsX[20];
	uint8_t dataLength;
};

struct AutomaticBoardMessage {
	int16_t sprungSeverityThreshold;
	int16_t unsprungSeverityThreshold;
};
#endif /* MESSAGES_H_ */
