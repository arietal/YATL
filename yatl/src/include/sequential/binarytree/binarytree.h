/*
 * tree.h
 *
 *  Created on: Jun 22, 2012
 *      Author: arietal
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <ostream>

namespace yatl {

struct _EmptyExtend {
   void printExtend(ostream &os) { }
};


enum BinaryTreeDirection {
	dir_left_child,
	dir_right_child,
	dir_parent
};

template <typename KeyType, typename DataType, typename Extend=_EmptyExtend>
class BinaryTreeNode : public Extend {
	BinaryTreeNode<KeyType,DataType,Extend> *_left, *_right, *_parent;
	KeyType _key;
	DataType _data;
public:
	/**
	 * Constructor. Initiate a Tree Node with a key value and data
	 */
	BinaryTreeNode<KeyType,DataType,Extend>(KeyType key, DataType data) : _left(NULL), _right(NULL), _parent(NULL),
	       _key(key), _data(data) {}

	/**
	 * Constructor. Initiate a Tree Node with a key value and data, as well as child references
	 */
	BinaryTreeNode<KeyType,DataType,Extend>(KeyType key, DataType data, BinaryTreeNode<KeyType,DataType,Extend>* left,
			BinaryTreeNode<KeyType,DataType,Extend>* right) : _left(left), _right(right), _parent(NULL), _key(key), _data(data) {
		if (_left != NULL)
			_left->set(this, dir_parent);
		if (_right != NULL)
			_right->set(this, dir_parent);
	}

	/**
	 * Get a node (left or right) or parent
	 */
	BinaryTreeNode<KeyType,DataType,Extend> *get(BinaryTreeDirection dir) {
		switch (dir) {
		case dir_left_child:
			return _left;
		case dir_right_child:
			return _right;
		case dir_parent:
			return _parent;
		}
		return NULL;
	}

	/**
	 * Set a child node of a tree node (left or right)
	 */
	void set(BinaryTreeNode<KeyType,DataType,Extend> *node, BinaryTreeDirection dir) {
		switch (dir) {
		case dir_left_child:
			_left = node;
			break;
		case dir_right_child:
			_right = node;
			break;
		case dir_parent:
			_parent = node;
			break;
		}
	}

	/**
	 * Return the direction of this node from its parent (assumes the node has a parent)
	 */
	BinaryTreeDirection getChildDirection() {
		if (this == _parent->_left)
			return dir_left_child;
		return dir_right_child;
	}

	/**
	 * Get the left child of a tree node
	 */
	BinaryTreeNode<KeyType,DataType,Extend> *getLeft() {
		return _left;
	}


	/**
	 * Set the left child of a tree node
	 */
	void setLeft(BinaryTreeNode<KeyType,DataType,Extend> *left) {
		_left = left;
	}

	/**
	 * Get the right child of a tree node
	 */
	BinaryTreeNode<KeyType,DataType,Extend> *getRight() {
		return _right;
	}

	/**
	 * Set the right child of a tree node
	 */
	void setRight(BinaryTreeNode<KeyType,DataType,Extend> *right) {
		_right = right;
	}

	/**
	 * Get the parent of a tree node
	 */
	BinaryTreeNode<KeyType,DataType,Extend> *getParent() {
		return _parent;
	}

	/**
	 * Set the parent of a tree node
	 */
	void setParent(BinaryTreeNode<KeyType,DataType,Extend> *parent) {
		_parent = parent;
	}

	/**
	 * Get the key of a tree node
	 */
	KeyType getKey() {
		return _key;
	}

	/**
	 * Set the key of a tree node
	 */
	void setKey(KeyType key) {
		_key = key;
	}

	/**
	 * Get the data of a tree node
	 */
	DataType getData() {
		return _data;
	}

	/**
	 * Set the data of a tree node
	 */
	void setData(DataType data) {
		_data = data;
	}
};


template <typename KeyType, typename DataType, typename Extend>
ostream& operator<<(ostream& os, BinaryTreeNode<KeyType,DataType,Extend> *node) {
	if (node == NULL)
		return os << "NIL";
	os << "(" << node->getKey() << ", " << node->getData();
	node->printExtend(os);
	return os << ")";
}

template <typename KeyType, typename DataType, typename Extend=_EmptyExtend>
class BinaryTree {
	BinaryTreeNode<KeyType,DataType,Extend> *_root;
public:
	typedef BinaryTreeNode<KeyType,DataType,Extend> NodeType;
	/**
	 * Constructor. Create an empty binary tree
	 */
	BinaryTree<KeyType,DataType,Extend>() : _root(NULL) {}

	// define the interface of a search tree

	/**
	 * Insert a node into the binary tree
	 */
	void insertNode(NodeType *node);

	/**
	 * Delete a node from the binary tree
	 */
	void deleteNode(NodeType *node);

	/**
	 * Search for a node in the tree based on a key
	 * @return the node that has the key or NULL if no such node exists in the tree
	 */
	BinaryTreeNode<KeyType,DataType,Extend> *searchNode(KeyType key) {
		// implement a non-recursive search
		NodeType *targetNode;
		targetNode = _root;

		while (targetNode != NULL && targetNode->getKey() != key) {
			if (key < targetNode->getKey())
				targetNode = targetNode->get(dir_left_child);
			else
				targetNode = targetNode->get(dir_right_child);
		}
		return targetNode;
	}

	/**
	 * Search for the node with the smallest key in the sub tree starting at the parameter node
	 * @return the node with the smallest key or NULL if the tree is empty
	 */
	NodeType *getMinNode(NodeType *node) {
		NodeType *minNode = node;
		if (minNode == NULL)
			return NULL;
		while (minNode->get(dir_left_child) != NULL) {
			minNode = minNode->get(dir_left_child);
		}
		return minNode;
	}

	/**
	 * Search for the node with the largest key in the sub tree starting at the parameter node
	 * @return the node with the largest key or NULL if the tree is empty
	 */
	NodeType *getMaxNode(NodeType *node) {
		NodeType *maxNode = node;
		if (maxNode == NULL)
			return NULL;
		while (maxNode->get(dir_right_child) != NULL) {
			maxNode = maxNode->get(dir_right_child);
		}
		return maxNode;
	}

	/**
	 * Search for the successor node in the sorted order
	 * @return the node with the succeeding key, or NULL if the parameter node is already the max node
	 */
	NodeType *getSuccessor(NodeType *node) {
		iterator it(*this,node);
		return it.setToNext();
	}

	/**
	 * Search for the predecessor node in the sorted order
	 * @return the node with the preceding key value, or NULL if the parameter node is already the min node
	 */
	NodeType *getPredecessor(NodeType *node) {
		iterator it(*this,node);
		return it.setToPrevious();
	}


	/**
	 * Print the content of the tree in the sorted order from the smallest key to the largest key
	 */
	void printInOrder(ostream &os) {
		iterator it(*this);
		for (it.setToFirst(); *it; it.setToNext())
			os << *it << " ";
		os << endl;
	}

	/**
	 * Print the content of the tree in the reversed sorted order, from the largest key to the smallest key
	 */
	void printReversed(ostream &os) {
		iterator it(*this);
		for (it.setToLast(); *it; it.setToPrevious())
			os << *it << " ";
		os << endl;
	}


	/**
	 * An iterator class for a binary tree.
	 *
	 * This iterator class supports initializing at any node in the tree, moving backward and forward in the tree.
	 * The iterator uses O(1) additional memory to hold the state of the iterator.
	 */
	class iterator {
		BinaryTree<KeyType,DataType,Extend>& _bst;
		BinaryTreeDirection _direction;
		int _height;
		NodeType *_p;
		enum { move_left, move_right, none } _movement;
	public:
		iterator(BinaryTree<KeyType,DataType,Extend>& bst, NodeType *start=NULL) : _bst(bst),
		                     _direction(dir_parent), _height(0), _p(start), _movement(none) {}

		NodeType *setToFirst() {
			_p = _bst._root;
			_height=0;
			_direction=dir_parent;
			_movement = move_right;
			while (_p->get(dir_left_child) != NULL) {
				_height++;
				_p = _p->get(dir_left_child);
			}
			return _p;
		}
		NodeType *setToLast() {
			_p = _bst._root;
			_height=0;
			_direction=dir_parent;
			_movement = move_left;
			while (_p->get(dir_right_child) != NULL) {
				_height++;
				_p = _p->get(dir_right_child);
			}
			return _p;
		}
		NodeType * operator*() {
			return _p;
		}
		NodeType *setToNext() {
			if (_movement != move_right) {
				_movement = move_right;
				if (_p != NULL)
					_direction = dir_left_child;
			}
			NodeType *p = _p;
			if (_p == NULL)
				return NULL;
			while (_p != NULL) {
				switch (_direction) {
				case dir_parent:
					if (_p->get(dir_left_child) != NULL) {
						_p = _p->get(dir_left_child);
						_height++;
						continue;
					}
				case dir_left_child:
					if (p != _p)
						return _p;
					if (_p->get(dir_right_child) != NULL) {
						_p = _p->get(dir_right_child);
						_direction = dir_parent;
						_height++;
						continue;
					}
				case dir_right_child:
					if (_p->get(dir_parent) != NULL) {
						if (_p == _p->get(dir_parent)->get(dir_left_child))
							_direction = dir_left_child;
						else
							_direction = dir_right_child;
					}
					_p = _p->get(dir_parent);
					_height--;
					break;
				}
			}
			return _p;
		}
		NodeType *setToPrevious() {
			if (_movement != move_left) {
				_movement = move_left;
				if (_p != NULL)
					_direction = dir_right_child;
			}

			NodeType *p = _p;
			if (_p == NULL)
				return NULL;
			while (_p != NULL) {
				switch (_direction) {
				case dir_parent:
					if (_p->get(dir_right_child) != NULL) {
						_p = _p->get(dir_right_child);
						_height++;
						continue;
					}
				case dir_right_child:
					if (p != _p)
						return _p;
					if (_p->get(dir_left_child) != NULL) {
						_p = _p->get(dir_left_child);
						_direction = dir_parent;
						_height++;
						continue;
					}
				case dir_left_child:
					if (_p->get(dir_parent) != NULL) {
						if (_p == _p->get(dir_parent)->get(dir_left_child))
							_direction = dir_left_child;
						else
							_direction = dir_right_child;
					}
					_p = _p->get(dir_parent);
					_height--;
					break;
				}
			}
			return _p;
		}
		int getHeight() { return _height; }
	};

	/**
	 * Pretty print the content of the tree.
	 */
	void printTree(ostream &os) {
		NodeType *node = getRoot();
		int height=0;
		enum { left, right, top } direction = top;
		while (node != NULL) {
			switch (direction) {
			case top:
				os << endl;
				for (int i=0; i < height; i++) os << "  ";
				os << "(" << node << " ";
				if (node->get(dir_left_child) != NULL) {
					node = node->get(dir_left_child);
					height++;
					continue;
				} else  {
					os << "NIL ";
				}
			case left:
				if (node->get(dir_right_child) != NULL) {
					node = node->get(dir_right_child);
					direction = top;
					height++;
					continue;
				} else {
					os << "NIL";
				}
			case right:
				if (node->get(dir_parent) != NULL) {
					if (node == node->get(dir_parent)->get(dir_left_child))
						direction = left;
					else
						direction = right;
				}
				node = node->get(dir_parent);
				height--;
				os << ")";
				break;
			}
		}
		os << endl;
	}

	/**
	 * Calculate the height of the tree.
	 */
	int getHeight() {
		int maxHeight=0;
		iterator it(*this);
		for (it.setToFirst(); *it; it.setToNext())
			if (it.getHeight() > maxHeight)
				maxHeight = it.getHeight();
		return maxHeight;
	}
	/**
	 * Get the root of the tree
	 */
	NodeType *getRoot() {
		return _root;
	}
protected:
	/**
	 * Set the root node of the tree
	 */
	void setRoot(NodeType *node) {
		_root = node;
	}

	/**
	 * transplant - replaces one sub tree with another
	 */
	void transplant(NodeType *u, NodeType *v) {
		if (u->get(dir_parent) == NULL) {
			setRoot(v);
		} else if (u == u->get(dir_parent)->get(dir_left_child)) {
			u->get(dir_parent)->set(v, dir_left_child);
		} else {
			u->get(dir_parent)->set(v, dir_right_child);
		}
		if (v != NULL)
			v->set(u->get(dir_parent), dir_parent);
	}

	void rotate(NodeType *node, BinaryTreeDirection dir) {
		BinaryTreeDirection otherDir = (dir == dir_left_child) ? dir_right_child : dir_left_child;
		NodeType *other = node->get(otherDir);
		node->set(other->get(dir), otherDir);
		if (other->get(dir) != NULL) {
			other->get(dir)->set(node, dir_parent);
		}
		other->set(node->get(dir_parent), dir_parent);
		if (node->get(dir_parent) == NULL) {
			_root = other;
		} else {
			BinaryTreeDirection childDirection = node->getChildDirection();
			node->get(dir_parent)->set(other, childDirection);
		}
		other->set(node, dir);
		node->set(other, dir_parent);
	}

};

} // namespace

#endif /* BINARYTREE_H_ */
