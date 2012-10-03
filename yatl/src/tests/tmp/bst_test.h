/*
 * bst_test.h
 *
 *  Created on: Sep 7, 2012
 *      Author: arietal
 */

#ifndef BST_TEST_H_
#define BST_TEST_H_

#include "../test.h"
#include "../../include/math/composite_types.h"
#include "../../include/tmp/bst.h"
#include "../../include/tmp/intrange.h"

using namespace yatl;
using namespace yatl::List;

struct printData {
	typedef Integer arg1Type;
	template <typename typeArg>
	static bool execute(const arg1Type& input) { cout << "data = " << typeArg() << endl; return true; }
};

struct extractData {
	typedef Integer arg1Type;
	typedef Integer returnType;
	template <typename typeArg>
	static bool execute(const arg1Type& input, returnType& output) { output = typeArg(); return true; }
};

template <typename r>
struct mapit {
	typedef mapping< I<r::value>, I<r::template mpy<r>::value> > result;
};

class BstTest : public Test {
public:
	const char *getName() const { return "TMP::BinarySearchTree Test"; }
	bool runTest() {

		typedef bst< node < LR<5>, LR<25>,
							node< LR<3>, LR<9>,
								node< LR<1>, LR<1> >,
								node< LR<4>, LR<16> >
							>,
							node< LR<7>, LR<49>,
								node< LR<6>, LR<36> >,
								node< LR<8>, LR<64> >
							> > > myBst;

		Assume((myBst::find_< LR<6> >::eq<LR<36> >::result), ("Expected 6 -> 36"));

		typedef list<
				iimap< 5, 25 >,
				iimap< 3, 9 >,
				iimap< 1, 1 >,
				iimap< 4, 16 >,
				iimap< 7, 49 >,
				iimap< 6, 36 >,
				iimap< 8, 64 > >::sort  mappings;

		typedef bst_builder< mappings >::result myBst2;

		Assume((myBst2::find_< I<7> >::eq< I<49> >::result), ("Expected 6 -> 36"));
		Assume((myBst2::find_< I<6> >::eq< I<36> >::result), ("Expected 6 -> 36"));

		bool rc;
		Integer out;
		rc = myBst2::searchAndExecute< extractData >(4, out);
		Assume((rc && out == 16), ("Expected 4 - > 16"));

		rc = myBst2::searchAndExecute< extractData >(5, out);
		Assume((rc && out == 25), ("Expected 5 - > 25"));

		rc = myBst2::searchAndExecute< extractData >(2, out);
		Assume((!rc && out == 25), ("Expected rc = false, out to remain unchanged"));

		typedef bst_builder< intRange<0,50>::collect<mapit> >::result myBst3;
		Assume((myBst3::find_< I<15> >::eq< I<225> >::result), ("Expected 15 -> 225"));

		for (int i=40; i < 60; i++) {
			rc = myBst3::searchAndExecute< extractData >(i, out);
			if (i < 50) {
				Assume((rc && out == i*i), ("Expected %d -> %d", i, i*i));
			} else {
				Assume((!rc), ("Expected rc = false"));
			}
		}

//		myBst3::print();
		return true;
	}
};


#endif /* BST_TEST_H_ */
