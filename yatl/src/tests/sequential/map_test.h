/*
 * map_test.h
 *
 *  Created on: Jul 7, 2012
 *      Author: arietal
 */

#ifndef MAP_TEST_H_
#define MAP_TEST_H_

#include <iostream>

#include "../test.h"
#include "../../include/sequential/map.h"
using namespace yatl;


class MapTest : public Test {
public:
	const char *getName() const { return "Map Test"; }
	bool runTest() {
		Map<int,int> map;
		for (int i=0; i < 10; i++) {
			map.put(i, i*i);
		}

		Assume(map.containsKey(5), ("Expected to find mapping for key 5"));

		Assume(map.get(5) == 25, ("Expected a mapping 5 -> 25, got 5 -> %d", map.get(5)));

		map.remove(5);

		Assume(!map.containsKey(5), ("Expected to not find mapping for key 5"));

		return true;
	}
};


#endif /* MAP_TEST_H_ */
