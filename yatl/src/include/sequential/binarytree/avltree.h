/*
 * avltree.h
 *
 *  Created on: Jul 6, 2012
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

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <iostream>
using namespace std;

#include "binarysearchtree.h"
using namespace yatl;

namespace yatl {


class _AVLExtend {
	int _height;
public:
   _AVLExtend() : _height(0) {}
   void printExtend(ostream &os) {
	   os << ", h:" << _height;
   }

   int getHeight() { return _height; }

   bool update(void *left, void *right) {
	   _AVLExtend *l = (_AVLExtend*)left, *r = (_AVLExtend *)right;
	   int oldHeight = _height;
	   _height = 0;
	   if (l != NULL) _height = l->getHeight();
	   if (r != NULL && r->getHeight() > _height) _height = r->getHeight();
	   _height++;
	   return _height != oldHeight;
   }
};


template <typename KeyType, typename DataType>
class AVLTree : public BinarySearchTree<KeyType,DataType,_AVLExtend > {
	typedef BinarySearchTree<KeyType,DataType,_AVLExtend > SuperType;
public:
	typedef BinaryTreeNode<KeyType,DataType,_AVLExtend > NodeType;

	void insertNode(NodeType *node) {
		SuperType::insertNode(node);
		avlFixup(node);
	}

	void deleteNode(NodeType *node) {
		NodeType *parent = node->get(dir_parent);
		SuperType::deleteNode(node);
		updateNode(parent->get(dir_left_child));
		updateNode(parent->get(dir_right_child));
		avlFixup(parent);
	}
protected:
	enum Pattern { pat_LR, pat_LL, pat_RL, pat_RR, pat_Balanced };
	Pattern getPattern(NodeType *node) {
		int lh = 0, rh = 0;
		NodeType *l = node->get(dir_left_child), *r = node->get(dir_right_child);
		if (l != NULL) lh = l->getHeight();
		if (r != NULL) rh = r->getHeight();
		int diff = lh - rh;
		if (diff >= -1 && diff <= 1) return pat_Balanced;
		if (diff > 0) { // LL and LR cases
			NodeType *l1 = l->get(dir_left_child), *r1 = l->get(dir_right_child);
			int lh1 = 0, rh1 = 0;
			if (l1 != NULL) lh1 = l1->getHeight();
			if (r1 != NULL) rh1 = r1->getHeight();
			if (lh1 > rh1) return pat_LL;
			return pat_RL;
		} else { // RR and RL cases
			NodeType *l1 = r->get(dir_left_child), *r1 = r->get(dir_right_child);
			int lh1 = 0, rh1 = 0;
			if (l1 != NULL) lh1 = l1->getHeight();
			if (r1 != NULL) rh1 = r1->getHeight();
			if (lh1 > rh1) return pat_RL;
			return pat_RR;
		}
		return pat_Balanced; // unreachable
	}

	void updateNode(NodeType *node) {
		if (node != NULL)
			node->update(node->get(dir_left_child), node->get(dir_right_child));
	}

	void avlFixup(NodeType *node) {
		while (node != NULL) {
			Pattern pattern = getPattern(node);
			if (pattern == pat_LR) {
				SuperType::rotate(node->get(dir_left_child), dir_left_child);
				pattern = pat_LL;
			}
			if (pattern == pat_LL) {
				SuperType::rotate(node, dir_right_child);
			}
			if (pattern == pat_RL) {
				SuperType::rotate(node->get(dir_right_child), dir_right_child);
				pattern = pat_RR;
			}
			if (pattern == pat_RR) {
				SuperType::rotate(node, dir_left_child);
			}
			updateNode(node);
			node = node->get(dir_parent);
		}
	}
};

} // namespace

#endif /* AVLTREE_H_ */
