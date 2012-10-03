/*
 * main.cpp
 *
 *  Created on: Jun 22, 2012
 *      Author: arietal
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



