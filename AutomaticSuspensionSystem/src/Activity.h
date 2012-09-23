/*
 * Activity.h
 *
 *  Created on: 16.09.2012
 *      Author: Vlad
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

class Activity {
public:
	Activity() {};
	~Activity();

	virtual void init() = 0;

	virtual void update(unsigned long time) = 0;

};

#endif /* ACTIVITY_H_ */
