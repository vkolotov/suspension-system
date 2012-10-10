/*
 * Messages.h
 *
 *  Created on: 10.09.2012
 *      Author: Vlad
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

struct ManualTelemetry {
	uint8_t state;
	uint8_t speed;
	uint16_t cadence;
	uint8_t suspensionMode;
};

struct CDTTelemetry {
	uint8_t state;
	uint8_t speed;
	uint16_t cadence;
	uint8_t suspensionMode;
	int8_t gradient;
	int8_t climbGradient;
	int8_t descendGradient;
};

struct CDTBoardMessage {
	int8_t climbGradient;
	int8_t descendGradient;
};

#endif /* MESSAGES_H_ */
