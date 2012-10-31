/*
 * geometry.h
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

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "composite_types.h"
#include "../tmp/list.h"

using namespace yatl;
using namespace yatl::List;

namespace yatl {

/**
 * Represent a point in a 2D space
 */
template <typename rx, typename ry>
struct point {
	typedef rx x;
	typedef ry y;

	/**
	 * lexicographic compare of two points, with the x dimension as the primary one
	 */
	template <typename p2>
	struct compare {
		static const int c1 = x::template compare<typename p2::x>::result;
		static const int c2 = y::template compare<typename p2::y>::result;
		static const int result = (c1 != 0) ? c1 : c2;
	};
};

template <typename rx, typename ry>
ostream &operator<<(ostream &os, const point<rx,ry>& p) {
	return os << "(" << rx() << ", " << ry() << ")";
}

/**
 * The P class is the one that should be used (instead of using point directly). It also enables adding points
 * to a list, and converting such list to an array, etc.
 */
template <typename r1, typename r2>
struct P : public point<r1, r2> {
	typedef point<typename r1::o_type, typename r2::o_type> o_type;
};

/**
 * A segment is described as the line connecting two points.
 *
 * Assume p1::x != p2::x (for the time being)
 *
 * For each segment, calculate the "line" function (i.e. a*x + b)
 */
template <typename p1_, typename p2_>
struct segment {
	typedef segment<p1_, p2_> this_;
	typedef p1_ p1;
	typedef p2_ p2;
	typedef min_op<typename p1::x, typename p2::x> minx;
	typedef max_op<typename p1::x, typename p2::x> maxx;
	typedef typename if_else< lt_op<typename p1::x, typename p2::x>::result, typename p1::y, typename p2::y>::type minx_y;
	typedef typename if_else< gt_op<typename p1::x, typename p2::x>::result, typename p1::y, typename p2::y>::type maxx_y;
	typedef P< minx, minx_y > minp;
	typedef P< maxx, maxx_y > maxp;
	typedef typename div_op< sub_op<maxx_y, minx_y>, sub_op<maxx, minx> >::simplify a;
	typedef typename sub_op< minx_y, mpy_op< a, minx > >::simplify b;

	/**
	 * Calculate the y value at a given x
	 */
	template <typename x>
	struct at : public add_op< b,
				   mpy_op< a, x > >::simplify {};

	/**
	 * The dynamic version of 'at' that evaluates the line function at a given x
	 */
	static typename p1::o_type eval(const typename p1::o_type& x) {
		return a()*x + b();
	}

	/**
	 * Test whether a given x is in the x-range of a segment (i.e. between minx and maxx)
	 */
	template <typename x>
	struct x_in_range {
		static const bool result = ge_op< x, minx >::result && le_op< x, maxx >::result;
	};

	/**
	 * Derive a sub-segment from minx to a given x
	 */
	template <typename x>
	struct prefix : public segment< minp, P< x, at<x> > > {};

	/**
	 * Derive a sub-segment from a given x to maxx
	 */
	template <typename x>
	struct suffix : public segment< P< x, at<x> >, maxp > {};

	/**
	 * Derive a subsegment from a given x1 to a given x2
	 */
	template <typename x1, typename x2>
	struct subsegment : public segment< P< x1, at<x1> >, P< x2, at<x2> > > {};

	/**
	 * Test whether the segment overlaps segment seg2.
	 *
	 * That is, whether the x-range of this_ segment and the x-range of seg2 overlap.
	 */
	template <typename seg2>
	struct overlaps {
		static const bool result = le_op< typename seg2::minx, maxx>::result && ge_op< typename seg2::maxx, minx>::result;
	};

	/**
	 * Test whether the segment completely overlaps segment seg2.
	 *
	 * That is, whether seg2::minx >= minx and seg2::maxx <= maxx
	 */
	template <typename seg2>
	struct completelyOverlaps {
		static const bool result = ge_op< typename seg2::minx, minx >::result && le_op< typename seg2::maxx, maxx >::result;
	};

	/**
	 * Test whether the segment completely overlaps segment seg2.
	 *
	 * That is, whether seg2::minx > minx and seg2::maxx < maxx
	 */
	template <typename seg2>
	struct strictlyCompletelyOverlaps {
		static const bool result = gt_op< typename seg2::minx, minx >::result && lt_op< typename seg2::maxx, maxx >::result;
	};

	/**
	 * Test whether seg2 partially overlaps this_ segment on the right side.
	 *
	 * That is, whether seg2::minx is strictly within x-range, and seg2::maxx > maxx.
	 */
	template <typename seg2>
	struct partialOverlapOnRight {
		static const bool result = gt_op< typename seg2::minx, minx>::result && lt_op< typename seg2::minx, maxx>::result &&
				ge_op< typename seg2::maxx, maxx>::result;
	};

	/**
	 * Test whether seg2 partially overlaps this_ segment on the left side.
	 *
	 * That is, whether seg2::maxx is strictly within x-range, and seg2::minx < minxx.
	 */
	template <typename seg2>
	struct partialOverlapOnLeft {
		static const bool result = le_op< typename seg2::minx, minx>::result && gt_op< typename seg2::maxx, minx>::result &&
				lt_op< typename seg2::maxx, maxx>::result;
	};


	template <typename seg2>
	struct overlapAbove {
		static const bool result = (completelyOverlaps<seg2>::result && ge_op<at<typename seg2::minx> , typename seg2::minx_y>::result &&
										ge_op<at<typename seg2::maxx>, typename seg2::maxx_y>::result);
	};


	/**
	 * subtract the range of seg2 from seg1, resulting in a list of zero up to two segments.
	 */
	template <typename seg2>
	struct subtract : public mcond<
				!overlaps<seg2>::result,
					list< list_<this_> >,
				strictlyCompletelyOverlaps<seg2>::result,
					list< prefix<typename seg2::minx>, suffix<typename seg2::maxx> >,
				seg2::template completelyOverlaps<this_>::result,
					list< _NIL >,
				partialOverlapOnRight<seg2>::result,
					list< prefix<typename seg2::minx> >,
				partialOverlapOnLeft<seg2>::result,
					list< suffix<typename seg2::maxx> >,
				true, // should not reach this point, will likely generate errors
					list< LR<-1> >
		>::type {};

	template <typename seg2>
	struct above {
		static const bool result = overlapAbove<seg2>::result  ||
								   seg2::template overlapAbove<this_>::result ||
								   (overlaps<seg2>::result && ge_op< at<typename seg2::minx>, typename seg2::minx_y>::result && ge_op< maxx_y, typename seg2::template at<maxx> >::result);
	};



	template <typename seg2>
	struct compare : public if_else<(p1::template compare<typename seg2::p1>::result != 0),
												typename p1::template compare<typename seg2::p1>,
												typename p2::template compare<typename seg2::p2> >::type {};

	struct width : public sub_op< maxx, minx > {};

	struct height : public sub_op< maxx_y, minx_y >::abs {};

	struct o_type {
		typename minx::o_type minx;
		typename minx_y::o_type minx_y;
		typename maxx::o_type maxx;
		typename maxx_y::o_type maxx_y;
		typename a::o_type a;
		typename b::o_type b;

		o_type() : minx(minx::o_type()), minx_y(minx_y::o_type()), maxx(maxx::o_type()), maxx_y(maxx_y::o_type()), a(a::o_type()), b(b::o_type()) {}

	};
};

template <typename p1, typename p2>
ostream &operator <<(ostream& os, const segment<p1,p2>& s) {
	return os << "[" << p1() << " - " << p2() << ", a: " << typename segment<p1,p2>::a() << ", b: " << typename segment<p1,p2>::b() << ", minx: "
			<< typename segment<p1,p2>::minx() << ", miny: " << typename segment<p1,p2>::minx_y() << "]";
}


template <typename seg1, typename seg2, typename x>
struct check_in_range {
	const static bool result = seg1::template x_in_range<x>::result && seg2::template x_in_range<x>::result;
};

template <typename seg1, typename seg2>
struct check_in_range<seg1,seg2,_NIL> {
	const static bool result = false;
};

template <typename seg1, typename seg2>
struct seg_intersect {
	const static bool parallel = seg1::a::template eq<typename seg2::a>::result;
	typedef typename if_else<!parallel, typename div_op< sub_op<typename seg2::b, typename seg1::b>, sub_op<typename seg1::a, typename seg2::a> >::simplify, _NIL>::type x;
	const static bool valid = !parallel && check_in_range<seg1,seg2,x>::result;
};

// assume seg1::minx < seg2::minx (i.e. sorted segments)
template <typename seg1, typename seg2>
struct maxfunc {
	typedef seg_intersect<seg1,seg2> intr;
	typedef segment<typename seg1::p1, typename seg1::p2> seg1_;
	typedef segment<typename seg2::p1, typename seg2::p2> seg2_;
	typedef typename if_else< (intr::valid), typename intr::x, typename seg1::minx>::type splitPoint;
//	typedef typename intr::x splitPoint;

	// build a max list of two non-crossing segments (segments may join at their boundary in the case of split crossing segments),
	// where one completely overlaps the other
	template <typename s1, typename s2>
	struct maxOverlap {
		struct wider : public if_else< ge_op< typename s1::width, typename s2::width >::result, s1, s2 >::type {};
		struct narrower : public if_else< le_op< typename s1::width, typename s2::width >::result, s1, s2 >::type {};

		typedef typename if_else< (eq_op< typename s1::minx, typename s2::minx >::result),
					typename if_else< (eq_op< typename s1::maxx, typename s2::maxx >::result),
						// if both segments start and end at the same x points, we create a list containing the top one
						typename if_else< ( ge_op<typename s1::minx_y, typename s2::minx_y>::result ),
							list_< s1, _NIL >,
							list_< s2, _NIL > >::type,
						// else, both start at the same minx, but one segment is shorter.
						// therefore, there are two cases. Either the longer segment is on top, so the result would be a list containing the longer segment,
						// or the shorter segment is on top, so the result would be a list containing the shorter segment and a suffix of the longer segment.
						typename if_else< (ge_op<typename wider::template at<typename narrower::minx>, typename narrower::minx_y>::result &&
								           ge_op<typename wider::template at<typename narrower::maxx>, typename narrower::maxx_y>::result),
							list_< wider, _NIL >,
							list_< narrower, list_< typename wider::template suffix< typename narrower::maxx >, _NIL > > >::type >::type,
					typename if_else< (eq_op< typename s1::maxx, typename s2::maxx >::result),
						// symmetrically, if the segments have the same maxx
						typename if_else< (ge_op<typename wider::template at<typename narrower::minx>, typename narrower::minx_y>::result &&
										   ge_op<typename wider::template at<typename narrower::maxx>, typename narrower::maxx_y>::result),
							list_< wider, _NIL >,
							list_< typename wider::template prefix<typename narrower::minx>, list_< narrower, _NIL > > >::type,
						// else, both minx and maxx are different, so if the wider is on top, we'll use it as the result, otherwise, we'll have to break it 3-ways
						typename if_else< (ge_op<typename wider::template at<typename narrower::minx>, typename narrower::minx_y>::result &&
										   ge_op<typename wider::template at<typename narrower::maxx>, typename narrower::maxx_y>::result),
							list_< wider, _NIL >,
							list_< typename wider::template prefix<typename narrower::minx>, list_<narrower, list_< typename wider::template suffix< typename narrower::maxx >, _NIL > > > >::type
					>::type
		>::type result;
	};

	template <typename s1, typename s2>
	struct segSplitOverlap {
		typedef typename list< typename maxOverlap< typename s1::template prefix<splitPoint>, typename s2::template prefix<splitPoint> >::result>::
			  	  	  template concat< list< typename maxOverlap< typename s1::template suffix<splitPoint>, typename s2::template suffix<splitPoint> >::result > >::List_ result;
	};

	typedef list<typename mcond<
			// if seg1_ and seg 2 do not overlap, return a list containing both
			!seg1_::template overlaps<seg2_>::result,
				list_<seg1_, list_<seg2_, _NIL> >,
			// if seg1_ completely overlaps seg2_ and is above it, then return seg1_ as the result
			seg1_::template overlapAbove<seg2_>::result,
				list_<seg1_, _NIL>,
			// if seg2_ completely overlaps seg1_ and is above it, then return seg2_ as the result
			seg2_::template overlapAbove<seg1_>::result,
				list_<seg2_, _NIL>,
			// if seg1_ completely overlaps seg2_ and is below seg2_, then compose the result from the prefix of seg1_ up to the point where seg2_ starts, then seg2_, and then the
			// suffix of seg1_ from where seg2_ ends
			!intr::valid && seg1_::template completelyOverlaps<seg2_>::result,
				list_< typename seg1_::template prefix<typename seg2_::minx>, list_<seg2_, list_<typename seg1_::template suffix<typename seg2_::maxx>, _NIL > > >,
			// if seg2_ completely overlaps seg1_ and is below seg1_, then compose the result from the prefix of seg2_ up to the point where seg1_ starts, then seg1_, and then the
			// suffix of seg2_ from where seg1_ ends
			!intr::valid && seg2_::template completelyOverlaps<seg1_>::result,
				list_<typename seg2_::template prefix<typename seg1_::minx>, list_<seg1_, list_<typename seg2_::template suffix<typename seg1_::maxx>, _NIL > > >,
			// if seg1_ and seg2_ cross one another
			!intr::parallel && intr::valid,
				typename segSplitOverlap<seg1_,seg2_>::result,
			// since the segments are assumed to be sorted, assume seg2_ on the right of seg1_
			// so if seg1_ is above seg2_, return seg1_ and the suffix of seg2_ from where seg1_ ends
			seg1_::template above<seg2_>::result,
				list_<seg1_, list_<typename seg2_::template suffix<typename seg1_::maxx>, _NIL> >,
			// otherwise seg2_ is above seg1_, return the prefix of seg1_ up to where seg2_ starts, and seg 2
			true,
				list_< typename seg1_::template prefix<typename seg2_::minx>, list_<seg2_, _NIL> >
			>::type > result;
};


template <typename segments>
struct rec_maxfunc {
	typedef typename rec_maxfunc< list< typename segments::List_ > >::result result;
};

template <typename seg1, typename seg2, typename next>
struct rec_maxfunc< list< list_<seg1, list_<seg2, next > > > > {
	typedef typename maxfunc<seg1, seg2>::result::List_::Node mxfNode;
	typedef typename maxfunc<seg1, seg2>::result::List_::Next mxfNext;
	typedef list< typename list< mxfNext >::template concat< list< next > >::List_ > nextSegments;
	typedef list< list_< mxfNode, typename rec_maxfunc< nextSegments >::result::List_ > > result;
};

template <typename seg1, typename seg2>
struct rec_maxfunc< list<seg1, seg2> > {
	typedef list<typename maxfunc<seg1, seg2>::result::List_> result;
};

template <typename seg1, typename seg2>
struct rec_maxfunc< list<list_<seg1, list_<seg2 > > > > {
	typedef list<typename maxfunc<seg1, seg2>::result::List_> result;
};


template <typename segment>
struct rec_maxfunc<list<list_<segment> >  > {
	typedef list<list_<segment> > result;
};


template <typename l, bool sorted=false>
struct multi_segment {
	typedef typename if_else <sorted,l, typename l::sort>::type segments;
	typedef min_op<typename l::List_::Node::minx, typename multi_segment<list<typename l::List_::Next>, true >::minx> minx;
	typedef max_op<typename l::List_::Node::minx, typename multi_segment<list<typename l::List_::Next>, true >::maxx> maxx;
	template <typename x>
	struct at : public if_else<(l::Node::template x_in_range<x>::result), typename l::Node::template at<x>, typename multi_segment<typename l::Next, true>::template at<x> >::type {};
	template <typename x>
	struct x_in_range : public if_else<(l::Node::template x_in_range<x>::result), typename l::Node::template x_in_range<x>, typename l::Next::template x_in_range<x> >::type {};


};



template<typename seg, bool sorted>
struct multi_segment< list< list_<seg, _NIL> >, sorted>{
	typedef list<list_<seg, _NIL> > segments;
	typedef typename seg::minx minx;
	typedef typename seg::maxx maxx;
	template <typename x>
	struct at : public seg::template at<x> {};
	template <typename x>
	struct x_in_range : public seg::template x_in_range<x> {};

	typedef segments maxfunc;
};

template <typename l, bool sorted>
ostream& operator <<(ostream& os, const multi_segment<l, sorted>& ms) {
	multi_segment<l,sorted>::segments::print(os);
	return os;
}

}

#endif /* GEOMETRY_H_ */
