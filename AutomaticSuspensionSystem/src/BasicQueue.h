/*
 * BasicQueue.h
 *
 *  Created on: Aug 10, 2012
 *      Author: vkolotov
 */

#ifndef BASICQUEUE_H_
#define BASICQUEUE_H_

template <unsigned int capacity, typename Data> class BasicQueue {
public:
	BasicQueue() : cursor(0), length(0), iterator(0), iteratorIndex(0) {

	}
	~BasicQueue() {}

	void push(Data entry) {
		if (cursor >= capacity - 1) {
			cursor = 0;
		} else {
			cursor++;
		}
		if (length < capacity) {
			length++;
		}
		data[cursor] = entry;
	}

	void clear() {
		cursor = 0;
		length = 0;
	}

	Data last() {
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

	Data iteratorNext() {
		Data tmp = data[iterator];
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

#endif /* BASICQUEUE_H_ */
