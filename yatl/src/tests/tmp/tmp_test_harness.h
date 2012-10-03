/*
 * tmp_test_harness.h
 *
 *  Created on: Sep 14, 2012
 *      Author: arietal
 */

#ifndef TMP_TEST_HARNESS_H_
#define TMP_TEST_HARNESS_H_

#include "../test.h"
#include "list_test.h"
#include "bst_test.h"

class TMPTestHarness : public Harness {
public:
	const char *getName() const { return "Template Metaprogramming Test Harness"; }
	bool runTest() {
		bool rc = true;
		rc &= Tester<ListTest>().run();
		rc &= Tester<BstTest>().run();
		return rc;
	}
};




#endif /* TMP_TEST_HARNESS_H_ */
