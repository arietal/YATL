/*
 * composite_types_test.h
 *
 *  Created on: Sep 17, 2012
 *      Author: arietal
 */

#ifndef COMPOSITE_TYPES_TEST_H_
#define COMPOSITE_TYPES_TEST_H_


#include "../test.h"
#include "../../include/math/composite_types.h"

using namespace yatl;

class CompositeTypesTest : public Test {
public:
	const char *getName() const { return "Composite Types Test"; }
	bool runTest() {

		LongRational r1(5,2), r2(7,2);
		Long fv(5), fr(4);
		Assume((fv != fr), ("Expected fv != fr"));
		LongRational r = r1 + r2;
		Assume((r == LongRational(6)), ("Expected r=6"));

		typedef LR<5,2> t1;
		typedef LR<7,3> t2;
		typedef LR<2,5> t3;
		typedef LR<6> t4;

		Assume((t1::mpy<t2>::add<t3>::eq< LR< 187, 30> >::result), ("Expected 187/30"));

		Assume((LR<4,6>::eq<LR<2,3> >::result), ("Expected 4/6 == 2/3"));


		Assume((div_op< t1, t2 >::eq< LR< 15,14> >::result), ("Expected 15/14"));

		Assume((div_op< t4, LR<2> >::simplify::denominator == 1), ("Expected 1"));

		Assume((eq_op<LR<4,6>, LR<2,3> >::result), ("Expected 4/6 == 2/3"));

        LR<4,6> R1;
        LR<2,3> R2;
        LR<4,3> R3;
        LR<8,18> R4;
        LR<2,1> R5;
        LR<14,9> R6;

        LongRational rtn(0,1);
        rtn = R2;
        LongRational rt(8,3);
        LongRational rt2 = R2 + rt;
        r = R2 + R3 / R5;

		Assume((R2 == R1), ("Expected 4/6 = 2/3"));

        Assume((R2 + R1 == R3), ("Expected 4/6+2/3 = 4/3"));

        Assume((R1 * R2 == R4), ("Expected 4/6 * 2/3 = 4/9"));

        Assume((R3 / R2 == R5), ("Expected 4/6 / 2/3 = 2"));

        Assume((R1+ R2 * R3 == R6), ("Expected 4/6 = (2/3 * 4/3) = 14/9"));

        Assume((R2 == LongRational(6,9)), ("Expected 2/3 == 6/9"));

        Assume((r == LR<4,3>()), ("Expected 2/3 + 4/3 / 2/1 = 4/3"));

        Assume((eq_op<LR<14,9>, add_op<LR<4,6>, mpy_op<LR<2,3>,LR<4,3> > > >::result == 1), ("Expected 14/9 = 4/6 + (2/3 * 4/3)"));

        Assume((max(R2,R3) == R3), ("Expected max(2/3,4/3) = 4/3"));

        Assume((max(R3,r) == R3), ("Expected max(2/3,4/3) = 4/3"));

        Assume((min(R2,R3) == R2), ("Expected min(2/3,4/3) = 2/3"));

        Assume((min(R2,r) == R2), ("Expected min(2/3,4/3) = 2/3"));

        Assume((min_op<LR<2,3>, LR<4,5> >::nominator == 2l), ("Expected min(2/3,4/5) == 2/3"));

        return true;
	}
};




#endif /* COMPOSITE_TYPES_TEST_H_ */
