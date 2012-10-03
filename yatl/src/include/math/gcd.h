/*
 * gcd.h
 *
 *  Created on: Aug 3, 2012
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
