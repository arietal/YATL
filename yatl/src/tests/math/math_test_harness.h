/*
 * math_test_harness.h
 *
 *  Created on: Sep 14, 2012
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

#ifndef MATH_TEST_HARNESS_H_
#define MATH_TEST_HARNESS_H_

#include "gcd_test.h"
//#include "rational_test.h"
#include "composite_types_test.h"
#include "geometry_test.h"

class MathTestHarness : public Harness {
public:
	const char *getName() const { return "Math Test Harness"; }
	bool runTest() {
		bool rc = true;
		rc &= Tester<GCDTest>().run();
	//	rc &= Tester<RationalTest>().run();
		rc &= Tester<CompositeTypesTest>().run();
		rc &= Tester<GeometryTest>().run();
		return rc;
	}
};




#endif /* MATH_TEST_HARNESS_H_ */
