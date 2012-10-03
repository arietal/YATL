/*
 * gcd_test.h
 *
 *  Created on: Aug 4, 2012
 *      Author: arietal
 */

#ifndef GCD_TEST_H_
#define GCD_TEST_H_

#include "../test.h"
#include "../../include/math/gcd.h"

using namespace yatl;

class GCDTest : public Test {
public:
	const char *getName() const { return "GCD Test"; }
	bool runTest() {

		Assume((gcd<4,6>::result == 2), ("Expected GCD(4,6)=2"));

		Assume((gcd<1,3>::result == 1), ("Expected GCD(1,3)=1"));

		Assume((gcd<17,13>::result == 1), ("Expected GCD(17,13)=1"));

		Assume((gcd<12,8>::result == 4), ("Expected GCD(12,8)=4"));

		return true;
	}
};




#endif /* GCD_TEST_H_ */
