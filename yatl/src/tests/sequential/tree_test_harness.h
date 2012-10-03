/*
 * tree_test_harness.h
 *
 *  Created on: Jul 6, 2012
 *      Author: arietal
 */

#ifndef TREE_TEST_HARNESS_H_
#define TREE_TEST_HARNESS_H_

#include "binarytree/binarysearchtree_test.h"
#include "binarytree/redblacktree_test.h"
#include "binarytree/intervaltree_test.h"
#include "binarytree/avltree_test.h"
#include "binarytree/topdownredblacktree_test.h"

class TreeTestHarness : public Harness {
public:
	const char *getName() const { return "Tree Test Harness"; }
	bool runTest() {
		bool rc = true;
		rc &= Tester<BinarySearchTreeTest>().run();
		rc &= Tester<RedBlackTreeTest>().run();
		rc &= Tester<IntervalTreeTest>().run();
		rc &= Tester<AVLTreeTest>().run();
//		rc &= Tester<TopDownRedBlackTreeTest>().run();
		return rc;
	}
};


#endif /* TREE_TEST_HARENESS_H_ */
