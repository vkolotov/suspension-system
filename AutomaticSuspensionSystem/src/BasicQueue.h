/*
 * BasicQueue.h
 *
 *  Created on: Aug 10, 2012
 *      Author: vkolotov
 */

#ifndef BASICQUEUE_H_
#define BASICQUEUE_H_

template <uint8_t capacity, typename Data> class BasicQueue {
public:
	BasicQueue() : cursor(0), length(0), iterator(0), iteratorIndex(0) {

	}
	~BasicQueue() {}

	Data& push(Data entry) {
		if (cursor >= capacity - 1) {
			cursor = 0;
		} else {
			cursor++;
		}
		if (length < capacity) {
			length++;
		}
		Data& replacingValue = data[cursor];
		data[cursor] = entry;
		return replacingValue;
	}

	virtual void clear() {
		cursor = 0;
		length = 0;
	}

	Data& last() {
		return data[cursor];
	}

	unsigned char size() {
		return length;
	}

	void iteratorReset() {
		iterator = cursor - length + 1;
		if (iterator < 0) {
			iterator = cursor + 1;
		}
		iteratorIndex = 0;
	}

	Data& iteratorNext() {
		Data& tmp = data[iterator];
		if (iterator == capacity - 1) {
			iterator = 0;
		} else {
			iterator++;
		}
		iteratorIndex++;
		return tmp;
	}

	bool iteratorHasNext() {
		return iteratorIndex < length;
	}




protected:
	unsigned char cursor;
	unsigned char length;

	short iterator;
	unsigned char iteratorIndex;
	Data data[capacity];
};

template <uint8_t capacity>
class NumericQueue: public BasicQueue<capacity, unsigned long> {
public:
	NumericQueue() : BasicQueue<capacity, unsigned long>(), sum(0), average(0) {

	}

	void push(unsigned long data) {
		sum += data;
		if (this->size() == capacity) {
			sum -= BasicQueue<capacity, unsigned long>::push(data);
		} else {
			BasicQueue<capacity, unsigned long>::push(data);
		}
		average = sum / BasicQueue<capacity, unsigned long>::size();
	}

	void clear() {
		BasicQueue<capacity, unsigned long>::clear();
		average = 0;
		sum = 0;
	}

	unsigned long getSum() {
		return this->sum;
	}

	unsigned long getAverage() {
		return this->sum;
	}

protected:
	unsigned long sum;
	unsigned long average;
};

template <uint8_t capacity>
class FrequencyQueue: public BasicQueue<capacity, uint16_t> {
public:
	FrequencyQueue(unsigned long period) : BasicQueue<capacity, uint16_t>(),
			period(period), segment(0), sum(0), segmentDuration(period / capacity) {
	}

	void update(unsigned long time) {
		if (time - segment >= segmentDuration) {
			if (this->size() == capacity) {
				sum -= this->push(0);
			} else {
				this->push(0);
			}
		}
	}

	void event() {
		this->last()++;
		this->sum++;
	}

	unsigned long getSum() {
		return this->sum;
	}

protected:
	unsigned long period;
	unsigned long segment;
	unsigned long sum;

private:
	unsigned long segmentDuration;
};

#endif /* BASICQUEUE_H_ */
