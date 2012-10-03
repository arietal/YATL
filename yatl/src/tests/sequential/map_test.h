/*
 * map_test.h
 *
 *  Created on: Jul 7, 2012
 *      Author: arietal
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
