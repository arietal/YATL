/*
 * bst.h
 *
 *  Created on: Sep 5, 2012
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

#ifndef BST_H_
#define BST_H_

#include <iostream>
using namespace std;

#include "list.h"

using namespace yatl::List;

namespace yatl {

template <typename kType, typename dType>
struct mapping {
	typedef kType key;
	typedef dType data;
	typedef mapping o_type;
	template <typename pair>
	struct compare : public key::template compare<typename pair::key> {};
};

template <typename kType, typename dType>
ostream& operator<<(ostream &os, const mapping<kType,dType>& m) {
	return os << kType() << " -> " << dType() << endl;
}

// for integer keys
template <int k, typename dType>
struct imap : public mapping< I<k>, dType > {};

// for integer keys and data
template <int k, int d>
struct iimap : public mapping< I<k>, I<d> > {};

struct _nil_node {
	typedef _NIL data;
	typedef _NIL key;
	template <typename Key>
	struct find_ {
		typedef _NIL result;
	};

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1) {
		return false;
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, const typename functor::arg2Type& a2) {
		return false;
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, const typename functor::returnType& ret) {
		return false;
	}

};

ostream& operator<<(ostream& os, const _nil_node& nil) {
	return os << "(nil)" << endl;
}

template <typename kType, typename dType, typename leftNodeType=_nil_node, typename rightNodeType=_nil_node, int depth_=0>
struct node {
	typedef kType key;
	typedef dType data;
	typedef leftNodeType left;
	typedef rightNodeType right;
	enum { depth = depth_ };
	template <typename Key>
	struct find_ {
		typedef typename if_else< (Key::template compare<key>::result == 0),
				data,
				  typename if_else< (Key::template compare<key>::result < 0),
				  	  typename left::template find_<Key>::result,
				  	  typename right::template find_<Key>::result >::type >::type result;
	};

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1) {
		int cmp = functor::arg1Type::template compareConst<key>(a1);
		if (cmp == 0)
			return functor::template execute<data>(a1);
		if (cmp < 0)
			return left::template searchAndExecute<functor>(a1);
		return right::template searchAndExecute<functor>(a1);
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, const typename functor::arg2Type& a2) {
		int cmp = functor::arg1Type::template compareConst<key>(a1);
		if (cmp == 0)
			return functor::template execute<data>(a1,a2);
		if (cmp < 0)
			return left::template searchAndExecute<functor>(a1,a2);
		return right::template searchAndExecute<functor>(a1,a2);
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, typename functor::returnType& ret) {
		int cmp = functor::arg1Type::template compareConst<key>(a1);
		if (cmp == 0)
			return functor::template execute<data>(a1,ret);
		if (cmp < 0)
			return left::template searchAndExecute<functor>(a1,ret);
		return right::template searchAndExecute<functor>(a1,ret);
	}



};

template <typename kType, typename dType, typename leftNodeType, typename rightNodeType, int depth>
ostream& operator<<(ostream& os, const node<kType,dType,leftNodeType,rightNodeType,depth>& n) {
	typedef node<kType,dType,leftNodeType,rightNodeType,depth> nodeType;
	os << "(" << typename nodeType::key() << ", " << typename nodeType::data() << ")" << endl;
	for (int i=0; i < depth+1; i++)
		os << "  ";
	os << leftNodeType();
	for (int i=0; i < depth+1; i++)
		os << "  ";
	os << rightNodeType();
	return os;
}

template <typename sortedList, int startIndex=0, int endIndex=sortedList::size, int depth=0>
struct bst_builder;

template <typename rootNode>
struct bst {
	typedef rootNode root;

	template <typename Key>
	struct find_ : public root::template find_<Key>::result {};

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1) {
		return root::template searchAndExecute<functor>(a1);
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, const typename functor::arg2Type& a2) {
		return root::template searchAndExecute<functor>(a1, a2);
	}

	template <typename functor>
	static bool searchAndExecute(const typename functor::arg1Type& a1, typename functor::returnType& ret) {
		return root::template searchAndExecute<functor>(a1, ret);
	}




	static void print() {
		cout << root() << endl;
	}
};


//template <typename func>
//struct bst_functor {
//	typedef lambdaRet1Var1Type<bool,Rational,false, func> lambda;
//};



template <typename sortedList, int startIndex, int endIndex, int depth>
struct bst_builder {
	enum { midIndex = startIndex+ (endIndex-startIndex)/2 };
	typedef typename sortedList::template elementAt<midIndex> midPair;
	typedef bst< node< typename midPair::key, typename midPair::data,
			typename bst_builder< sortedList, startIndex, midIndex, depth+1 >::result::root,
			typename bst_builder< sortedList, midIndex+1, endIndex, depth+1 >::result::root, depth> > result;
};

template <int startIndex, int endIndex, int depth>
struct bst_builder<list<_NIL>, startIndex, endIndex, depth > {
	typedef bst<_nil_node> result;
};

template <typename sortedList, int sIndex, int depth>
struct bst_builder<sortedList, sIndex, sIndex, depth> {
	typedef bst<_nil_node> result;
};

}

#endif /* BST_H_ */
