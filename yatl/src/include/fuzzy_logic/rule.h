/*
 * rule.h
 *
 *  Created on: Aug 5, 2012
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

#ifndef RULE_H_
#define RULE_H_

#include "../tmp/list.h"
#include "../math/geometry.h"


using namespace yatl;
using namespace yatl::List;

namespace yatl {

template <int n>
struct rule_truth : public LR<n,100> {};

template <int n>
struct rule_confidence : public LR<n,100> {} ;

template <typename truthType, typename confidenceType, typename outputType >
struct rule {
    typedef truthType truth;
    typedef confidenceType confidence;
    typedef outputType output;
    static truthType getTruth() { return truth(); }
    static confidenceType getConfidence() { return confidence(); }
    static outputType getOutput() { return output(); }
};

template <typename confidenceType, typename outputType>
struct rule<LongRational, confidenceType, outputType> {
	LongRational truth_;
	typedef LongRational truth;
	typedef confidenceType confidence;
	typedef outputType output;
	LongRational getTruth() const { return truth_; }
	static confidenceType getConfidence() { return confidence(); }
	static outputType getOutput() { return output(); }
	rule(const LongRational& truthValue) : truth_(truthValue) {}
};

template <typename confidenceType>
struct rule<LongRational, confidenceType, LongRational > {
	LongRational truth_;
	LongRational output_;
	typedef LongRational truth;
	typedef LongRational output;
	typedef confidenceType confidence;
	LongRational getTruth() const { return truth_; }
	static confidenceType getConfidence() { return confidence(); }
	LongRational getOutput() const { return output_; }
	rule(const LongRational& truthValue, const LongRational& outputValue=LongRational()) : truth_(truthValue), output_(outputValue) {}
};



template <typename confidence>
struct fact : public rule<rule_truth<100>, confidence, LR<1,1> > {};

struct absolute_fact : public fact<rule_confidence<100> > {};

template <typename lowerBound, typename upperBound, typename result>
struct rule_relative_ratio : public div_op<sub_op<result, lowerBound>, sub_op<upperBound, lowerBound> > {};

template <typename lowerBound, typename upperBound, typename confidence, typename outputType, typename result>
struct threshold : public rule<
min_op< max_op< result, lowerBound >, upperBound >,
confidence, outputType> {};

template <typename lowerBound, typename upperBound, typename confidence, typename outputType>
struct threshold<lowerBound,upperBound,confidence,outputType,LongRational > : public rule<LongRational, confidence, outputType> {
	threshold(const LongRational& truthValue) : rule<LongRational, confidence, outputType>(truthValue.template max<lowerBound>().template min<upperBound>()) {}
};



template <typename confidence, typename outputType, typename result = LongRational >
struct zero_one_threshold : public threshold<typename result::zero, typename result::one,  confidence, outputType, result> {
	zero_one_threshold(const LongRational& truthValue) : threshold<typename confidence::zero, typename confidence::one, confidence, outputType, result >(truthValue) {}
};

template <typename upperBound, typename result, typename confidence, typename outputType>
struct zero_bound_threshold : public threshold<typename result::zero, upperBound, confidence, outputType, result> {};

// rule combination operations

#define __rule_and_op_SuperType rule<\
typename if_else<same_type<typename rule1::truth, LongRational >::result == 1 || same_type<typename rule2::truth, LongRational >::result == 1, LongRational, min_op<typename rule1::truth, typename rule2::truth> >::type,\
sub_op< typename rule1::truth::one,\
     mpy_op<\
        sub_op< typename rule1::truth::one, typename rule1::confidence >,\
        sub_op< typename rule2::truth::one, typename rule2::confidence> > >,\
        typename rule1::output>

template <typename rule1, typename rule2>
struct rule_and_op : public __rule_and_op_SuperType {
	// assert if rule1::output and rule2::output differ
	typedef int __Assert_rule1_output__and__rule2_output__must_be_identical[same_type<typename rule1::output, typename rule2::output>::result - 1] ;
	rule_and_op(const rule1& r1, const rule2& r2) : __rule_and_op_SuperType(r1.getTruth().min(r2.getTruth())) {}
	rule_and_op() {}
};

#define __rule_or_op_SuperType rule<\
typename if_else<same_type<typename rule1::truth, LongRational >::result == 1 || same_type<typename rule2::truth, LongRational >::result == 1,\
					LongRational, max_op<typename rule1::truth, typename rule2::truth> >::type,\
sub_op< typename rule1::truth::one,\
     mpy_op<\
        sub_op< typename rule1::truth::one, typename rule1::confidence >,\
        sub_op< typename rule2::truth::one, typename rule2::confidence> > >,\
     typename if_else<same_type<typename rule1::output, LongRational >::result == 1 || same_type<typename rule2::output, LongRational >::result == 1,\
		LongRational,\
		typename if_else<eq_op<max_op<typename rule1::truth, typename rule2::truth>, typename rule1::truth> ::result == 1,\
					typename rule1::output, typename rule2::output>::type >::type>


template <typename rule1, typename rule2>
struct rule_or_op : public __rule_or_op_SuperType {
	rule_or_op(const rule1& r1, const rule2& r2) : __rule_or_op_SuperType(r1.getTruth().max(r2.getTruth()), (r1.getTruth().max(r2.getTruth()) == r1.getTruth())?r1.getOutput() : r2.getOutput()) {}
	rule_or_op()  {}
};

template <typename rule1, typename rule2>
rule_and_op<rule1,rule2> operator& (const rule1& r1, const rule2& r2) {
	return rule_and_op<rule1,rule2>(r1,r2);
}


template<typename p1, typename p2, typename input>
struct const_membership {
	// assert if p1::y < 0 or p1::y > 1 or p2::y < 0 or p2::y > 1
	typedef int __Assert_membership_ranges_must_be_between_zero_and_one[
	                0 - (lt_op<typename p1::y, typename input::zero>::result == 1 ||
	                   gt_op<typename p1::y, typename input::one>::result == 1 ||
	                   lt_op<typename p2::y, typename input::zero>::result == 1 ||
	                   gt_op<typename p2::y, typename input::one>::result == 1)];

	// if (input < p1::x) result = input::zero
	// if (input > p2::x) result = input::zero
	// if p2::y > p1::y
	//     delta = (p2::y - p1::y) / (p2::x - p1::x)
	//     result = (input - p1::x) * delta = input*delta - p1::x*delta
	// else
	//     delta = (p1::y - p2::y) / (p2::x - p1::x)
	//     result = (p2::x - input) * delta = p2::x*delta - input*delta
	typedef typename if_else< gt_op<typename p2::y, typename p1::y>::result == 1,
			div_op< sub_op<typename p2::y, typename p1::y>, sub_op<typename p2::x, typename p1::x> >,
			div_op< sub_op<typename p1::y, typename p2::y>, sub_op<typename p2::x, typename p1::x> > > :: type delta;
	typedef typename if_else<lt_op< input, typename p1::x>::result == 1, typename input::zero,
			   typename if_else<gt_op< input, typename p2::x>::result == 1, typename input::zero,
			   	   typename if_else< gt_op<typename p2::y, typename p1::y>::result == 1,
			   	   	   sub_op< mpy_op<input, delta>, mpy_op<typename p1::x, delta> >,
			   	   	   sub_op< mpy_op< typename p2::x, delta>, mpy_op<input, delta> > >::type >::type >::type  _result;
	typedef LR<_result::nominator,_result::denominator> output;
};

template <typename p1, typename p2>
struct var_membership {
	typedef typename if_else< gt_op<typename p2::y, typename p1::y>::result == 1,
			div_op< sub_op<typename p2::y, typename p1::y>, sub_op<typename p2::x, typename p1::x> >,
			div_op< sub_op<typename p1::y, typename p2::y>, sub_op<typename p2::x, typename p1::x> > > :: type delta;
	enum { up = gt_op<typename p2::y, typename p1::y>::result - gt_op<typename p1::y, typename p2::y >::result };
	typedef LR<up * delta::nominator, delta::denominator> sdelta;
	typedef mpy_op<typename p1::x, delta> subtractor;
	typedef mpy_op<typename p2::x, delta> subtracted;
	typedef typename if_else< (up > 0), LR< -subtractor::nominator, subtractor::denominator >,
			             typename if_else< (up < 0), LR< subtracted::nominator, subtracted::denominator >, typename p2::y >::type >::type addend;

	static LongRational output(const LongRational& input) {
			return ((!input.in_range<typename p1::x, typename p2::x>()) ? LongRational(0) : input.ma<sdelta, addend>());
	}
};


template <typename p1, typename p2, typename input, typename membership=const_membership<p1,p2,input> >
struct const_binary_term {
	typedef typename membership::output output;
};

template <typename p1, typename p2, typename membership=var_membership<p1,p2> >
struct var_binary_term : public membership {
	static LongRational output(const LongRational& input) {
		return membership::output(input);
	}
};


template <typename l, typename input, typename membership=const_membership<typename l::List_::Node, typename l::List_::Next::Node, input > >
struct const_term {
	typedef max_op< typename const_binary_term< typename l::List_::Node, typename l::List_::Next::Node, input, membership >::output,
				typename const_term< list<typename l::List_::Next>, input>::output > output;
};

template <typename l, typename membership=var_membership<typename l::List_::Node, typename l::List_::Next::Node > >
struct var_term {
	typedef typename l::List_::Node::x minx;
	typedef typename var_term<list<typename l::List_::Next> > ::maxx maxx;
	typedef l List;
	static LongRational output(const LongRational& input) {
		if (input.lt<typename l::List_::Node::x>())
			return LongRational(0);
		if (!input.gt<typename l::List_::Next::Node::x>()) {
			LongRational out = var_binary_term<typename l::List_::Node, typename l::List_::Next::Node, membership>::output(input);
			if (out.ne< LR<0,1> >())
				return out;
		}
//		if (out.gt< LR<0,1> >())
//			return out;
		LongRational out1 = var_term<list<typename l::List_::Next> >::output(input);
		return out1;
//		if (out.ne< LR<0,1> >() && out1.ne< LR<0,1> >()) {
//			cout << "out = " << out << endl;
//			cout << "out1 = " << out1 << endl;
//			cout << "input = " << input << endl;
//		} /*else if (out.gt< LR<0,1> >()) {
//			cout << "out = " << out << endl;
//			cout << "out1 = " << out1 << endl;
//			cout << "input = " << input << endl;
//			return out;
//		}*/
//		return max<0,0,0,0>(out,out1);
//		return ((input < typename l::List_::Node::x()) ?
//				LongRational(0) :
//				((var_binary_term<typename l::List_::Node, typename l::List_::Next::Node, membership>::output(input).getNominator() > 0) ?
//						var_binary_term<typename l::List_::Node, typename l::List_::Next::Node, membership>::output(input) :
//						var_term<list<typename l::List_::Next> >::output(input)) );
	}


};


template <typename p1, typename p2, typename input, typename membership>
struct const_term<list<list_<p1, list_<p2, _NIL> > >, input, membership> {
	typedef typename const_binary_term< p1, p2, input, membership >::output output;
};

template <typename p1, typename p2, typename membership>
struct var_term<list<list_<p1, list_<p2, _NIL> > >, membership> {
	typedef typename p1::x minx;
	typedef typename p2::x maxx;
	typedef list<list_<p1, list_<p2, _NIL> > > List;
	static LongRational output(const LongRational& input) {
		return var_binary_term<p1, p2, membership>::output(input);
	}
};


template <long lcm, typename vterm_>
struct lcm_var_term : public vterm_ {
	static const long factor1_ = lcm / vterm_::minx::denominator;
	static const long factor2_ = lcm / vterm_::maxx::denominator;
	typedef LR<vterm_::minx::nominator * factor1_, lcm> minx;
	typedef LR<vterm_::maxx::nominator * factor2_, lcm> maxx;
};


template <typename x1, typename x2, typename x3>
struct triangular {
	typedef list< P< x1, LR<0> >,  P< x2, LR<1> >, P<x3, LR<0> > > list;
};

template <typename x1, typename x2, typename x3, typename x4>
struct trapezoidal {
	typedef list< P< x1, LR<0> >, P< x2, LR<1> >, P< x3, LR<1> >, P< x4, LR<0> > > list;
};

template <int outputId, typename p1, typename p2=_NIL, DEF_PARAM_LIST>
struct term {
	enum { output = outputId };
	typedef var_term< list<p1, PARAM_LIST> > vterm;
	typedef term o_type;
	template <typename term2>
	struct compare {
		enum { result = gt_op<typename vterm::minx, typename term2::vterm::minx>::result -
			lt_op<typename vterm::minx, typename term2::vterm::minx>::result };
	};
};

template <int outputId, typename x1, typename x2, typename x3>
struct term<outputId, triangular<x1,x2,x3> > {
	enum { output = outputId };
	typedef var_term< typename triangular<x1,x2,x3>::list > vterm;
	typedef term o_type;
	template <typename term2>
	struct compare {
		enum { result = gt_op<typename vterm::minx, typename term2::vterm::minx>::result -
			lt_op<typename vterm::minx, typename term2::vterm::minx>::result };
	};
};

template <int outputId, typename x1, typename x2, typename x3, typename x4>
struct term<outputId, trapezoidal<x1,x2,x3,x4> > {
	enum { output = outputId };
	typedef var_term< typename trapezoidal<x1,x2,x3,x4>::list > vterm;
	typedef term o_type;
	template <typename term2>
	struct compare {
		enum { result = gt_op<typename vterm::minx, typename term2::vterm::minx>::result -
			lt_op<typename vterm::minx, typename term2::vterm::minx>::result };
	};
};

template <int lcm, typename term_>
struct lcm_term : public term_ {
	typedef lcm_var_term<lcm, typename term_::vterm> vterm;
};

template <typename l, bool fast>
struct Defuzzifier {
	static int defuzzify(const LongRational& input, const LongRational& max, int outputId) {
		if ((fast) ? input.fast_lt<typename l::List_::Node::vterm::minx>() : input.lt<typename l::List_::Node::vterm::minx>())
			return outputId;

		if ((fast) ? !input.fast_gt<typename l::List_::Node::vterm::maxx>() : !input.gt<typename l::List_::Node::vterm::maxx>() ) {
			LongRational v = l::List_::Node::vterm::output(input);
			if (v > max)
				return Defuzzifier<list<typename l::List_::Next>, fast >::defuzzify(input, v, (int)l::List_::Node::output );
		}// else {
//			cout << "input = " << input <<endl;
//			cout << "minx = " << typename l::List_::Node::vterm::minx() << endl;
//			cout << "maxx = " << typename l::List_::Node::vterm::maxx() << endl;
//		}
		return Defuzzifier<list<typename l::List_::Next>, fast >::defuzzify(input, max, outputId);
	}
};

template<bool fast>
struct Defuzzifier<list<_NIL>, fast > {
	static int defuzzify(const LongRational& input, const LongRational& max, int outputId) {
		return outputId;
	}
};

template <typename l>
struct list_lcm {
	enum { result = lcm_const<l::Node::denominator, list_lcm<typename l::Next>::result >::result };
};

template <typename n>
struct list_lcm<list_<n, _NIL> > {
	enum { result = n::denominator } ;
};

template <typename p1, typename p2=_NIL, DEF_PARAM_LIST>
struct Fuzzify {
	template <typename term_>
	struct extract_minx {
		typedef typename term_::vterm::minx result;
	};
	template <typename term_>
	struct extract_maxx {
		typedef typename term_::vterm::maxx result;
	};
	typedef typename list<p1, PARAM_LIST>::template collect<extract_minx> minxList;
	typedef typename list<p1, PARAM_LIST>::template collect<extract_maxx> maxxList;
	static const long lcm_ =  list_lcm<typename minxList::template concat<maxxList>::List_>::result;

	template <typename term_>
	struct apply_lcm {
		typedef lcm_term<lcm_, term_> result;
	};

	template <typename term_>
	struct print {
		static void scan(ostream& os=cout) {
			os << "term[outputId = " << term_::output << ", minx = " << typename term_::vterm::minx() << ", maxx = " << typename term_::vterm::maxx() << "]" ;
		}
		static void pre(ostream& os=cout) {}
		static void post(ostream& os=cout) { os << endl; }
		static void postScan(ostream& os=cout) { os << endl; }
	};

	typedef typename list<p1, PARAM_LIST>::template collect<apply_lcm>::sort list;
	static int defuzzify(const LongRational& input) {
		if ((lcm_ / input.denominator) * input.denominator == lcm_) { // fast path
			long factor = lcm_ / input.denominator;
			LongRational input_(input.nominator * factor, lcm_);
			return Defuzzifier<list,true>::defuzzify(input_, LongRational(0), -1);
		} else {
			return Defuzzifier<list,false>::defuzzify(input, LongRational(0), -1);
		}
	}
};

} // namespace

#endif
