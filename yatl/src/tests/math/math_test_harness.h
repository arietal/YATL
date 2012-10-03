/*
 * math_test_harness.h
 *
 *  Created on: Sep 14, 2012
 *      Author: arietal
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
