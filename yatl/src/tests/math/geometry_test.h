/*
 * geometry_test.h
 *
 *  Created on: Sep 14, 2012
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

#ifndef GEOMETRY_TEST_H_
#define GEOMETRY_TEST_H_

#include "../test.h"
#include "../../include/math/geometry.h"

using namespace yatl;

template <typename peak>
struct triangle {
	typedef P< sub_op< peak, LR<1> >, LR<0> > low;
	typedef P< peak, LR<1> > p;
	typedef P< add_op< peak, LR<1> >, LR<0> > high;
	typedef segment< low, p > s1;
	typedef segment< p, high > s2;
};

template <int start, int end>
struct comb_ : public list<list_< typename triangle< LR< start > >::s1, list_< typename triangle< LR< start > >::s2, typename comb_<start+1,end>::List_> > > {};

template <int end>
struct comb_<end,end> : public list<_NIL> {};

template <int start, int end>
struct comb : public multi_segment< comb_<start,end>, false > {};

class GeometryTest : public Test {
public:
	const char *getName() const { return "Geometry Test"; }
	bool runTest() {

		typedef segment< P< LR<0>, LR<0> >, P< LR<6>, LR<3> > > s1;
		typedef segment< P< LR<1,2>, LR<0> >, P< LR<3>, LR<3> > > s2;
		typedef segment< P< LR<0>, LR<1> >, P< LR<6>, LR<0> > > s3;
		typedef segment< P< LR<0>, LR<3> >, P< LR<6>, LR<0> > > s4;
		typedef segment< P< LR<4>, LR<2> >, P< LR<7>, LR<5> > > s5;
		typedef segment< P< LR<1>, LR<0> >, P< LR<7>, LR<2> > > s6;

		Assume((s1::a::eq< LR<1,2> >::result && s1::b::eq< LR<0> >::result), ("Expected a: 1/2, b: 0"));
		Assume((s2::a::eq< LR<6,5> >::result && s2::b::eq< LR<-3,5> >::result), ("Expected a: 6/5, b: -3/5"));
		Assume((s3::a::eq< LR<-1,6> >::result && s3::b::eq< LR<1> >::result), ("Expected a: -1/6, b: 1"));
		Assume((s4::a::eq< LR<-1,2> >::result && s4::b::eq< LR<3> >::result), ("Expected a: -1/2, b: 3"));


		Assume((s1::at< LR<3> >::eq< LR<3,2> >::result), ("Expected 3/2"));

		typedef seg_intersect<s1,s2> s1s2;
		Assume((s1s2::valid), ("Expected s1 to intersect s2"));
		Assume((s1s2::x::eq< LR<6,7> >::result && s1::at<s1s2::x>::eq<s2::at<s1s2::x> >::result), ("Expected 6/7, 3/7, 3/7"));

		typedef seg_intersect<s1,s3> s1s3;
		Assume((s1s3::valid), ("Expected s1 to intersect s3"));
		Assume((s1s3::x::eq< LR<3,2> >::result && s1::at<s1s3::x>::eq<s3::at<s1s3::x> >::result), ("Expected 3/2, 3/4, 3/4"));

		typedef seg_intersect<s1,s4> s1s4;
		Assume((s1s4::valid), ("Expected s1 to intersect s4"));
		Assume((s1s4::x::eq< LR<3> >::result && s1::at<s1s4::x>::eq<s4::at<s1s4::x> >::result), ("Expected 3, 3/2, 3/2"));

		typedef seg_intersect<s1,s5> s1s5;
		Assume((s1s5::valid), ("Expected s1 to intersect s5"));
		Assume((s1s5::x::eq< LR<4> >::result && s1::at<s1s5::x>::eq<s5::at<s1s5::x> >::result), ("Expected 4,2,2"));


		typedef seg_intersect<s1,s6> s1s6;
		Assume((!s1s6::valid), ("Expected s1 to not intersect s6"));

		Assume((!s2::overlaps<s5>::result), ("Expected s2 to not overlaps s5"));

		Assume((s2::overlaps<s6>::result), ("Expected s2 to overlap s6"));


		typedef multi_segment< list< s5, s2 > > mseg;
		Assume((mseg::segments::compare< list< segment< P< LR<1,2>, LR<0> >, P< LR<3>, LR<3> > >, segment< P< LR<4>, LR<2> >, P< LR<7>, LR<5> > > > >::result == 0),
				("Expected (sorted) [(1/2, 0) - (3, 3), a: 6/5, b: -3/5, minx: 1/2, miny: 0], [(4, 2) - (7, 5), a: 1, b: -2, minx: 4, miny: 2]"));

		typedef comb<1, 3> mseg2;

		Assume((mseg2::segments::compare< list<
				segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > >, segment< P< LR<1>, LR<0> >, P< LR<2>, LR<1> > >,
				segment< P< LR<1>, LR<1> >, P< LR<2>, LR<0> > >, segment< P< LR<2>, LR<1> >, P< LR<3>, LR<0> > > > >::result == 0),
				("Expected [(0, 0) - (1, 1), a: 1, b: 0, minx: 0, miny: 0], [(1, 0) - (2, 1), a: 1, b: -1, minx: 1, miny: 0], [(1, 1) - (2, 0), a: -1, b: 2, minx: 1, miny: 1], [(2, 1) - (3, 0), a: -1, b: 3, minx: 2, miny: 1]"));


		typedef segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > > ns1;
		typedef segment< P< LR<1,4>, LR<1> >, P< LR<1>, LR<0> > > ns2;

		Assume((maxfunc<ns1,ns2>::result::compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,4>, LR<1,4> > >, segment< P< LR<1,4>, LR<1> >, P< LR<4,7>, LR<4,7> > >,
				segment< P< LR<4,7>, LR<4,7> >, P< LR<1>, LR<1> > > > >::result == 0),
				("Expected [(0, 0) - (1/4, 1/4), a: 1, b: 0, minx: 0, miny: 0], [(1/4, 1) - (4/7, 4/7), a: -4/3, b: 4/3, minx: 1/4, miny: 1], [(4/7, 4/7) - (1, 1), a: 1, b: 0, minx: 4/7, miny: 4/7]"));

		Assume((maxfunc< segment< P< LR<0>, LR<0> >, P< LR<1,2>, LR<1,3> > >,
			     segment< P< LR<1,4>, LR<1,2> >, P< LR<1>, LR<1> > > >::result::
			     compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,4>, LR<1,6> > >, segment< P< LR<1,4>, LR<1,2> >, P< LR<1>, LR<1> > > > >::result == 0),
				("Expeted [(0, 0) - (1/4, 1/6), a: 2/3, b: 0, minx: 0, miny: 0], [(1/4, 1/2) - (1, 1), a: 2/3, b: 1/3, minx: 1/4, miny: 1/2]"));

		Assume((maxfunc< segment< P< LR<0>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
			     segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > >::result::
			     compare< list< segment< P< LR<0>, LR<2,3> >, P< LR<1,2>, LR<4,9> > >, segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result == 0),
				("Expected [(0, 2/3) - (1/2, 4/9), a: -4/9, b: 2/3, minx: 0, miny: 2/3], [(1/2, 2/3) - (1, 3/4), a: 1/6, b: 7/12, minx: 1/2, miny: 2/3]"));

		Assume((maxfunc< segment< P< LR<0>, LR<2,3> >, P< LR<1>, LR<1,3> > >,
			     segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<3,4> > > >::result::
			     compare< list< segment< P< LR<0>, LR<2,3> >, P< LR<1,2>, LR<1,2> > >, segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<3,4> > >,
			     segment< P< LR<3,4>, LR<5,12> >, P< LR<1>, LR<1,3> > > > >::result == 0),
				("Expected [(0, 2/3) - (1/2, 1/2), a: -1/3, b: 2/3, minx: 0, miny: 2/3], [(1/2, 2/3) - (3/4, 3/4), a: 1/3, b: 1/2, minx: 1/2, miny: 2/3], [(3/4, 5/12) - (1, 1/3), a: -1/3, b: 2/3, minx: 3/4, miny: 5/12]"));

		Assume((ns1::subtract<ns2>::compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,4>, LR<1,4> > > > >::result == 0),
				("Expected ns1 - ns2 = [(0, 0) - (1/4, 1/4), a: 1, b: 0, minx: 0, miny: 0] "));
		Assume((ns2::subtract<ns1>::size == 0), ("Expected ns2 - ns1 = empty list"));
		Assume((s2::subtract<s1>::size == 0), ("Expected s2 - s1 = empty list"));
		Assume((s1::subtract<s2>::compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,2>, LR<1,4> > >,
				segment< P< LR<3>, LR<3,2> >, P< LR<6>, LR<3> > > > >::result == 0),
				("Expected s1 - s2 = [(0, 0) - (1/2, 1/4), a: 1/2, b: 0, minx: 0, miny: 0], [(3, 3/2) - (6, 3), a: 1/2, b: 0, minx: 3, miny: 3/2]"));
		Assume((s5::subtract<s2>::compare< list< s5 > >::result == 0), ("Expected s5 -s2 = s5"));



		Assume((rec_maxfunc< list< segment< P< LR<0>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					     segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result::compare<
					     list< segment< P< LR<0>, LR<2,3> >, P< LR<1,2>, LR<4,9> > >,
					           segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > >
					     >::result == 0),
					     ("[(0, 2/3) - (1/2, 4/9), a: -4/9, b: 2/3, minx: 0, miny: 2/3], [(1/2, 2/3) - (1, 3/4), a: 1/6, b: 7/12, minx: 1/2, miny: 2/3]"));
		Assume((rec_maxfunc< list<
		                 segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					     segment< P< LR<3,4>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result::compare<
					     list< segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					           segment< P< LR<3,4>, LR<2,3> >, P< LR<1>, LR<3,4> > > >
					     >::result == 0),
						 ("[(1/2, 2/3) - (3/4, 1/3), a: -4/3, b: 4/3, minx: 1/2, miny: 2/3], [(3/4, 2/3) - (1, 3/4), a: 1/3, b: 5/12, minx: 3/4, miny: 2/3]"));
		Assume((rec_maxfunc< list< segment< P< LR<0>, LR<0> >, P< LR<2,5>, LR<1> > >,
		                 segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					     segment< P< LR<3,4>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result::compare<
					     list< segment< P< LR<0>, LR<0> >, P< LR<2,5>, LR<1> > >,
					     	   segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					     	   segment< P< LR<3,4>, LR<2,3> >, P< LR<1>, LR<3,4> > > >
					     >::result == 0),
						("[(0, 0) - (2/5, 1), a: 5/2, b: 0, minx: 0, miny: 0], [(1/2, 2/3) - (3/4, 1/3), a: -4/3, b: 4/3, minx: 1/2, miny: 2/3], [(3/4, 2/3) - (1, 3/4), a: 1/3, b: 5/12, minx: 3/4, miny: 2/3]"));

		Assume((rec_maxfunc< list< segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > >,
						   segment< P< LR<1,2>, LR<0> >, P< LR<3,2>, LR<1> > >,
						   segment< P< LR<1>, LR<1> >, P< LR<2>, LR<0> > >,
						   segment< P< LR<3,2>, LR<1> >, P< LR<5,2>, LR<0> > > > >::result::compare<
						   list< segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > > ,
						         segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >,
						         segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >,
						         segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > > >
					  	   >::result == 0),
						 ("[(0, 0) - (1, 1), a: 1, b: 0, minx: 0, miny: 0], [(1, 1) - (5/4, 3/4), a: -1, b: 2, minx: 1, miny: 1], [(5/4, 3/4) - (3/2, 1), a: 1, b: -1/2, minx: 5/4, miny: 3/4], [(3/2, 1) - (5/2, 0), a: -1, b: 5/2, minx: 3/2, miny: 1]"));

		Assume((rec_maxfunc< mseg2::segments::sort >::result::compare<
				list< segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > >,
				      segment< P< LR< 1,1>, LR<1,1> >, P< LR< 3,2>, LR<1,2> > >,
				      segment< P< LR< 3,2>, LR<1,2> >, P< LR< 2,1>, LR<1,1> > >,
				      segment< P< LR< 2,1>, LR<1,1> >, P< LR< 3,1>, LR<0,1> > > >
				>::result == 0),
		("[(0, 0) - (1, 1), a: 1, b: 0, minx: 0, miny: 0], [(1, 1) - (3/2, 1/2), a: -1, b: 2, minx: 1, miny: 1], [(3/2, 1/2) - (2, 1), a: 1, b: -1, minx: 3/2, miny: 1/2], [(2, 1) - (3, 0), a: -1, b: 3, minx: 2, miny: 1]"));

		Assume((rec_maxfunc< comb<1,5>::segments::sort >::result::compare< list<
					  segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > >,
					  segment< P< LR< 1,1>, LR<1,1> >, P< LR< 3,2>, LR<1,2> > >,
					  segment< P< LR< 3,2>, LR<1,2> >, P< LR< 2,1>, LR<1,1> > >,
					  segment< P< LR< 2,1>, LR<1,1> >, P< LR< 5,2>, LR<1,2> > >,
					  segment< P< LR< 5,2>, LR<1,2> >, P< LR< 3,1>, LR<1,1> > >,
					  segment< P< LR< 3,1>, LR<1,1> >, P< LR< 7,2>, LR<1,2> > >,
					  segment< P< LR< 7,2>, LR<1,2> >, P< LR< 4,1>, LR<1,1> > >,
					  segment< P< LR< 4,1>, LR<1,1> >, P< LR< 5,1>, LR<0,1> > >
				> >::result == 0),
		("[(0, 0) - (1, 1), a: 1, b: 0, minx: 0, miny: 0], [(1, 1) - (3/2, 1/2), a: -1, b: 2, minx: 1, miny: 1], [(3/2, 1/2) - (2, 1), a: 1, b: -1, minx: 3/2, miny: 1/2], [(2, 1) - (5/2, 1/2), a: -1, b: 3, minx: 2, miny: 1], [(5/2, 1/2) - (3, 1), a: 1, b: -2, minx: 5/2, miny: 1/2], [(3, 1) - (7/2, 1/2), a: -1, b: 4, minx: 3, miny: 1], [(7/2, 1/2) - (4, 1), a: 1, b: -3, minx: 7/2, miny: 1/2], [(4, 1) - (5, 0), a: -1, b: 5, minx: 4, miny: 1]"));

		Assume((maxfunc< s3, s2::suffix<s3::minx> >::result::compare< list<
					  segment< P< LR< 0,1>, LR<1,1> >, P< LR< 48,41>, LR<33,41> > >,
					  segment< P< LR< 48,41>, LR<33,41> >, P< LR< 3,1>, LR<3,1> > >,
					  segment< P< LR< 3,1>, LR<1,2> >, P< LR< 6,1>, LR<0,1> > >
				> > ::result == 0),
		("[(0, 1) - (48/41, 33/41), a: -1/6, b: 1, minx: 0, miny: 1], [(48/41, 33/41) - (3, 3), a: 6/5, b: -3/5, minx: 48/41, miny: 33/41], [(3, 1/2) - (6, 0), a: -1/6, b: 1, minx: 3, miny: 1/2]"));

		// a maxfunc of a case where a singularity filter would need to remove a singularity
		Assume((maxfunc< segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >,
		segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > > >::result::compare< list<
			segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,1>, LR<1,1> > >
			 > > ::result == 0),
		("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,1>, LR<1,1> > >"));


		typedef rec_maxfunc< list< segment< P< LR<0>, LR<0> >, P< LR<2,5>, LR<1> > >,
		                 segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
					     segment< P< LR<3,4>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result rmxf1;

		typedef rec_maxfunc< list< segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > >,
						   segment< P< LR<1,2>, LR<0> >, P< LR<3,2>, LR<1> > >,
						   segment< P< LR<1>, LR<1> >, P< LR<2>, LR<0> > >,
						   segment< P< LR<3,2>, LR<1> >, P< LR<5,2>, LR<0> > > > >::result rmxf2;

		typedef list< segment< P< LR<2>, LR<0> >, P< LR<5,2>, LR<1> > >, segment< P< LR<5,2>, LR<1> >, P< LR<3>, LR<0> > > > f3;

		Assume((rmxf1::compare< list<
				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 3,4>, LR<1,3> > >, segment< P< LR< 3,4>, LR<2,3> >, P< LR< 1,1>, LR<3,4> > >
				> >::result == 0),
				("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 3,4>, LR<1,3> > >, segment< P< LR< 3,4>, LR<2,3> >, P< LR< 1,1>, LR<3,4> > >"));
		Assume((rmxf2::compare< list<
				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >
				> >::result == 0),
				("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >"));
//		typedef rmxf1::concat< rmxf2 >::sort srt;
//		srt::print(cout);
//		typedef list< segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >,
//				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 1,1>, LR<1,1> > >//,
//				segment< P< LR< 1,2>, LR<2,3> >, P< LR< 3,4>, LR<1,3> > >,
//				segment< P< LR< 3,4>, LR<2,3> >, P< LR< 1,1>, LR<3,4> > >,
//				segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >,
//				segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >,
				//segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >
//		> srt_;

//		typedef rec_maxfunc< srt_ >::result rmxf;
//		rmxf::print(cout);
		Assume((two_way_maxfunc< rmxf1, rmxf2 >::result::compare< list<
				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >
				> >::result == 0),
				("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >"));


		Assume((multi_segment_maxfunc< list< multi_segment< rmxf1 >, multi_segment< rmxf2 > > >::result::segments::compare< list<
				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >
				> >::result == 0),
				("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 5,2>, LR<0,1> > >"));

		Assume((multi_segment_maxfunc< list< multi_segment< rmxf1 >, multi_segment< rmxf2 >, multi_segment< f3 > > >::result::segments::compare< list<
				segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 13,6>, LR<1,3> > >, segment< P< LR< 13,6>, LR<1,3> >, P< LR< 5,2>, LR<1,1> > >, segment< P< LR< 5,2>, LR<1,1> >, P< LR< 3,1>, LR<0,1> > >
				> >::result == 0),
				("segment< P< LR< 0,1>, LR<0,1> >, P< LR< 2,5>, LR<1,1> > >, segment< P< LR< 2,5>, LR<2,5> >, P< LR< 1,2>, LR<1,2> > >, segment< P< LR< 1,2>, LR<2,3> >, P< LR< 4,7>, LR<4,7> > >, segment< P< LR< 4,7>, LR<4,7> >, P< LR< 1,1>, LR<1,1> > >, segment< P< LR< 1,1>, LR<1,1> >, P< LR< 5,4>, LR<3,4> > >, segment< P< LR< 5,4>, LR<3,4> >, P< LR< 3,2>, LR<1,1> > >, segment< P< LR< 3,2>, LR<1,1> >, P< LR< 13,6>, LR<1,3> > >, segment< P< LR< 13,6>, LR<1,3> >, P< LR< 5,2>, LR<1,1> > >, segment< P< LR< 5,2>, LR<1,1> >, P< LR< 3,1>, LR<0,1> > >"));

		return true;
	}
};



#endif /* GEOMETRY_TEST_H_ */
