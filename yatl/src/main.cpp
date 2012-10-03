/*
 * main.cpp
 *
 *  Created on: Jun 22, 2012
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

#include <iostream>
using namespace std;

#include "tests/tmp/tmp_test_harness.h"
#include "tests/math/math_test_harness.h"
//#include "tests/concurrent/concurrentcontainer_test.h"
#include "tests/sequential/tree_test_harness.h"
#include "tests/sequential/map_test.h"
#include "tests/fuzzy_logic/rule_test.h"

//#include <amino/ordered_list.h>

//#include <amino/ordered_list.h>

int main(int argc, char* argv[]) {

	TestHarness<TMPTestHarness>().run();

	TestHarness<MathTestHarness>().run();

	//TestHarness<TreeTestHarness>().run();

	//Tester<MapTest>().run();

//	Tester<ConcurrentContainerTest<amino::OrderedList<int> > >().run();


    
    //Tester<RuleTest>().run();

	return 0;
}



