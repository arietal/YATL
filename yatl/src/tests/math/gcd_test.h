/*
 * gcd_test.h
 *
 *  Created on: Aug 4, 2012
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
