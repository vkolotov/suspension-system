/*
 * ThreadListener.h
 *
 *  Created on: 10.07.2012
 *      Author: Vlad
 */

#ifndef THREADLISTENER_H_
#define THREADLISTENER_H_

#define HIGH_PRIORITY 0
#define NORMAL_PRIORITY 1
#define LOW_PRIORITY 2

class ThreadListener {
public:

	virtual void update() = 0;
	virtual int getPriority() = 0;
};

#endif /* THREADLISTENER_H_ */
