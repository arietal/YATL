//
//  rational_test.h
//  yatl
//
//  Created by Arie Tal on 2012-08-05.
//  Copyright (c) 2012 Arie Tal. All rights reserved.
//

#ifndef RATIONAL_TEST_H_
#define RATIONAL_TEST_H_


#include "../test.h"
#include "../../include/math/rational.h"

using namespace yatl;

class RationalTest : public Test {
public:
	const char *getName() const { return "Rational Test"; }
	bool runTest() {
        
		Assume((rational_eq_op<R<4,6>, R<2,3> >::result), ("Expected 4/6 == 2/3"));
        
        R<4,6> r1;
        R<2,3> r2;
        R<4,3> r3;
        R<8,18> r4;
        R<2,1> r5;
        R<14,9> r6;

        rational_var rtn(0,1);
        rtn = r2;
        Rational rt(8,3);
        Rational rt2 = r2 + rt;
        Rational r = r2 + r3 / r5;

		Assume((r2 == r1), ("Expected 4/6 = 2/3"));

        Assume((r2 + r1 == r3), ("Expected 4/6+2/3 = 4/3"));
        
        Assume((r1 * r2 == r4), ("Expected 4/6 * 2/3 = 4/9"));
        
        Assume((r3 / r2 == r5), ("Expected 4/6 / 2/3 = 2"));
        
        Assume((r1+ r2 * r3 == r6), ("Expected 4/6 = (2/3 * 4/3) = 14/9"));
        
        Assume((r2 == Rational(6,9)), ("Expected 2/3 == 6/9"));

        Assume((r == R<4,3>()), ("Expected 2/3 + 4/3 / 2/1 = 4/3"));
        
        Assume((rational_eq_op<R<14,9>, rational_add_op<R<4,6>, rational_mpy_op<R<2,3>,R<4,3> > > >::result == 1), ("Expected 14/9 = 4/6 + (2/3 * 4/3)"));

        Assume((max(r2,r3) == r3), ("Expected max(2/3,4/3) = 4/3"));

        Assume((max(r2,r) == r3), ("Expected max(2/3,4/3) = 4/3"));

        Assume((min(r2,r3) == r2), ("Expected min(2/3,4/3) = 2/3"));

        Assume((min(r2,r) == r2), ("Expected min(2/3,4/3) = 2/3"));

        Assume((rational_min_op<R<2,3>, R<4,5> >::nominator == 2), ("Expected min(2/3,4/5) == 2/3"));

        return true;
	}
};


#endif
