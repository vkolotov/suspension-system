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
	BasicQueue() : cursor(-1), length(0), iterator(0), iteratorIndex(0) {

	}
	~BasicQueue() {}

	Data push(Data entry) {
		if (cursor >= capacity - 1) {
			cursor = 0;
		} else {
			cursor++;
		}
		if (length < capacity) {
			length++;
		}
		Data replacingValue = data[cursor];
		data[cursor] = entry;
		return replacingValue;
	}

	virtual void clear() {
		cursor = -1;
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
	int16_t cursor;
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
		return this->average;
	}

protected:
	unsigned long sum;
	unsigned long average;
};

template <uint8_t capacity>
class FrequencyQueue: public BasicQueue<capacity, uint16_t> {
public:
	FrequencyQueue(unsigned long period) : BasicQueue<capacity, uint16_t>(),
			period(period), segmentTail(0), sum(0), segmentDuration(period / capacity) {
	}

	void start(unsigned long time) {
		this->clear();
		segmentTail = time;
	}

	void update(unsigned long time) {

		uint8_t i = 0;
		while (time - segmentTail >= segmentDuration) {
			segmentTail += segmentDuration;
			shift();
			i++;
			if (i >= capacity) {
				break;
			}
		}

	}

	void shift() {
		if (this->size() == capacity) {
			uint16_t prev = BasicQueue<capacity, uint16_t>::push(0);
			sum -= prev;
		} else {
			BasicQueue<capacity, uint16_t>::push(0);
		}
	}

	void event() {
		this->last()++;
		this->sum++;
	}

	void clear() {
		BasicQueue<capacity, uint16_t>::clear();
		sum = 0;
	}


	unsigned long getSum() {
		return this->sum;
	}

	unsigned long calculateSum() {
		unsigned long sum = 0;
		this->iteratorReset();
		while (this->iteratorHasNext()) {
			sum += this->iteratorNext();
		}
		return sum;
	}

	unsigned long getSegmentDuration() {
		return segmentDuration;
	}

	unsigned long getSegmentTail() {
		return segmentTail;
	}

protected:
	unsigned long period;
	unsigned long segmentTail;
	unsigned long sum;

private:
	unsigned long segmentDuration;
};

#endif /* BASICQUEUE_H_ */
