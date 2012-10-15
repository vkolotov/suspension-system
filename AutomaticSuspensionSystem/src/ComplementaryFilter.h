/*
 * ComplementaryFilter.h
 *
 *  Created on: 14.10.2012
 *      Author: Vlad
 */

#ifndef COMPLEMENTARYFILTER_H_
#define COMPLEMENTARYFILTER_H_

template <class T>
class ComplementaryFilter {
public:
	ComplementaryFilter(float alpha, float beta) : alpha(alpha), beta(beta), lastValue(), lastVelocity(0.0) {};
	~ComplementaryFilter() {};

	T next(T raw, unsigned long dt) {
		// make prediction
		float predictedValue = lastValue + ((float) dt) * lastVelocity;
		float predictedVelocity = lastVelocity;
		// calculate the error
		float residualError = raw - predictedValue;

		predictedValue += alpha * residualError;
		predictedVelocity += beta /((float) dt) * residualError;

		lastValue = predictedValue;
		lastVelocity = predictedVelocity;

		return lastValue;
	}

protected:
	float alpha;
	float beta;
	T lastValue;
	float lastVelocity;
};

#endif /* COMPLEMENTARYFILTER_H_ */
