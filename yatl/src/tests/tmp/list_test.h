/*
 * list_test.h
 *
 *  Created on: Aug 24, 2012
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

#ifndef LIST_TEST_H_
#define LIST_TEST_H_


#include "../test.h"
#include "../../include/math/composite_types.h"
#include "../../include/tmp/list.h"
#include "../../include/tmp/intrange.h"

using namespace yatl;
using namespace yatl::List;

template <typename r1, typename r2>
struct rational_rev_compare {
	enum { result = -cmp<r1,r2>::result };
};



template <typename r>
struct doubler {
	typedef mpy_op<r, LR<2> > result;
};

template <int v>
struct doubler<I<v> > {
	struct result : public  I<v>::template add<I<v> > {};
};

template <typename r>
struct identify {
	typedef r result;
};

template <typename r>
struct integerFilter {
	enum { result = r::simplify::denominator == 1 };
};

template <typename r>
struct evenFilter {
	enum { result = div_op<typename r::simplify, LR<2> >::simplify::denominator == 1 };
};

template <typename r>
struct oddFilter {
	enum { result = div_op<typename r::simplify, LR<2> >::simplify::denominator != 1 };
};

template <typename r>
struct noopFilter {
	enum { result = 1 };
};


class ListTest : public Test {
public:
	const char *getName() const { return "TMP::List Test"; }
	bool runTest() {


        typedef list< LR<8>, LR<2>, LR<3>, LR<7>, LR<1>, LR<9>, LR<2,3>, LR<11>, LR<6> > myList;

        Assume((myList::sort::compare< list< LR<2,3>, LR<1>, LR<2>, LR<3>, LR<6>, LR<7>, LR<8>, LR<9>, LR<11> > >::result == 0),
        		("Expected 2/3, 1, 2, 3, 6, 7, 8, 9, 11"));

        Assume((myList::sort_<rational_rev_compare>::compare< list < LR<11>, LR<9>, LR<8>, LR<7>, LR<6>, LR<3>, LR<2>, LR<1>, LR<2,3> > >::result == 0),
        		("Expected 11, 9, 8, 7, 6, 3, 2, 1, 2/3"));

        Assume((myList::filter<integerFilter>::collect<doubler>::sort::
        			compare< list< LR<2>, LR<4>, LR<6>, LR<12>, LR<14>, LR<16>, LR<18>, LR<22> > >::result == 0),
        		("Expected 2, 4, 6, 12, 14, 16, 18, 22"));

        Assume((myList::reverse::compare< list< LR<6>, LR<11>, LR<2,3>, LR<9>, LR<1>, LR<7>, LR<3>, LR<2>, LR<8> > >::result == 0),
        		("Expected 6, 11, 2/3, 9, 1, 7, 3, 2, 8"));

        Assume((myList::filter<evenFilter>::concat<myList::filter<oddFilter> >::
        			compare< list< LR<8>, LR<2>, LR<6>, LR<3>, LR<7>, LR<1>, LR<9>, LR<2,3>, LR<11> > >::result == 0),
        		("Expected 8, 2, 6, 3, 7, 1, 9, 2/3, 11"));

        Assume((myList::sublist<2,6>::compare< list< LR<3>, LR<7>, LR<1>, LR<9> > >::result == 0),
        		("Expected 3, 7, 1, 9"));

        Assume((myList::sublist<7,12>::compare< list< LR<11>, LR<6> > >::result == 0),
        		("Expected myList::sublist<7,12> = 11, 6"));

        Assume((myList::elementAt<3>() == LR<7>()),
        		("Expected element at index 3 is 7"));

        Assume((myList::indexOf< LR<7> >::result == 3),
        		("Expected index of 7 to be 3"));

        Assume((myList::indexOf< LR<18> >::result == -1),
        		("Expected index of non-existent element LR<18> to be -1"));

        Assume((myList::equals<myList::sort>::result == 1),
        		("Expected a list to equal a sorted version of itself"));

        Assume((equals< myList::List_, myList::filter<evenFilter>::List_>::result == 0),
        		("Expected a list to not equal a filtered version of itself with non-even elements removed"));

        Assume((list< LR<1>, LR<2>, LR<8> >::compare<list< LR<1>, LR<2>, LR<7> > >::result == 1),
        		("Expected 1,2,8 > 1,2,7"));

        Assume((list< LR<1>, LR<2>, LR<8>, LR<1> >::compare<list< LR<1>, LR<2>, LR<9> > >::result == -1),
        		("Expected 1,2,8,1 < 1,2,9"));

        Assume((list< LR<1>, LR<2>, LR<3>, LR<2>, LR<1> >::indexOf< LR<2> >::result == 1),
        		("Expected (1,2,3,2,1).indexOf(2) == 1"));

        Assume((list< LR<1>, LR<2>, LR<3>, LR<2>, LR<1> >::lastIndexOf< LR<2> >::result == 3),
        		("Expected (1,2,3,2,1).lastIndexOf(2) == 3"));

        LongRational* a = myList::sort::toArray();

        Assume((a[3] == LongRational(3)), ("Expected a[3] = 3"));

        Assume((intRange<0,5>::collect<doubler>::compare<list< I<0>, I<2>, I<4>, I<6>, I<8> > >::result ==0),
        		("Expected 0,2,4,6,8"));

		return true;
	}
};


#endif /* LIST_TEST_H_ */
