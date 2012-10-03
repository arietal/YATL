/*
 * queue.h
 *
 *  Created on: Jul 8, 2012
 *      Author: arietal
 *
 *  Copyright (c) 2012 Arie Tal. All rights reserved.
 *
 *  This file is part of Yet Another Template Library (YATL).
 *
 *  YATL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  YATL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with YATL.  If not, see <http://www.gnu.org/licenses/>.
 *
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
