/*
 * redblacktree.h
 *
 *  Created on: Jun 26, 2012
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

#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

#include "binarysearchtree.h"

using namespace yatl;

namespace yatl {

enum RedBlackColor {
	color_red,
	color_black
};

struct _EmptyAugment {
	void printAugment(ostream &os) { }
	bool update(void *left, void *right) { return false; }
};

template <typename Augment=_EmptyAugment>
class _RedBlackExtend : public Augment {
	RedBlackColor _color;
public:
   _RedBlackExtend() : _color(color_red) {}
   void printExtend(ostream &os) {
	   os << ((_color == color_red) ? ", R" : ", B");
	   Augment::printAugment(os);
   }

   RedBlackColor getColor() {
	   return _color;
   }
   void setColor(RedBlackColor color) {
	   _color = color;
   }

   bool update(void *left, void *right) {
	   return Augment::update(left,right);
   }
};


template <typename KeyType, typename DataType, typename Augment=_EmptyAugment>
class RedBlackTree : public BinarySearchTree<KeyType,DataType,_RedBlackExtend<Augment> > {
	typedef BinarySearchTree<KeyType,DataType,_RedBlackExtend<Augment> > SuperType;
public:
	typedef BinaryTreeNode<KeyType,DataType,_RedBlackExtend<Augment> > NodeType;

	void insertNode(NodeType *node) {
		SuperType::insertNode(node);
		node->setColor(color_red);
		redBlack_insertFixup(node);
	}

	/**
	 * Delete a node from the binary tree
	 */
	void deleteNode(NodeType *node) {
		NodeType *y = node, *x, *xp;
		RedBlackColor y_original_color = y->getColor();
		if (node->get(dir_left_child) == NULL) {
			x = node->get(dir_right_child);
			xp = node->get(dir_parent);
            SuperType::transplant(node, node->get(dir_right_child));
		} else if (node->get(dir_right_child) == NULL) {
			x = node->get(dir_left_child);
			xp = node->get(dir_parent);
            SuperType::transplant(node, node->get(dir_left_child));
		} else {
			y = SuperType::getSuccessor(node);
			y_original_color = y->getColor();
			x = y->get(dir_right_child);
			if (y->get(dir_parent) == node) {
				if (x != NULL)
					x->set(y, dir_parent);
				xp = y;
			} else {
                SuperType::transplant(y, y->get(dir_right_child));
				y->set(node->get(dir_right_child), dir_right_child);
				y->get(dir_right_child)->set(y, dir_parent);
				xp = y->get(dir_parent);
			}
            SuperType::transplant(node,y);
			y->set(node->get(dir_left_child), dir_left_child);
			y->get(dir_left_child)->set(y, dir_parent);
			y->setColor(node->getColor());
		}
		if (y_original_color == color_black) {
			redBlack_deleteFixup(x, xp);
		} else {
			updateAugmentedNodesToRoot(xp);
		}
	}

protected:
	bool updateAugmentedNode(NodeType *node) {
		if (node != NULL)
			return node->update(node->get(dir_left_child), node->get(dir_right_child));
		return false;
	}

	void updateAugmentedNodesToRoot(NodeType *node) {
		while (node != NULL) {
			updateAugmentedNode(node);
			node = node->get(dir_parent);
		}
	}
	void rotate(NodeType *node, BinaryTreeDirection dir) {
		BinaryTreeDirection otherDir = (dir == dir_left_child) ? dir_right_child : dir_left_child;
		NodeType *other = node->get(otherDir);
		SuperType::rotate(node, dir);
		updateAugmentedNode(node);
		updateAugmentedNode(other);
	}

	void redBlack_insertFixup(NodeType *node) {
		NodeType *parent;
		while ((parent = node->get(dir_parent)) != NULL && parent->getColor() == color_red) {
			BinaryTreeDirection parentDir = parent->getChildDirection();
			BinaryTreeDirection otherParentDir = (parentDir == dir_left_child) ? dir_right_child : dir_left_child;
			NodeType *uncle = parent->get(dir_parent)->get(otherParentDir);
			if (uncle != NULL && uncle->getColor() == color_red) {
				parent->setColor(color_black);
				uncle->setColor(color_black);
				parent->get(dir_parent)->setColor(color_red);
				node = parent->get(dir_parent);
			} else {
				if (node == parent->get(otherParentDir)) {
					node = parent;
					rotate(node, parentDir);
				}
				parent = node->get(dir_parent);
				if (parent != NULL) {
					parent->setColor(color_black);
					if (parent->get(dir_parent) != NULL) {
						parent->get(dir_parent)->setColor(color_red);
						rotate(parent->get(dir_parent), otherParentDir);
					}
				}
			}
		}
		updateAugmentedNodesToRoot(parent);
		SuperType::getRoot()->setColor(color_black);
	}

	void redBlack_deleteFixup(NodeType *node, NodeType *xp) {
		while (node != SuperType::getRoot() && (node == NULL || node->getColor() == color_black)) {
			BinaryTreeDirection nodeDir = (node == xp->get(dir_left_child)) ? dir_left_child : dir_right_child;
			BinaryTreeDirection nodeOtherDir = (nodeDir == dir_left_child) ? dir_right_child : dir_left_child;
			NodeType *w = xp->get(nodeOtherDir);
			if (w->getColor() == color_red) {
				w->setColor(color_black);
				xp->setColor(color_red);
				rotate(xp, nodeDir);
				w = xp->get(nodeOtherDir);
			}
			if ((w->get(nodeDir) == NULL || w->get(nodeDir)->getColor() == color_black) &&
					(w->get(nodeOtherDir) == NULL || w->get(nodeOtherDir)->getColor() == color_black)) {
				w->setColor(color_red);
				node = xp;
				xp = xp->get(dir_parent);
			} else {
				if (w->get(nodeOtherDir)->getColor() == color_black) {
					w->get(nodeDir)->setColor(color_black);
					w->setColor(color_red);
					rotate(w, nodeOtherDir);
					w = xp->get(nodeOtherDir);
				}
				w->setColor(xp->getColor());
				xp->setColor(color_black);
				w->get(nodeOtherDir)->setColor(color_black);
				rotate(xp, nodeDir);
				node = SuperType::getRoot(); xp = NULL;
			}
		}
		node->setColor(color_black);
		updateAugmentedNodesToRoot(node);
	}
};

}

#endif /* REDBLACKTREE_H_ */
