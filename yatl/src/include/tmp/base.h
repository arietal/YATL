/*
 * base.h
 *
 *  Created on: Aug 24, 2012
 *      Author: arietal
 */

#ifndef BASE_H_
#define BASE_H_

namespace yatl {


template<bool cond, typename trueType, typename falseType>
struct if_else {
    typedef trueType type;
};

template<typename trueType, typename falseType>
struct if_else<false,trueType,falseType> {
    typedef falseType type;
};


template <typename Type1, typename Type2>
struct same_type {
	enum { result = 0 };
};

template <typename Type1>
struct same_type<Type1, Type1> {
	enum { result = 1 };
};


template <typename ret, typename arg1, ret nilValue, typename func>
struct lambdaRet1Var1Type {
	typedef ret returnType;
	typedef arg1 arg1Type;
	static const ret nil = nilValue;
	template <typename typeArg>
	static ret execute(arg1Type& a1) { return func::template execute<typeArg>(a1); }
};


struct _NIL {
	enum { size = 0 };
};


/**
 * mcond (multiple conditionals) allows testing a series of conditionals (instead of nesting if_else conditions)
 */
template <bool c1, typename case1,
          bool c2=true, typename case2=_NIL,
          bool c3=true, typename case3=_NIL,
          bool c4=true, typename case4=_NIL,
          bool c5=true, typename case5=_NIL,
          bool c6=true, typename case6=_NIL,
          bool c7=true, typename case7=_NIL,
          bool c8=true, typename case8=_NIL,
          bool c9=true, typename case9=_NIL,
          bool c10=true,typename case10=_NIL>
struct mcond {
    typedef typename mcond<c2,case2,c3,case3,c4,case4,c5,case5,c6,case6,c7,case7,c8,case8,c9,case9,c10,case10,true,_NIL>::type type;
};

template <typename case1, bool c2, typename case2, bool c3, typename case3, bool c4, typename case4, bool c5, typename case5, bool c6, typename case6, bool c7, typename case7,
				bool c8, typename case8, bool c9, typename case9, bool c10, typename case10>
struct mcond<true,case1,c2,case2,c3,case3,c4,case4,c5,case5,c6,case6,c7,case7,c8,case8,c9,case9,c10,case10> {
    typedef case1 type;
};



template <bool cond, template <typename, typename> class t>
struct apply {
	template <typename t1, typename t2>
	struct type : public t<t1,t2> {};
};

template <template <typename, typename> class t>
struct apply<false,t> {
	typedef _NIL type;
};

}

#endif /* BASE_H_ */
