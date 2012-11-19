/*
 * list.h
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

#ifndef LIST_H_
#define LIST_H_

#include <iostream>
using namespace std;

#include "base.h"

namespace yatl {

namespace List {

#define DPX(ty, d,def) ty p##d##0 def, ty p##d##1 def, ty p##d##2 def, ty p##d##3 def, \
		ty p##d##4 def, ty p##d##5 def, ty p##d##6 def, ty p##d##7 def, ty p##d##8 def, ty p##d##9 def

#define DPARAMS ,DPX(typename, 1, =_NIL), DPX(typename, 2, =_NIL)
//, DPX(typename, 3, =_NIL), DPX(typename, 4, =_NIL), DPX(typename, 5, =_NIL)
#define TPARAMS ,DPX(typename, 1,), DPX(typename, 2,)
//, DPX(typename, 3,), DPX(typename, 4,), DPX(typename, 5,)
#define PARAMS ,DPX(,1,), DPX(,2,)
//, DPX(,3,), DPX(,4,), DPX(,5,)

#define DEF_PARAM_LIST typename p3=_NIL, typename p4=_NIL, typename p5=_NIL, \
		typename p6=_NIL, typename p7=_NIL, typename p8=_NIL, typename p9=_NIL DPARAMS

#define TYPENAME_PARAM_LIST typename p3, typename p4, typename p5, \
		typename p6, typename p7, typename p8, typename p9 TPARAMS


#define PARAM_LIST p2,p3,p4,p5,p6,p7,p8,p9 PARAMS


ostream& operator<<(ostream& os, const _NIL& nil) {
	return os << "nil";
}

template <typename r>
struct print {
	static void scan(ostream& os=cout) {
		os << r();
	}
	static void pre(ostream& os=cout) {}
	static void post(ostream& os=cout) { os << ", "; }
	static void postScan(ostream& os=cout) { os << endl; }
};


template <typename r>
struct _nilFilter {
	enum { result = 1 } ;
};

template <typename r>
struct _identity {
	typedef r result;
};



template<typename l, template <typename r> class scanner>
struct each;

template <typename l>
struct sort;


template <typename l, template <typename r1, typename r2> class compare>
struct sort_;

template <typename l, template <typename r> class mapper>
struct collect;

template <typename l, template <typename r> class filt>
struct filter;

template <typename l, typename res>
struct reverse;

template <typename l1, typename l2>
struct concat;

template <typename l, int idx>
struct elementAt;

template <typename l, typename r, int idx>
struct indexOf;

template <typename l, typename r, int idx, int lastIdx>
struct lastIndexOf;

template <typename l1, typename l2>
struct equals;

template <typename l1, typename l2>
struct compare;

template <typename l, int startIdx, int endIdx, int idx>
struct sublist;

template <typename l>
struct to_a;


template <typename node, typename next=_NIL>
struct list_ {
	typedef node Node;
	typedef next Next;

	enum { size = 1 + Next::size };
};

#define __list_methods \
\
enum { size = List_::size }; \
\
template<template <typename r> class scanner> \
static void each() { \
	List::each<List_,scanner>::scan(); \
} \
template<template <typename r> class scanner, typename p1_> \
static void each(p1_ &o1) { \
	List::each<List_, scanner>::scan(o1); \
} \
template<template <typename r> class scanner, typename p1_> \
static void each(const p1_ &o1) { \
	List::each<List_, scanner>::scan(o1); \
} \
\
static typename List_::Node::o_type *toArray() {\
	typename List_::Node::o_type *a = (typename List_::Node::o_type *)new char[sizeof(typename List_::Node::o_type)*List_::size];\
	List::to_a<List_>::init(a);\
	return a;\
}\
struct sort : public list<typename List::sort<List_>::List> {}; \
\
template <template <typename r1, typename r2> class compare> \
struct sort_ : public list<typename List::sort_<List_, compare>::List> {}; \
\
template <template <typename r1> class mapper> \
struct collect : public list<typename List::collect<List_, mapper>::List> {}; \
\
template <template <typename r1> class filt> \
struct filter : public list<typename List::filter<List_, filt>::List> {}; \
\
typedef list<typename List::reverse<List_, _NIL>::List> reverse; \
\
template <typename l2> \
struct concat : public list<typename List::concat<List_, typename l2::List_>::List> {}; \
\
template <int idx> \
struct elementAt : public List::elementAt<List_, idx>::result {}; \
\
template <typename r> \
struct indexOf : public List::indexOf<List_, r, 0> {}; \
\
template <typename r> \
struct lastIndexOf : public List::lastIndexOf<List_, r, 0, -1> {}; \
\
template <typename l2> \
struct equals : public List::equals<List_, typename l2::List_> {}; \
\
template <typename l2> \
struct compare : public List::compare<List_, typename l2::List_> {}; \
\
template <int startIdx, int endIdx> \
struct sublist : public list< typename List::sublist<List_, startIdx, endIdx, 0>::List> {}; \
\
static void print(ostream& os=cout) { each<List::print>(os); }




template <typename p1, typename p2=_NIL, DEF_PARAM_LIST>
struct list {
	typedef list_<p1, typename list<PARAM_LIST>::List_> List_;

	__list_methods

};

template <typename node, typename next>
struct list<list_<node,next> > {
	typedef list_<node,next> List_;

	__list_methods
};

template<>
struct list<_NIL> {
	typedef _NIL List_;

	enum { size = List_::size };



	template<template <typename r> class scanner >
	static void each() {
		scanner<_NIL>::postScan();
	}

	typedef list<_NIL> sort;

//	template <template <typename r1, typename r2> class compare>
//	struct sort_ : public list<_NIL> {};
//
//	template <template <typename r1> class mapper>
//	struct collect : public list<_NIL> {};
//
//	template <template <typename r1> class filt>
//	struct filter : public list<_NIL> {};

	typedef list<_NIL> reverse;

	template <typename l2>
	struct concat : public l2 {};

	template <int idx>
	struct elementAt : public _NIL {};

	template <typename r>
	struct indexOf : public List::indexOf<List_, r, 0> {};

	template <typename r>
	struct lastIndexOf : public List::lastIndexOf<List_, r, 0, -1> {};

	template <typename l2>
	struct equals : public List::equals<List_, typename l2::List_> {};

	template <typename l2>
	struct compare : public List::compare<List_, typename l2::List_> {};

//	template <int startIdx, int endIdx>
//	struct sublist : public list<_NIL> {};


	static void print() { each<List::print>(); }
};

template <typename l>
struct sort {
	typedef typename sort<typename l::Next>::newHead nextMin;
	typedef typename sort<typename l::Next>::newTail nextTail;
	typedef typename if_else<(l::Node::template compare<nextMin>::result < 0),
            typename l::Node, nextMin
            >::type newHead;
	typedef typename sort <
		    typename if_else<(l::Node::template compare<nextMin>::result < 0),
		 	   list_<nextMin, nextTail>,
		 	   list_<typename l::Node, nextTail> >::type
		>::List newTail;

	typedef list_<newHead, newTail> List;
};

template <typename p1, typename p2>
struct sort< list_<p1, list_<p2, _NIL> > > {
	typedef typename if_else<(p1::template compare<p2>::result < 0), p1, p2>::type newHead;
	typedef list_<typename if_else<(p1::template compare<p2>::result < 0), p2, p1>::type, _NIL> newTail;
	typedef list_<newHead, newTail> List;
};

template <typename p1>
struct sort< list_<p1, _NIL> > {
	typedef int a[-p1::denominator]; // assert
	typedef p1 newHead;
	typedef _NIL newTail;
	typedef list_<p1, _NIL> List;
};

template<>
struct sort<_NIL > {
	typedef _NIL List;
};


template <typename l, template <typename r1, typename r2> class compare>
struct sort_ {
	typedef typename sort_<typename l::Next,compare>::newHead nextMin;
	typedef typename sort_<typename l::Next,compare>::newTail nextTail;
	typedef typename if_else<(compare<typename l::Node, nextMin>::result < 0),
            typename l::Node, nextMin
            >::type newHead;
	typedef typename sort_ <
		    typename if_else<(compare<typename l::Node, nextMin>::result < 0),
		 	   list_<nextMin, nextTail>,
		 	   list_<typename l::Node, nextTail> >::type, compare
		>::List newTail;

	typedef list_<newHead, newTail> List;
};

template <typename p1, typename p2, template<typename r1, typename r2> class compare>
struct sort_< list_<p1, list_<p2, _NIL> >, compare > {
	typedef typename if_else<(compare<p1, p2>::result < 0), p1, p2>::type newHead;
	typedef list_<typename if_else<(compare<p1, p2>::result < 0), p2, p1>::type, _NIL> newTail;
	typedef list_<newHead, newTail> List;
};

template <typename p1, template <typename r1, typename r2> class compare>
struct sort_< list_<p1, _NIL>, compare > {
	typedef int a[-p1::denominator];
	typedef p1 newHead;
	typedef _NIL newTail;
	typedef list_<p1, _NIL> List;
};

template<template <typename r1, typename r2> class compare>
struct sort_<_NIL, compare > {
	typedef _NIL List;
};



template<typename l, template <typename r> class scanner>
struct each {
	static void scan() {
		scanner<typename l::Node>::pre();
		scanner<typename l::Node>::scan();
		scanner<typename l::Node>::post();
		each< typename l::Next, scanner>::scan();
	}
	template <typename p1>
	static void scan(p1 &o1) {
		scanner<typename l::Node>::pre(o1);
		scanner<typename l::Node>::scan(o1);
		scanner<typename l::Node>::post(o1);
		each< typename l::Next, scanner>::scan(o1);
	}
	template <typename p1>
	static void scan(const p1 &o1) {
		scanner<typename l::Node>::pre(o1);
		scanner<typename l::Node>::scan(o1);
		scanner<typename l::Node>::post(o1);
		each< typename l::Next, scanner>::scan(o1);
	}
};

template <typename p, template <typename r> class scanner>
struct each< list_< p, _NIL>, scanner > {
	static void scan() {
		scanner<p>::pre();
		scanner<p>::scan();
		scanner<p>::postScan();
	}
	template <typename p1>
	static void scan(p1 &o1) {
		scanner<p>::pre(o1);
		scanner<p>::scan(o1);
		scanner<_NIL>::postScan(o1);
	}
	template <typename p1>
	static void scan(const p1 &o1) {
		scanner<p>::pre(o1);
		scanner<p>::scan(o1);
		scanner<_NIL>::postScan(o1);
	}
};

template <template <typename r> class scanner>
struct each<_NIL, scanner> {
	static void scan() {}
	template <typename p1>
	static void scan(p1 &o1) {
		scanner<_NIL>::postScan(o1);
	}
	template <typename p1>
	static void scan(const p1 &o1) {
		scanner<_NIL>::postScan(o1);
	}
};

template <typename l, template <typename r> class mapper>
struct collect {
	typedef typename mapper<typename l::Node>::result Node;
	typedef typename collect<typename l::Next, mapper>::List Next;
	typedef list_<Node, Next> List;
};

template <template <typename r> class mapper>
struct collect<_NIL, mapper> {
	typedef _NIL List;
};

template <typename l, template <typename r> class filt>
struct filter {
	typedef typename if_else< (filt<typename l::Node>::result == 1),
			list_< typename l::Node, typename filter< typename l::Next, filt>::List >,
			typename filter< typename l::Next, filt >::List>::type List;
};


template <template <typename r> class filt>
struct filter<_NIL, filt> {
	typedef _NIL List;
};



template <typename l, typename res=_NIL>
struct reverse {
	typedef typename reverse<typename l::Next, list_<typename l::Node, res> >::List List;
};

template <typename res>
struct reverse<_NIL, res > {
	typedef res List;
};

template <typename l1, typename l2>
struct concat {
	typedef list_<typename l1::Node, typename concat<typename l1::Next, l2>::List> List;
};

template <typename l2>
struct concat<_NIL, l2> {
	typedef l2 List;
};


template <typename l, int idx>
struct elementAt {
	typedef typename elementAt<typename l::Next, idx-1>::result result;
};

template <typename l>
struct elementAt<l,0> {
	typedef typename l::Node result;
};

template <typename l, typename r, int idx=0>
struct indexOf {
	enum { result = (same_type<typename l::Node, r>::result == 1) ? idx : indexOf<typename l::Next, r, idx+1>::result } ;
};

template <typename r, int idx>
struct indexOf<_NIL, r, idx> {
	enum { result = -1 };
};


template <typename l, typename r, int idx=0, int lastIdx=-1>
struct lastIndexOf {
	enum { result = lastIndexOf<typename l::Next, r, idx+1, (same_type<typename l::Node, r>::result == 1) ? idx : lastIdx>::result };
};

template <typename r, int idx, int lastIdx>
struct lastIndexOf<_NIL, r, idx, lastIdx> {
	enum { result = lastIdx };
};



template <typename l1, typename l2>
struct compareSorted {
	enum { result = l1::Node::template compare<typename l2::Node>::result == 0 &&
		             compareSorted<typename l1::Next, typename l2::Next>::result == 1 };
};

template <>
struct compareSorted<_NIL, _NIL> {
	enum { result = 1 };
};

template <typename n1, typename n2>
struct compareSorted<_NIL, list_<n1, n2> > {
	enum { result = 0 };
};

template <typename n1, typename n2>
struct compareSorted<list_<n1, n2>, _NIL> {
	enum { result = 0 };
};




template <typename l1, typename l2>
struct equals {
	enum { result = compareSorted<typename List::sort<l1>::List,
		typename List::sort<l2>::List>::result };
};


template <typename l1, typename l2>
struct compare {
	enum { result = (l1::Node::template compare<typename l2::Node>::result != 0) ?
					(int)l1::Node::template compare<typename l2::Node>::result :
		            (int)compare<typename l1::Next, typename l2::Next>::result
	};
};

template <>
struct compare<_NIL, _NIL> {
	enum { result = 0 };
};

template <typename n1, typename n2>
struct compare<_NIL, list_<n1, n2> > {
	enum { result = -1 };
};

template <typename n1, typename n2>
struct compare<list_<n1, n2>, _NIL> {
	enum { result = 1 };
};

template <typename l, int startIdx, int endIdx, int idx=0>
struct sublist {
	typedef typename if_else<(idx >= startIdx && idx < endIdx),
			list_<typename l::Node, typename sublist<typename l::Next, startIdx, endIdx, idx+1>::List>,
			typename if_else<(idx < startIdx), typename sublist<typename l::Next, startIdx, endIdx, idx+1>::List,
								_NIL>::type >::type List;
};

template <int startIdx, int endIdx, int idx>
struct sublist<_NIL, startIdx, endIdx, idx> {
	typedef _NIL List;
};

template <typename l>
struct to_a {
	static void init(typename l::Node::o_type* a) {
		l::Node::to_o(a);
		to_a<typename l::Next>::init(a+1);
	}
};

template <typename p>
struct to_a<list_<p, _NIL> > {
	static void init(typename p::o_type* a) {
		p::to_o(a);
	}
};

}

}

#endif /* LIST_H_ */
