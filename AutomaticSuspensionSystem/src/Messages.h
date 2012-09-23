/*
 * Messages.h
 *
 *  Created on: 10.09.2012
 *      Author: Vlad
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

struct TelemetryMessage {
	int8_t speed;
	int16_t cadence;
	int32_t distance;
	int16_t heartRate;
	float floatField;
	double doubleField;
};

#endif /* MESSAGES_H_ */
