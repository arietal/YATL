/*
 * tree_test_harness.h
 *
 *  Created on: Jul 6, 2012
 *      Author: arietal
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
