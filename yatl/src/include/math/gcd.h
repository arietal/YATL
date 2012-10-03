/*
 * gcd.h
 *
 *  Created on: Aug 3, 2012
 *      Author: arietal
 */

#ifndef GCD_H_
#define GCD_H_

#include "../tmp/base.h"

namespace yatl {

template <long v1, long v2>
struct gcd_const {
    gcd_const(long v1_, long v2_) {}
    enum { result = gcd_const<v2, v1 % v2>::result };
    operator long() {
        return result;
    }
};

template <long v1>
struct gcd_const<v1,0> {
    gcd_const(long v1_, long v2_) {}
	enum { result = v1 };
    operator long() {
        return result;
    }
};

template <typename type>
struct VarGCD {
    type result;
    VarGCD(type v1, type v2) : result(0) {
        while (v2 != 0) {
            type t = v1;
            v1 = v2;
            v2 = t % v2;
        }
        if (v1 == 0) v1=1;
        result=v1;
    }
    operator type() {
        return result;
    }
};

template <long v1=0, long v2=0>
struct gcd : public if_else<v2 == 0, VarGCD<long>, gcd_const<v1,v2> >::type {
    gcd(long v1_, long v2_) : if_else<v2 == 0, VarGCD<long>, gcd_const<v1,v2> >::type(v1_,v2_) {}
};

template <long v1, long v2>
struct lcm_const {
	enum { result = (v1/gcd<v1,v2>::result)*v2 };
};

} // namespace

#endif /* GCD_H_ */
