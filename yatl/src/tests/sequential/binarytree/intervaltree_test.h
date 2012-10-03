/*
 * intervaltree_test.h
 *
 *  Created on: Jul 5, 2012
 *      Author: arietal
 */

#ifndef INTERVALTREE_TEST_H_
#define INTERVALTREE_TEST_H_

#include "../../test.h"
#include "../../../include/sequential/binarytree/intervaltree.h"
using namespace yatl;

class IntervalTreeTest : public Test {
public:
	const char *getName() const { return "Interval Tree"; }
	bool runTest() {
		IntervalTree<float,int> it;
		float lows [] = {  5.0, 15.0, 25.0, 17.0, 26.0, 19.0, 16.0,  6.0,  0.0,  8.0 };
		float highs[] = {  8.0, 23.0, 30.0, 19.0, 26.0, 20.0, 21.0, 10.0,  3.0,  9.0 };
		int count = sizeof(lows)/sizeof(float);
		for (int i=0; i < count; i++) {
			it.intervalInsert(new IntervalTree<float,int>::IntervalType(Interval<float>(lows[i],highs[i]),1));
		}

		IntervalTree<float,int>::IntervalType *ivl = it.intervalSearch(Interval<float>(24,26));

		Assume(ivl != NULL, ("Expected to locate an overlapping range for (24,26)"));

		Interval<float> in = ivl->getInterval();
		Assume(in.low == 25 && in.high == 30, ("Expected the overlapping interval to be (25,30) and not (%f,%f)", in.low, in.high));

		it.intervalDelete(ivl);

		ivl = it.intervalSearch(Interval<float>(24,26));

		Assume(ivl != NULL, ("Expected to locate a second overlapping range for (24,26)"));

		in = ivl->getInterval();
		Assume(in.low == 26 && in.high == 26, ("Expected the overlapping interval to be (15,23) and not (%f,%f)", in.low, in.high));

		it.intervalDelete(ivl);

		ivl = it.intervalSearch(Interval<float>(24,26));

		if (ivl != NULL)
			in = ivl->getInterval();
		Assume(ivl == NULL, ("Expected to not locate a third overlapping range for (24,26). Instead, found (%d,%d)", in.low, in.high));

		return true;
	}
};


#endif /* INTERVALTREE_TEST_H_ */
