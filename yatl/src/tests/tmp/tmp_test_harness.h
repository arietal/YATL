/*
 * tmp_test_harness.h
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
