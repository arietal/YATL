//
//  rule_test.h
//  yatl
//
//  Created by Arie Tal on 2012-08-05.
//  Copyright (c) 2012 Arie Tal. All rights reserved.
//

#ifndef RULE_TEST_H_
#define RULE_TEST_H_


#include "../test.h"
#include "../../include/fuzzy_logic/rule.h"

/**
 * Theorizing about the interface
 *
 * enum MyCrispOutputs {
 *     outputVal1,
 *     outputVal2,
 *     outputVal3
 *     ...
 * };
 *
 * typedef Defuzzify<
 *     term< outputVal1, .... >,
 *     term< outputVal2, .... >,
 *     term< outputVal3, .... >
 * > myDefuzzifier;
 *
 *
 * //uses
 * switch (myFuzzyProram.evaluate(input)) {
 *   case outputVal1: ...
 *        break;
 *   case outputVal2: ...
 *        break;
 *   ...
 * };
 *
 * // examples form FCL
 * DEFUZZIFY value
 *    TERM cold := (5, 0.0), (10.0, 1.0), (18.5, 0.0);
 *    TERM warm := (17.5, 0.0), (20.0, 1.0), (22.5, 0.0);
 * END_DEFUZZIFY
 *
 * // in this framework
 * struct MyDefuzzifier {
 *     enum Outputs { cold, warm };
 *     typedef Defuzzify<
 *               term<cold, P< LR<5,2>, LR<0> >, P< LR<10>, LR<1> >, P< LR<37,2>, LR<0> >
 *               term<warm, P< LR<35,2>, LR<0> >, P< LR<20>, LR<1> >, P< LR<45,2>, LR<0> >
 *             > defuzzifier;
 *     static Outputs evaluate(const LongRational& input) {
 *         return defuzzifier.evaluate(input);
 *     }
 * };
 *
 * //uses
 * switch (MyDefuzzifier.evaluate(temperature)) {
 *   case cold: // code to act on temperature =~ cold
 *        break;
 *   case warm: // code to act on temperature =~ warm
 *        break;
 *   ...
 * };
 *
 *
 */

using namespace yatl;

#include <iostream>
#include <sys/time.h>
using namespace std;


class RuleTest : public Test {
public:
	const char *getName() const { return "Rule Test"; }
	bool runTest() {
        
        
        Assume((fact<rule_confidence<60> >::truth() == LR<1,1>()), ("Expected truth value = 1"));

        Assume((fact<rule_confidence<60> >::confidence() == LR<3,5>()), ("Expected confidence value = 3/5"));

        Assume((threshold< LR<0,1>, LR<1,1>, rule_confidence<60>, LR<1,1>, LR<3,5> >::truth() == LR<3,5>()), ("Expected truth value = 3/5"));

        Assume((threshold< LR<0,1>, LR<1,1>, rule_confidence<60>, LR<1,1>, LR<6,5> >::truth() == LR<1,1>()), ("Expected truth value = 1/1"));

        Assume((threshold< LR<0,1>, LR<1,1>, rule_confidence<60>, LR<1,1>, LongRational >(LR<6,5>()).getTruth( ) == LR<1,1>()), ("Expected truth value = 1/1"));

        Assume((threshold< LR<0,1>, LR<1,1>, rule_confidence<60>, LR<1,1>, LongRational >(LongRational(6,5)).getTruth( ) == LR<1,1>()), ("Expected truth value = 1/1"));

        Assume((zero_one_threshold< rule_confidence<70>, LR<1,1> >(LongRational(6,5)).getTruth() == LR<1,1>()), ("Expected truth value = 1/1"));

        Assume((rule_and_op< zero_one_threshold< rule_confidence<70>, LR<1,1>, rule_truth<60> >, zero_one_threshold< rule_confidence<60>, LR<1,1>, rule_truth<80> > >().getTruth() == rule_truth<60>()), ("Expected truth<60> & truth<80> = truth<60>"));

        Assume(((zero_one_threshold< rule_confidence<70>, LR<1,1> >(rule_truth<60>()) & zero_one_threshold< rule_confidence<60>, LR<1,1> >(rule_truth<80>())).getTruth() == rule_truth<60>()), ("Expected truth<60> & truth<80> = truth<60>"));

        Assume((rule_or_op< zero_one_threshold< rule_confidence<70>, LR<1,1>, rule_truth<60> >, zero_one_threshold< rule_confidence<60>, LR<2,1>, rule_truth<80> > >().getOutput() == LR<2,1>()), ("Expected output = 2"));

        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<0,5> >::output() == LR<0>()), ("Expected 0"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<3,5> >::output() == LR<1,5>()), ("Expected 1/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<6,5> >::output() == LR<2,5>()), ("Expected 2/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<9,5> >::output() == LR<3,5>()), ("Expected 3/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<12,5> >::output() == LR<4,5>()), ("Expected 4/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<15,5> >::output() == LR<1>()), ("Expected 1"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<18,5> >::output() == LR<4,5>()), ("Expected 4/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<21,5> >::output() == LR<3,5>()), ("Expected 3/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<24,5> >::output() == LR<2,5>()), ("Expected 2/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<27,5> >::output() == LR<1,5>()), ("Expected 1/5"));
        Assume((const_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > >, LR<30,5> >::output() == LR<0>()), ("Expected 0"));

        Assume((var_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > > >::output(LongRational(24,5)) == LR<2,5>()), ("Expected 2/5"));

        typedef var_term< list<P< LR<0>, LR<0> >, P< LR<3> , LR<1> >, P< LR<6>, LR<0> > > > myterm;

        for (int i=1; i<30; i++) {
        	LongRational expected;
        	if (i <= 15)
        		expected = LongRational(i,15);
        	else
        		expected = LongRational(30-i,15);
        	Assume((myterm::output(LongRational(i,5)) == expected), ("Unexpected result for %d/5", i));
        }

        struct MyDefuzzifier {
            enum Outputs { freezing, cold, warm, hot, scorching, hell };
            typedef Fuzzify<
            		  term<freezing, triangular< LR<-10>, LR<0>, LR<3> > >,
                      term<cold, triangular< LR<5,2>, LR<10>, LR<37,2> > >,
                      term<warm, triangular< LR<35,2>, LR<20>, LR<28> > >,
                      term<hot, triangular< LR<25>, LR<40>, LR<60> > >,
            		  term<scorching, trapezoidal< LR<50>, LR<70>, LR<130>, LR<190> > >,
            		  term<hell, trapezoidal< LR<150>, LR<210>, LR<250>, LR<300> > >
                    > fuzzifier;
            static Outputs defuzzify(const LongRational& input) {
                return (Outputs)fuzzifier::defuzzify(input);
            }
        };


        timeval t1, t2;
        const long vmax=200;
        const long itr=800000;
        gettimeofday(&t1, NULL);
        int sum=0;
        for (int j=0; j < itr; j++)
        for (int i=0; i < vmax; i++) {
			MyDefuzzifier::Outputs o = MyDefuzzifier::defuzzify(LongRational(i,3));
			sum += (int)o;
        }
        gettimeofday(&t2, NULL);
		double diffTime = ((double)t2.tv_sec*1.0E6+(double)t2.tv_usec) -
				((double)t1.tv_sec*1.0E6+(double)t1.tv_usec);

        cout << "total time: " << diffTime/1.0E6 << "sec" << endl;
        double avgDefuzz =  (diffTime / (itr*vmax)) * 1000.0;
        cout << "avg defuzz: " << avgDefuzz << "nsec" << endl;
        cout << "defuzz/min: " << ((itr*vmax)/diffTime)*60.0 << "M" << endl;

        double CPUSpeed = 1.80E9;

        cout << "cyc/defuzz: " << CPUSpeed * avgDefuzz / 1.0E9 << endl;

        for (int i=0; i < 200; i++) {
			cout << "Testing " << i << " ... ";
			switch(MyDefuzzifier::defuzzify(LongRational(i))) {
			case MyDefuzzifier::cold:
				cout << "Cold!" << endl;
				break;
			case MyDefuzzifier::warm:
				cout << "Warm and cozy" << endl;
				break;
			case MyDefuzzifier::hot:
				cout << "Hot... hot ...." << endl;
				break;
			case MyDefuzzifier::scorching:
				cout << "Scorching hot!" << endl;
				break;
			case MyDefuzzifier::hell:
				cout << "h-e-l-l!" << endl;
				break;
			default:
				cout << "Oops, temperature not a member" << endl;
				break;
			}
	   }


        return true;
    }
};


#endif
