/*
 * BasicQueue.h
 *
 *  Created on: Aug 10, 2012
 *      Author: vkolotov
 */

#ifndef BASICQUEUE_H_
#define BASICQUEUE_H_

template <typename Data> class BasicQueue {
public:
	BasicQueue(unsigned char capacity) : cursor(0), length(0), capacity(capacity), data(capacity), iterator(0) {
	}

	void push(Data entry) {
		if (cursor >= capacity - 1) {
			cursor = 0;
		} else {
			cursor++;
		}
		if (length < 5) {
			length++;
		}
		data[cursor] = entry;
	}

	void clear() {
		cursor = 0;
		length = 0;
	}

	Data current() {
		return data[cursor];
	}

	void iteratorReset() {
		iterator = 0;
	}

	Data iteratorNext() {
		return data[iterator++];
	}

	bool iteratorHasNext() {
		return iterator < length;
	}

	virtual ~BasicQueue();

protected:
	unsigned char cursor;
	unsigned char length;
	unsigned char capacity;
	Data data[];

	unsigned char iterator;
};

#endif /* BASICQUEUE_H_ */
