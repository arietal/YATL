/*
 * stack.h
 *
 *  Created on: Jul 8, 2012
 *      Author: arietal
 */

#ifndef STACK_H_
#define STACK_H_

#ifndef NULL
#define NULL ((void*)0)
#endif

namespace yatl {

template <typename KeyType, int seglen=32>
class Stack {
	struct Node {
		KeyType data[seglen];
		Node *next;
	};
	Node *head ;
	int count;
	int totalCount;
public:
	Stack() : head(NULL), count(0), totalCount(0) {
	}

	~Stack() {
		Node *tmp = head ;
		if (tmp != NULL) {
			while (tmp->next != NULL) {
				Node *t = tmp->next;
				delete tmp;
				tmp = t;
			}
			delete tmp;
		}
	}

	inline void push(KeyType data) {
		if (head == NULL || count == seglen) {
			Node *tmp = new Node();
			count=0;
			tmp->next = head;
			head = tmp;
		}
		head->data[count++] = data;
		totalCount++;
	}

	inline KeyType pop() {
		if (head != NULL && count == 0) {
			Node *tmp = head->next;
			delete head;
			head = tmp;
			count = seglen;
		}
		if (head != NULL) {
			totalCount--;
			return head->data[--count];
		}
		return (KeyType)0;
	}

	inline bool isEmpty() {
		return totalCount == 0;
	}

	inline int length() const {
		return totalCount;
	}
};

} // namespace

#endif /* STACK_H_ */
