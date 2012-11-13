/*
 * composite_types.h
 *
 *  Created on: Sep 16, 2012
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

#ifndef COMPOSIT_TYPES_H_
#define COMPOSIT_TYPES_H_

#include "types.h"
#include "gcd.h"

namespace yatl {

template <typename type, typename varType, type n, type d>
struct RationalLiteralType;


template <typename varType>
struct RationalVarType {
	varType nominator;
	varType denominator;
	typedef varType type;
	typedef RationalLiteralType<typename type::type, RationalVarType, 0, 1> zero;
	typedef RationalLiteralType<typename type::type, RationalVarType, 1, 1> one;

	typedef RationalVarType o_type;

	RationalVarType(const varType& nominator=0, const varType& denominator = 1) : nominator(denominator.sign() * nominator), denominator(denominator.abs()) {}

	template <typename r> RationalVarType add() const { return RationalVarType( nominator*r::denominator + denominator*r::nominator, denominator*r::denominator); }
	template <typename r> RationalVarType sub() const { return RationalVarType( nominator*r::denominator - denominator*r::nominator, denominator*r::denominator); }
	template <typename r> RationalVarType mpy() const { return RationalVarType( nominator*r::nominator, denominator*r::denominator); }
	template <typename r> RationalVarType div() const { return RationalVarType( nominator*r::denominator, denominator*r::nominator); }
	template <typename multiplier, typename addend> RationalVarType ma() const {
		varType mul_n = nominator * multiplier::nominator;
    	varType mul_d = denominator * multiplier::denominator;
    	return RationalVarType(mul_n * addend::denominator + mul_d * addend::nominator, mul_d * addend::denominator);
	}

    template <typename rmin, typename rmax>
    bool in_range() const {
    	typedef lcm_const<rmin::denominator, rmax::denominator> rangelcm;
    	static const typename varType::type factor1_ = rangelcm::result / rmin::denominator;
    	static const typename varType::type factor2_ = rangelcm::result / rmax::denominator;
    	static const typename varType::type rmin_n = rmin::nominator * factor1_;
    	static const typename varType::type rmax_n = rmax::nominator * factor2_;
    	typename varType::type lcmnominator = nominator.val_ * rangelcm::result;
    	return lcmnominator >= rmin_n*denominator && lcmnominator <= rmax_n*denominator;
    }


	// TODO: mod

	template <typename r> bool eq() const { return nominator*r::denominator == denominator*r::nominator; }
	template <typename r> bool ne() const { return nominator*r::denominator != denominator*r::nominator; }
	template <typename r> bool lt() const { return nominator*r::denominator < denominator*r::nominator; }
	template <typename r> bool le() const { return nominator*r::denominator <= denominator*r::nominator; }
	template <typename r> bool gt() const { return nominator*r::denominator > denominator*r::nominator; }
	template <typename r> bool ge() const { return nominator*r::denominator >= denominator*r::nominator; }

	template <typename v> bool fast_lt() const { return nominator.val_ < v::nominator; }
	template <typename v> bool fast_gt() const { return nominator.val_ > v::nominator; }

	template <typename r> RationalVarType max() const { return (ge<r>()) ? *this : RationalVarType(r::nominator, r::denominator); }
	template <typename r> RationalVarType min() const { return (le<r>()) ? *this : RationalVarType(r::nominator, r::denominator); }

	RationalVarType max(const RationalVarType& a) const { return ((*this) >= a) ? *this : a; }
	RationalVarType min(const RationalVarType& a) const { return ((*this) <= a) ? *this : a; }

	RationalVarType operator+(const RationalVarType& r) const { return RationalVarType( nominator*r.denominator + denominator*r.nominator, denominator*r.denominator); }
	RationalVarType operator-(const RationalVarType& r) const { return RationalVarType( nominator*r.denominator - denominator*r.nominator, denominator*r.denominator); }
	RationalVarType operator-() const { return RationalVarType(-nominator, denominator); }
	RationalVarType operator*(const RationalVarType& r) const { return RationalVarType( nominator*r.nominator, denominator*r.denominator); }
	RationalVarType operator/(const RationalVarType& r) const { return RationalVarType( nominator*r.denominator, denominator*r.nominator); }
	// TODO: operator%

	RationalVarType& operator=(const RationalVarType& r) { nominator = r.nominator; denominator = r.denominator; return *this; }
	RationalVarType& operator+=(const RationalVarType& r) { return *this = *this + r; }
	RationalVarType& operator-=(const RationalVarType& r) { return *this = *this - r; }
	RationalVarType& operator*=(const RationalVarType& r) { return *this = *this * r; }
	RationalVarType& operator/=(const RationalVarType& r) { return *this = *this / r; }
	// TODO: operator %=

	bool operator==(const RationalVarType& r) const { return nominator*r.denominator == denominator*r.nominator; }
	bool operator!=(const RationalVarType& r) const { return nominator*r.denominator != denominator*r.nominator; }
	bool operator<(const RationalVarType& r) const { return nominator*r.denominator < denominator*r.nominator; }
	bool operator<=(const RationalVarType& r) const { return nominator*r.denominator <= denominator*r.nominator; }
	bool operator>(const RationalVarType& r) const { return nominator*r.denominator > denominator*r.nominator; }
	bool operator>=(const RationalVarType& r) const { return nominator*r.denominator >= denominator*r.nominator; }

	template <typename r>
	static int compareConst(const RationalVarType &a) {
		int n1 = a.nominator * r::denominator;
		int n2 = a.denominator * r::nominator;
		return (n1 > n2) - (n1 < n2);
	}

	int sign() const {
		return (nominator > 0) - (nominator < 0);
	}

	RationalVarType abs() const { return RationalVarType( (nominator > 0) ? nominator : -nominator, denominator); }

	RationalVarType simplify() const {
		VarGCD<typename varType::type> g(nominator, denominator);
		return RationalVarType( nominator/g, denominator/g);
	}

	operator RationalVarType() const { return *this; }

};

typedef RationalVarType<Short> ShortRational;
typedef RationalVarType<Integer> IntegerRational;
typedef RationalVarType<Long> LongRational;
typedef RationalVarType<LongLong> LongLongRational;

template <typename type>
ostream& operator<<(ostream& os, const RationalVarType<type>& r) {
	os << r.nominator;
	if (r.denominator != type(1)) os << '/' << r.denominator;
	return os;
}

template <typename type, typename varType, type n, type d>
struct RationalLiteralType {
	typedef varType o_type;
	static const type nominator=(d >= 0) ? n : -n;
	static const type denominator=(d >= 0) ? d : -d;

	typedef RationalLiteralType<type, varType, type(1), type(1)> one;
	typedef RationalLiteralType<type, varType, type(0), type(1)> zero;

	template <typename r>
	struct mpy : public RationalLiteralType<type, varType, nominator*r::nominator, denominator*r::denominator> {};
	template <typename r>
	struct add : public RationalLiteralType<type, varType, nominator*r::denominator + denominator*r::nominator, denominator*r::denominator> {};
	template <typename r>
	struct sub : public RationalLiteralType<type, varType, nominator*r::denominator - denominator*r::nominator, denominator*r::denominator> {};
	template <typename r>
	struct div : public RationalLiteralType<type, varType, nominator*r::denominator, denominator*r::nominator> {};

	template <typename r>
	struct max : public if_else< (nominator * r::denominator >= denominator * r::nominator), RationalLiteralType<type, varType, n, d>, r>::type {};

	template <typename r>
	struct min : public if_else< (nominator * r::denominator <= denominator * r::nominator), RationalLiteralType<type, varType, n, d>, r>::type {};

	template <typename r>
	struct eq {
		static const bool result = nominator*r::denominator == denominator*r::nominator;
		operator bool() { return result; }
	};
	template <typename r>
	struct ne {
		static const bool result = nominator*r::denominator != denominator*r::nominator;
		operator bool() { return result; }
	};
	template <typename r>
	struct lt {
		static const bool result = nominator*r::denominator < denominator*r::nominator;
		operator bool() { return result; }
	};
	template <typename r>
	struct le {
		static const bool result = nominator*r::denominator <= denominator*r::nominator;
		operator bool() { return result; }
	};
	template <typename r>
	struct gt {
		static const bool result = nominator*r::denominator > denominator*r::nominator;
		operator bool() { return result; }
	};
	template <typename r>
	struct ge {
		static const bool result = nominator*r::denominator >= denominator*r::nominator;
		operator bool() { return result; }
	};


	template <typename r>
	struct compare {
		static const int result = gt<r>::result - lt<r>::result;
	};

	varType operator+(const varType& r) const { return varType( nominator * r.denominator + denominator * r.nominator, denominator * r.denominator); }
	varType operator-(const varType& r) const { return varType( nominator * r.denominator - denominator * r.nominator, denominator * r.denominator); }

	varType operator*(const varType& r) const { return varType( nominator * r.nominator, denominator * r.denominator); }
	varType operator/(const varType& r) const { return varType( nominator * r.denominator, denominator * r.nominator); }

	bool operator==(const varType& r) const { return nominator*r.denominator == denominator*r.nominator; }
	bool operator!=(const varType& r) const { return nominator*r.denominator != denominator*r.nominator; }
	bool operator<(const varType& r) const { return nominator*r.denominator < denominator*r.nominator; }
	bool operator<=(const varType& r) const { return nominator*r.denominator <= denominator*r.nominator; }
	bool operator>(const varType& r) const { return nominator*r.denominator > denominator*r.nominator; }
	bool operator>=(const varType& r) const { return nominator*r.denominator >= denominator*r.nominator; }

	operator varType() const { return varType(nominator, denominator); }

	static const int sign = (nominator > 0) - (nominator < 0);

	typedef RationalLiteralType<type, varType, (nominator >=0) ? nominator : -nominator, denominator> abs;

	typedef RationalLiteralType<type, varType, nominator / (gcd_const<nominator,denominator>::result ? gcd_const<nominator,denominator>::result : 1),
			denominator / (gcd_const<nominator,denominator>::result ? gcd_const<nominator,denominator>::result : 1)> simplify;

    static o_type *to_o(o_type *where) {
    	return new (where) o_type(nominator, denominator);
    }


};

template <typename type, typename varType, type n, type d>
ostream& operator<<(ostream& os, const RationalLiteralType<type,varType,n,d> &r) {
	os << r.nominator;
	if (r.denominator != 1) os << '/' << r.denominator;
	return os;
}

template <short n, short d=1> struct SR : public RationalLiteralType<short, ShortRational, n, d> {};
template <int n, int d=1> struct IR : public RationalLiteralType<int, IntegerRational, n, d> {};
template <long n, long d=1> struct LR : public RationalLiteralType<long, LongRational, n, d> {};
template <long long n, long long d=1> struct LLR : public RationalLiteralType<long long, LongLongRational, n, d> {};

}

#endif /* COMPOSIT_TYPES_H_ */
