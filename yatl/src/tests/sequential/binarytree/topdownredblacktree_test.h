/*
 * topdownredblacktree_test.h
 *
 *  Created on: Jul 18, 2012
 *      Author: arietal
 */

#ifndef TOPDOWNREDBLACKTREE_TEST_H_
#define TOPDOWNREDBLACKTREE_TEST_H_


#include <iostream>
using namespace std;

#include "../../test.h"
#include "../../../include/sequential/binarytree/topdownredblacktree.h"
using namespace yatl;

class TopDownRedBlackTreeTest : public Test {
public:
	const char *getName() const { return "Top-Down Red-Black Tree"; }
	bool runTest() {
		TopDownRedBlackTree<int,int> bst;
		for (int i=0; i < 20*7; i += 7) {
			int k = i % 13;
			TopDownRedBlackTree<int,int>::NodeType *btn = new TopDownRedBlackTree<int,int>::NodeType(k,i);
			bst.insertNode(btn);
		}
		bst.printTree(cout);

		Assume(bst.getHeight() == 4, ("Height should be 4 not %d", bst.getHeight()));

		TopDownRedBlackTree<int,int>::NodeType *btn = bst.searchNode(6);

	    Assume(btn != NULL, ("Failed to find node 6"));

	    Assume(btn->getKey() == 6, ("Returned node key is %d instead of 6", btn->getKey()));

	    Assume(btn->getData() == 84, ("Returned node 6 data is %d instead of 84", btn->getData()));


	    TopDownRedBlackTree<int,int>::NodeType *succ = bst.getSuccessor(btn);

	    Assume(succ != NULL, ("Failed to find successor node to node 6"));

	    Assume(succ->getKey() == 7, ("Successor node to node 6 is key=%d instead of 7", succ->getKey()));

	    Assume(succ->getData() == 7, ("Successor node 7 data is %d instead of 7", succ->getData()));

	    TopDownRedBlackTree<int,int>::NodeType *pred = bst.getPredecessor(btn);

	    Assume(pred != NULL, ("Failed to find predecessor node to node 6"));

	    Assume(pred->getKey() == 5, ("Predecessor node to node 6 is key=%d instead of 5", pred->getKey()));

	    Assume(pred->getData() == 70, ("Predecessor node 5 data is %d instead of 70", pred->getData()));

//		bst.deleteNode(btn);
//
//		Assume(succ == bst.getSuccessor(pred), ("After deleting 6, successor to (5,70) is not (7,7)"));
//
//		Assume(pred == bst.getPredecessor(succ), ("After deleting 6, predecessor to (7,7) is not (5,70)"));
//
//	    Assume(bst.getHeight() == 4, ("Height should be 4 not %d", bst.getHeight()));
//
//		btn = bst.searchNode(2);
//
//	    Assume(btn != NULL, ("Failed to find node 2"));
//
//	    Assume(btn->getKey() == 2, ("Returned node key is %d instead of 2", btn->getKey()));
//
//	    Assume(btn->getData() == 28, ("Returned node 2 data is %d instead of 28", btn->getData()));
//
//	    // create an iterator starting from the found node (2,28) and run it back and forth a few times and test
//	    // where we end up
//	    TopDownRedBlackTree<int,int>::iterator it(bst,btn);
//
//		for (int j=0; j < 2; j++) {
//			for (int i=0; i < 3; i++) {
//				it.setToPrevious();
//			}
//
//			for (int i=0; i < 3; i++) {
//				it.setToNext();
//			}
//		}
//
//		Assume((*it)->getKey() == 2, ("Iterator at wrong place: key is %d instead of 2", (*it)->getKey()));
//
//		Assume((*it)->getData() == 28, ("Iterator at wrong place: found (2, %d) instead of (2, 28)", (*it)->getData()));
//
//		Assume((*it)->getColor() == color_black, ("(2,28) color is not black"));
//
//		bst.deleteNode(btn);
//
//	    Assume(bst.getHeight() == 4, ("Height should be 4 not %d", bst.getHeight()));
//
//	    btn = bst.searchNode(3);
//
//	    Assume(btn != NULL, ("Failed to find node 3"));
//
//	    Assume(btn->getKey() == 3, ("Returned node key is %d instead of 3", btn->getKey()));
//
//	    Assume(btn->getData() == 42, ("Returned node 3 data is %d instead of 42", btn->getData()));
//
//	    Assume(btn->getColor() == color_black, ("Node 3 color is not black"));
//
//	    bst.deleteNode(btn);
//
//	    Assume(bst.getHeight() == 4, ("Height should be 4 not %d", bst.getHeight()));

	    return true;
	}
};


#endif /* TOPDOWNREDBLACKTREE_TEST_H_ */
