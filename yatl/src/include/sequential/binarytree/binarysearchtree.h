/*
 * binarysearchtree.h
 *
 *  Created on: Jun 22, 2012
 *      Author: arietal
 */

#ifndef BINARYSEARCHTREE_H_
#define BINARYSEARCHTREE_H_

#include "binarytree.h"

using namespace yatl;

namespace yatl {

template <typename KeyType, typename DataType, typename Extend=_EmptyExtend>
class BinarySearchTree : public BinaryTree<KeyType,DataType,Extend> {
	typedef BinaryTree<KeyType,DataType,Extend> SuperType;
public:
	typedef BinaryTreeNode<KeyType,DataType,Extend> NodeType;
	/**
	 * Insert a node into the binary tree
	 */
	void insertNode(NodeType *node) {
		BinaryTreeNode<KeyType,DataType,Extend> *targetNode = SuperType::getRoot(), *parentNode = NULL;

		while (targetNode != NULL) {
			parentNode = targetNode;
			if (node->getKey() < targetNode->getKey())
				targetNode = targetNode->get(dir_left_child);
			else
				targetNode = targetNode->get(dir_right_child);
		}
		if (parentNode == NULL) {
            SuperType::setRoot(node);
		} else {
			if (node->getKey() < parentNode->getKey()) {
				parentNode->set(node, dir_left_child);
			} else {
				parentNode->set(node, dir_right_child);
			}
			node->set(parentNode, dir_parent);
		}
	}

	/**
	 * Delete a node from the binary tree
	 */
	void deleteNode(NodeType *node) {
		if (node->get(dir_left_child) == NULL)
			SuperType::transplant(node, node->get(dir_right_child));
		else if (node->get(dir_right_child) == NULL)
			SuperType::transplant(node, node->get(dir_left_child));
		else {
			NodeType *succ = SuperType::getSuccessor(node);
			if (succ->get(dir_parent) != node) {
                SuperType::transplant(succ, succ->get(dir_right_child));
				succ->set(node->get(dir_right_child), dir_right_child);
				succ->get(dir_right_child)->set(succ, dir_parent);
			}
            SuperType::transplant(node,succ);
			succ->set(node->get(dir_left_child), dir_left_child);
			succ->get(dir_left_child)->set(succ, dir_parent);
		}
	}

};

} // namespace

#endif /* BINARYSEARCHTREE_H_ */
