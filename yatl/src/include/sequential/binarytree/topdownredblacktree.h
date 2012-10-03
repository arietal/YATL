/*
 * topdownredblacktree.h
 *
 * Implement Program 7
 *
 *  Created on: Jul 18, 2012
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

#ifndef TOPDOWNREDBLACKTREE_H_
#define TOPDOWNREDBLACKTREE_H_

#include "redblacktree.h"

using namespace yatl;

namespace yatl {

template <typename KeyType, typename DataType, typename Augment=_EmptyAugment>
class TopDownRedBlackTree : public RedBlackTree<KeyType,DataType,Augment> {
	typedef RedBlackTree<KeyType,DataType,Augment> SuperType;
	typename SuperType::NodeType *h, *y[4], *z[2];
public:
	typedef typename SuperType::NodeType NodeType;
	TopDownRedBlackTree() {
//		for (int j=0; j < 2; j++) {
//			for (int i=0; i < 2; i++) {
////				y[j*2+i] = new NodeType(-2,j*2+i); y[j*2+i]->setColor(color_red);
//				y[j*2+i] = NULL;
//			}
//			z[j] = NULL; // new NodeType(-1,j,y[j*2],y[j*2+1]); z[j]->setColor(color_black);
//		}
//		h = new NodeType((KeyType)-1000000,0,z[0],z[1]); h->setColor(color_black);
//		SuperType::setRoot(h);
	}

	~TopDownRedBlackTree() {
//		delete h; //delete z[0]; delete z[1];
//		for (int i=0; i < 4; i++) delete y;
	}

	bool checkChildColor(NodeType *node, BinaryTreeDirection dir, RedBlackColor color) {
//		return node->get(dir) == NULL || node->get(dir)->getColor() == color;
		return node->get(dir) != NULL && node->get(dir)->getColor() == color;
	}
	void insertNode(NodeType *node) {
		NodeType *x, *g = NULL, *f = NULL; // bool success;
		KeyType v = node->getKey();
		x = SuperType::getRoot(); // z[0]->setKey(v); z[1]->setKey(v); success = true;
		if (x == NULL) {
			node->setColor(color_black);
			SuperType::setRoot(node);
			return;
		}
		bool checkRoot = true;
		while (x->getKey() != v) {
			if (!checkRoot) {
				g = f; f = x;
				if (v < x->getKey()) {
					x = x->get(dir_left_child);
				} else {
					x = x->get(dir_right_child);
				}
			} else {
				checkRoot = false;
			}
			if (x == NULL || (checkChildColor(x, dir_left_child, color_red) && checkChildColor(x, dir_right_child, color_red)) ||
					(x->getColor() == color_red && (checkChildColor(x, dir_left_child, color_red) || checkChildColor(x, dir_right_child, color_red))))  {
				if (x == z[0] || x == z[1]) {
					x = node;
					x->set(z[0], dir_left_child); x->set(z[1], dir_right_child); x->setColor(color_black);
					//success = false;
				}
				if (x->getColor() == color_black) {
					if (x->get(dir_left_child) != NULL)
						x->get(dir_left_child)->setColor(color_black);
					if (x->get(dir_right_child) != NULL)
						x->get(dir_right_child)->setColor(color_black);
					x->setColor(color_red);
				} else {
					x->setColor(color_black);
					f->setColor(color_red);
				}
				if (x->getColor() == color_black ||
						(f != NULL && f->getColor() == color_red && (g != NULL && ((v < g->getKey()) != (v < f->getKey()))) )) {
					if (v < f->getKey()) {
						f->set(x->get(dir_right_child), dir_left_child);
						if (x->get(dir_right_child) != NULL)
							x->get(dir_right_child)->set(f, dir_parent);
						x->set(f, dir_right_child);
						f->set(x, dir_parent);
						f = g;
					} else {
						f->set(x->get(dir_left_child), dir_right_child);
						if (x->get(dir_left_child) != NULL)
							x->get(dir_left_child)->set(f, dir_parent);
						x->set(f, dir_left_child);
						f->set(x, dir_parent);
						f = g;
					}
				}
				if (f != NULL) {
					if (v < f->getKey()) {
						f->set(x, dir_left_child);
					} else {
						f->set(x, dir_right_child);
					}
					x->set(f, dir_parent);
				} else {
					x->set(NULL, dir_parent);
					SuperType::setRoot(x);
				}
			}
		}
		SuperType::getRoot()->setColor(color_black);
//		h->get(dir_right_child)->setColor(color_black);
	}


	void deleteNode(NodeType *node, NodeType *h) {
		NodeType *x, *g = NULL, *f = NULL, *t, *b, *bb;
		x = h;
		//z->setKey(MININF);
		t = NULL;
		KeyType v = node->getKey();
		if (h->get(dir_right_child)->get(dir_left_child)->getColor() == color_black &&
				h->get(dir_right_child)->get(dir_right_child)->getColor() == color_black)
			h->get(dir_right_child)->setColor(color_red);
		while (x != NULL) {
			g = f;
			f = x;
			if (v < x->getKey()) {
				x = x->get(dir_left_child);
				b = x->get(dir_right_child);
				bb = b->get(dir_right_child);
			} else {
				x = x->get(dir_right_child);
				b = x->get(dir_left_child);
				bb = b->get(dir_left_child);
			}
			if (v == x->getKey()) {
				t = x;
			}
			if (x->getColor() == color_black && f->getColor() == color_black && b->getColor() == color_red)
				balance(v, g,f,b,bb);
			if (x->getColor() == color_black && x->get(dir_left_child)->getColor() == color_black && x->get(dir_right_child)->getColor() == color_black) {
				x->setColor(color_red);
				f->setColor(color_black);
				if (b->getColor() == color_black &&  b->get(dir_left_child)->getColor() == color_black && b->get(dir_right_child)->getColor() == color_black)
					b->setColor(color_red);
				else if (b->get(dir_left_child)->getColor() == color_red)
					balance(v, g, f, b, b->get(dir_left_child));
				else if (b->get(dir_right_child)->getColor() == color_red)
					balance(v, g, f, b, b->get(dir_right_child));
			}
		}
		h->get(dir_right_child)->setColor(color_black);
		// z->setColor(color_black);
		if (t == NULL) {
			if (v < g->getKey())
				g->set(z, dir_left_child);
			else
				g->set(z, dir_right_child);
			t->setKey(f->getKey());
		}
		// return t;
	}

	void balance(KeyType v, NodeType *gg, NodeType *g, NodeType *f, NodeType *x) {
		if (g == NULL || (v < g->getKey()) != v < (f->getKey())) {
			if (v < f->getKey()) {
				f->set(x->get(dir_right_child), dir_left_child);
				x->set(f, dir_right_child);
			} else {
				f->set(x->get(dir_left_child), dir_right_child);
				x->set(f, dir_left_child);
			}
			f = x;
			if (v < g->getKey()) {
				g->set(f->get(dir_right_child), dir_left_child);
				f->set(g, dir_right_child);
			} else {
				g->set(f->get(dir_left_child), dir_right_child);
				f->set(g, dir_left_child);
			}
			RedBlackColor tmp = g->getColor();
			g->setColor(f->getColor());
			f->setColor(tmp);
			g = f;
			if (v < gg->getKey()) {
				gg->set(g, dir_left_child);
			} else {
				gg->set(g, dir_right_child);
			}
		}
	}

	void printTree1(ostream &os) {
		printTree1(os, SuperType::getRoot(), 0);
		os << endl;
	}

	void printTree1(ostream &os, NodeType *node, int depth) {
		for (int i=0; i < depth; i++) os << "  ";
		os << "(";
		if (node == h)
			os << "h";
		else if (node == z[0])
			os << "z[0]";
		else if (node == z[1])
			os << "z[1]";
		else if (node == y[0])
			os << "y[0]";
		else if (node == y[1])
			os << "y[1]";
		else if (node == y[2])
			os << "y[2]";
		else if (node == y[3])
			os << "y[3]";
		else
			os << node;

		os << " ";
		if (node != NULL) {
			if (node->get(dir_left_child)) {
				os << endl;
				printTree(os, node->get(dir_left_child), depth+1);
			} else
				os << "NIL ";
			if (node->get(dir_right_child)) {
				os << endl;
				printTree(os, node->get(dir_right_child), depth+1);
			} else
				os << "NIL";
		}
		os << ")";
	}
};

} // namespace

#endif /* TOPDOWNREDBLACKTREE_H_ */
