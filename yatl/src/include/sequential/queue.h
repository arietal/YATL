/*
 * queue.h
 *
 *  Created on: Jul 8, 2012
 *      Author: arietal
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef NULL
#define NULL ((void*)0)

namespace yatl {

template <typename KeyType>
class queue {
	struct Node {
		KeyType data;
		Node *next;
		Node(KeyType data) : data(data), next(NULL) {}
	};
	Node *head, *tail;
public:
	queue() : head(NULL), tail(NULL) {	}

	void enqueue(KeyType data) {
		if (head == NULL) {
			head = tail = new Node(data);
		} else {
			Node *t = new Node(data);
			tail->next = t;
			tail = t;
		}
	}
	KeyType dequeue() {
		if (head != NULL) {
			Node *t = head;
			head = head->next;
			if (head == NULL)
				tail = NULL;
			KeyType r =  t->data;
			delete t;
			return r;
		}
		return (KeyType)0;
	}

	bool isEmpty() const { return head == NULL; }

	KeyType peek() {
		if (head != NULL) {
			return head->data;
		}
		return (KeyType)0;
	}
};

} // namespace

#endif /* QUEUE_H_ */
