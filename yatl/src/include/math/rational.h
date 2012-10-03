/*
 * rational.h
 *
 *  Created on: Aug 4, 2012
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

// DEPRACATED, use composite_types.h instead

#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <iostream>

#include "gcd.h"
#include "../tmp/base.h"

using namespace yatl;

namespace yatl {

template <int n, int d, bool useGCD=true>
struct rational_const {
    rational_const(int n_, int d_) {}
    rational_const() {}
	enum { nominator = n/gcd_const<n,d>::result, denominator = d/gcd_const<n,d>::result};
    operator double() { return (double)nominator / (double)denominator; }
    int getNominator() const { return nominator; }
    int getDenominator() const { return denominator; }
    void setNominator(int nominator) {}
    void setDenominator(int denominator) {}
};

template <int n, int d>
struct rational_const<n,d,false> {
    rational_const(int n_, int d_) {}
    rational_const() {}
	enum { nominator = n, denominator = d};
    operator double() { return (double)nominator / (double)denominator; }
    int getNominator() const { return nominator; }
    int getDenominator() const { return denominator; }
    void setNominator(int nominator) {}
    void setDenominator(int denominator) {}
};


template <bool useGCD>
struct rational_const<0,0,useGCD> {
    rational_const() {}
    enum {nominator = 0, denominator = 0};
//    operator double() { return (denominator != 0) ? (double)nominator / (double)denominator : 0.0; }
    int getNominator() const { return nominator; }
    int getDenominator() const { return denominator; }
    void setNominator(int nominator) {}
    void setDenominator(int denominator) {}
};

template <int n, int d>
struct rational;

struct rational_var {
    enum {nominator = 0, denominator = 0};
    int nominator_, denominator_;
    rational_var(int nom, int denom) {
       // gcd_var gcd_(nom, denom);
        nominator_ = nom ;// / gcd_;
        denominator_ = denom ;// / gcd_;
    }
    rational_var& simplify() {
    	VarGCD<int> gcd_(nominator_, denominator_);
    	nominator_ /= gcd_;
    	denominator_ /= gcd_;
    	return *this;
    }

    operator double() { return (double)nominator_ / (double)denominator_; }
    int getNominator() const { return nominator_; }
    int getDenominator() const { return denominator_; }
    void setNominator(int nom) { nominator_ = nom; }
    void setDenominator(int denom) { denominator_ = denom; }
    rational_var& operator=(const rational_var& r) {
        nominator_ = r.nominator_;
        denominator_ = r.denominator_;
        return *this;
    }
    rational_var& operator+=(const rational_var& r) {
        int n = nominator_*r.denominator_ + r.nominator_*denominator_;
        int d = denominator_*r.denominator_;
        gcd<> g(n,d);
        nominator_ = n/g;
        denominator_ = d/g;
        return *this;
    }
    rational_var& operator-=(const rational_var& r) {
        int n = nominator_*r.denominator_ - r.nominator_*denominator_;
        int d = denominator_*r.denominator_;
        gcd<> g(n,d);
        nominator_ = n/g;
        denominator_ = d/g;
        return *this;
    }
    rational_var& operator*=(const rational_var& r) {
        int n = nominator_*r.nominator_;
        int d = denominator_*r.denominator_;
        gcd<> g(n,d);
        nominator_ = n/g;
        denominator_ = d/g;
        return *this;
    }
    rational_var& operator/=(const rational_var& r) {
        int n = nominator_*r.denominator_;
        int d = denominator_*r.nominator_;
        gcd<> g(n,d);
        nominator_ = n/g;
        denominator_ = d/g;
        return *this;
    }
    template <int n, int d>
    rational_var& operator=(const rational_const<n,d>& rc) {
    	nominator_ = rc.getNominator();
    	denominator_ = rc.getDenominator();
    	return *this;
    }
    template <int n>
    bool fast_lt() const {
    	return nominator_ < n;
    }

    template <typename r>
    bool lt() const {
    	return nominator_ * r::denominator < r::nominator * denominator_;
    }

    template <int n>
    bool fast_gt() const {
    	return nominator_ > n;
    }

    template <typename r>
    bool gt() const {
    	return nominator_ * r::denominator > r::nominator * denominator_;
    }

    template <typename r>
    bool ne() const {
    	return nominator_ * r::denominator != r::nominator * denominator_;
    }

    template <typename rmin, typename rmax>
    bool in_range() const {
    	typedef lcm_const<rmin::denominator, rmax::denominator> rangelcm;
    	enum { factor1_ = rangelcm::result / rmin::denominator,
    		   factor2_ = rangelcm::result / rmax::denominator };
    	typedef rational_const< rmin::nominator * factor1_, rangelcm::result, false> rangemin;
    	typedef rational_const< rmax::nominator * factor2_, rangelcm::result, false> rangemax;
    	int lcmnominator = nominator_ * rangelcm::result;
    	return lcmnominator >= rangemin::nominator*denominator_ && lcmnominator <= rangemax::nominator*denominator_;
//    	return !lt<rmin>() && !gt<rmax>();
    }

    template <int rmin_n, int rmax_n>
    bool in_range_fast() const {
    	return nominator_ >= rmin_n*denominator_ && nominator_ <= rmax_n*denominator_;
    }


    template <typename r>
    rational_var mul() const {
    	return rational_var(nominator_ * r::nominator, denominator_ * r::denominator);
    }

    template <typename multiplier, typename addend>
    rational_var rma() const {
    	int mul_n = nominator_ * multiplier::nominator;
    	int mul_d = denominator_ * multiplier::denominator;
    	return rational_var(mul_n * addend::denominator + mul_d * addend::nominator, mul_d * addend::denominator);
    }

    template <typename r>
    static int compareConst(const rational_var& rv) {
    	int n1 = rv.nominator_ * r::denominator;
    	int n2 = rv.denominator_ * r::nominator;
    	int d = rv.denominator_ * r::denominator;
    	int sign = (d > 0) - (d < 0);
    	return sign * (( n1 > n2) - ( n1 < n2));
    }
};

template <typename r1, typename r2>
struct rational_gt_op;

template <typename r1, typename r2>
struct rational_lt_op;

template <typename r1, typename r2>
struct rational_compare;


template <int n=0, int d=0>
struct rational : public if_else<d==0, rational_var, rational_const<n,d> >::type {
    typedef typename if_else<d==0, rational_var, rational_const<n,d> >::type SuperType;
    using SuperType::getDenominator;
    using SuperType::getNominator;
    using SuperType::setDenominator;
    using SuperType::setNominator;
    typedef rational<0,1> zero;
    typedef rational<1,1> one;
    rational() : SuperType(n,d) {}
    rational(int n_) : SuperType(n_,1) {}
    rational(float f) : SuperType(floatToRational(f)) {}
    rational(double f) : SuperType(floatToRational((float)f)) {}
    rational(int n_, int d_) : SuperType(n_, d_) {}
    rational(const rational_var& rv) : SuperType(rv.getNominator(), rv.getDenominator()) {}
    rational(const rational<>& rv) : SuperType(rv.getNominator(), rv.getDenominator()) {}
    operator rational<0,0>() { return rational<0,0>(getNominator(), getDenominator()); }
    operator const rational<0,0>() const { return rational<0,0>(getNominator(), getDenominator()); }
    template <int n2, int d2> bool operator==(const rational<n2,d2>& r2) { return (getNominator()*r2.getDenominator() == r2.getNominator() * getDenominator()) && (getDenominator() * r2.getDenominator() != 0); }
    static rational<> floatToRational(float f) {
    	const int maxDiv = 10000;
    	int div=1;
    	while (div < maxDiv && (f*(float)div) - (int)(f*(float)div) != 0) {
    		div *= 10;
    	}
    	return rational<>( (int)(f*(float)div), div );
    }
    void print(ostream &os) {
        os << getNominator() << "/" << getDenominator();
    }

    typedef rational<> o_type;

    static o_type *to_o(o_type *where) {
    	return new (where) o_type(SuperType::nominator, SuperType::denominator);
    }

    template <typename r2>
    struct compare : public rational_compare<rational<n,d>, r2> {};
//    template <typename r>
//    struct compare {};
};

template <int n, int d=1>
struct R : public rational<n,d> {
	R(int n_=n, int d_=d) : rational<n,d>(n_,d_) {}
};


template <int n, int d>
ostream& operator<<(ostream& os, const rational<n,d>& r) {
    os << r.getNominator();
    if (r.getDenominator() != 1) os << "/" << r.getDenominator();
    return os;
}

ostream& operator<<(ostream& os, const rational_var& r) {
    os << r.getNominator();
    if (r.getDenominator() != 1) os << "/" << r.getDenominator();
    return os;
}

#define Rational rational<>
    
/**
 * == operator
 */
    
template <typename r1, typename r2>
struct rational_eq_op {
    enum { result = ((r1::nominator*r2::denominator == r2::nominator*r1::denominator) ? 1 : 0) };
    operator bool() { return result; }
};


template <typename r1>
class rational_eq_op<r1, Rational > {
    const Rational& r2_;
public:
    rational_eq_op(const Rational& r2) : r2_(r2) {}
    operator bool() { return r1::nominator * r2_.getDenominator() == r2_.getNominator() * r1::denominator; };
};

template <>
class rational_eq_op<Rational, Rational > {
    const Rational& r1_;
    const Rational& r2_;
public:
    rational_eq_op(const Rational& r1, const Rational& r2) : r1_(r1), r2_(r2) {}
    operator bool() { return r1_.getNominator() * r2_.getDenominator() == r2_.getNominator() * r1_.getDenominator(); };
};


//template <int n1, int d1, int n2, int d2>
//bool operator==(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) { return rational_eq_op<rational<n1,d1>, rational<n2,d2> >::result; }
//
//template <int n, int d>
//bool operator==(const rational<n,d>& r1_, const Rational& r2_) { return rational_eq_op<rational<n, d>, Rational >(r2_); }
//
//template <int n, int d>
//bool operator==(const Rational& r1_, const rational<n,d>& r2_) { return rational_eq_op<rational<n, d>, Rational >(r1_); }

//bool operator==(const Rational& r1_, const Rational& r2_) { return r1_.getNominator() * r2_.getDenominator() == r2_.getNominator() * r1_.getDenominator(); }


#define DEFINE_REL_OP(opName, op, revOp) \
\
template <typename r1, typename r2> \
struct rational_##opName##_op {\
    enum { result = (r1::denominator * r2::denominator > 0) ? ((r1::nominator*r2::denominator op r2::nominator*r1::denominator) ? 1 : 0) :\
                ((r1::nominator*r2::denominator revOp r2::nominator*r1::denominator) ? 1 : 0)  };\
    operator bool() { return result; }\
};\
\
\
template <typename r1>\
class rational_##opName##_op<r1, Rational > {\
    const Rational& r2_;\
public:\
    enum { result = 0 };\
    rational_##opName##_op(const Rational& r2) : r2_(r2) {}\
    operator bool() { return (r1::denominator * r2_.getDenominator() > 0) ? (r1::nominator*r2_.getDenominator() op r2_.getNominator()*r1::denominator) :\
        (r1::nominator*r2_.getDenominator() revOp r2_.getNominator()*r1::denominator); }\
};\
\
\
template <typename r2>\
class rational_##opName##_op<Rational, r2 > {\
    const Rational& r1_;\
public:\
    enum { result = 0 };\
    rational_##opName##_op(const Rational& r1) : r1_(r1) {}\
    operator bool() { return (r1_.getDenominator() * r2::denominator > 0) ? (r1_.getNominator()*r2::denominator op r2::nominator*r1_.getDenominator()) :\
        (r1_.getNominator()*r2::denominator revOp r2::nominator*r1_.getDenominator()); }\
};\
\
template <>\
class rational_##opName##_op<Rational, Rational > {\
    const Rational& r1_;\
    const Rational& r2_;\
public:\
	enum { result = 0 };\
    rational_##opName##_op(const Rational& r1, const Rational& r2) : r1_(r1), r2_(r2) {}\
    operator bool() { return (r1_.getDenominator() * r2_.getDenominator() > 0) ? (r1_.getNominator()*r2_.getDenominator() op r2_.getNominator()*r1_.getDenominator()) :\
        (r1_.getNominator()*r2_.getDenominator() revOp r2_.getNominator()*r1_.getDenominator()); }\
\
};\
\
\
template <int n1, int d1, int n2, int d2>\
bool operator op(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) { return rational_##opName##_op<rational<n1,d1>, rational<n2,d2> >::result; }\
\
template <int n, int d>\
bool operator op(const rational<n,d>& r1_, const Rational& r2_) { return rational_##opName##_op<rational<n, d>, Rational >(r2_); }\
\
template <int n, int d>\
bool operator op(const Rational& r1_, const rational<n,d>& r2_) { return rational_##opName##_op<Rational, rational<n, d> >(r1_); }\
\
bool operator op(const Rational& r1_, const Rational& r2_) { return rational_##opName##_op<Rational, Rational>(r1_, r2_); }
    
DEFINE_REL_OP(gt, >, <)
DEFINE_REL_OP(lt, <, >)
DEFINE_REL_OP(ge, >=, <=)
DEFINE_REL_OP(le, <=, >=)


    
template <typename r1, typename r2>
struct rational_add_op : public rational<r1::nominator*r2::denominator+r2::nominator*r1::denominator, r1::denominator*r2::denominator> {};


template <typename rconst>
struct rational_add_op<rconst, rational_var> : public rational<0,0> {
    rational_add_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getDenominator() + rconst::denominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};

template <typename rconst>
struct rational_add_op<rational_var, rconst> : public rational<0,0> {
    rational_add_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getDenominator() + rconst::denominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};
    

    
template <int n1, int d1, int n2, int d2>
Rational operator+(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return Rational(r1_.getNominator()*r2_.getDenominator() + r2_.getNominator()*r1_.getDenominator(), r1_.getDenominator()*r2_.getDenominator());
}
    
//template <int n1, int d1>
//rational_add_op<rational<n1, d1>, rational_var> operator+(const rational_const<n1,d1>& r1_, const rational_var& r2_) { return rational_add_op<rational<n1,d1>, rational_var >(r2_); }
//
//template <int n1, int d1>
//rational_add_op<rational<n1, d1>, rational_var> operator+(const rational_var& r2_, const rational_const<n1,d1>& r1_) { return rational_add_op<rational_var, rational<n1,d1> >(r2_); }
    



template <typename r1, typename r2>
struct rational_sub_op : public rational<r1::nominator*r2::denominator-r2::nominator*r1::denominator, r1::denominator*r2::denominator> {};
    
template <typename rconst>
struct rational_sub_op<rconst, rational_var> : public rational<0,0> {
    rational_sub_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getDenominator() - rconst::denominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};
    
template <typename rconst>
struct rational_sub_op<rational_var, rconst> : public rational<0,0> {
    rational_sub_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getDenominator() - rconst::denominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};
    
    
template <int n1, int d1, int n2, int d2>
Rational operator-(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return Rational(r1_.getNominator()*r2_.getDenominator() - r2_.getNominator()*r1_.getDenominator(), r1_.getDenominator()*r2_.getDenominator());
}

//
//template <int n1, int d1>
//rational_sub_op<rational<n1, d1>, rational_var> operator-(const rational_const<n1,d1>& r1_, const rational_var& r2_) { return rational_sub_op<rational<n1,d1>, rational_var >(r2_); }
//
//template <int n1, int d1>
//rational_sub_op<rational<n1, d1>, rational_var> operator-(const rational_var& r2_, const rational_const<n1,d1>& r1_) { return rational_sub_op<rational_var, rational<n1,d1> >(r2_); }
//
//

    
    
template <typename r1, typename r2>
struct rational_mpy_op : public rational<r1::nominator*r2::nominator, r1::denominator*r2::denominator> {};

template <typename rconst>
struct rational_mpy_op<rconst, rational_var> : public rational<0,0> {
    rational_mpy_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};

template <typename rconst>
struct rational_mpy_op<rational_var, rconst> : public rational<0,0> {
    rational_mpy_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getNominator(), rconst::denominator*rv.getDenominator()) {}
};

//template <>
//struct rational_mpy_op<rational_var, rational_var> : public rational<0,0> {
//    rational_mpy_op(const rational_var& rv1, const rational_var& rv2) : SuperType(rv1.getNominator()*rv2.getNominator(), rv1.getDenominator()*rv2.getDenominator()) {}
//};


template <int n1, int d1, int n2, int d2>
Rational operator*(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return Rational(r1_.getNominator() * r2_.getNominator(), r1_.getDenominator() * r2_.getDenominator());
}
//
//template <int n1, int d1>
//rational_mpy_op<rational<n1, d1>, Rational> operator*(const rational_const<n1,d1>& r1_, const rational_var& r2_) { cout << "+++" ; return rational_mpy_op<rational<n1,d1>, rational_var >(r2_); }
//
//template <int n1, int d1>
//rational_mpy_op<Rational, rational<n1,d1> > operator*(const rational_var& r2_, const rational_const<n1,d1>& r1_) { cout << "---" ; return rational_mpy_op<rational_var, rational<n1,d1> >(r2_); }


    
    
template <typename r1, typename r2>
struct rational_div_op : public rational<r1::nominator*r2::denominator, r1::denominator*r2::nominator> {};

template <typename rconst>
struct rational_div_op<rconst, rational_var> : public rational<0,0> {
    rational_div_op(const rational_var& rv) : SuperType(rconst::nominator*rv.getDenominator(), rconst::denominator*rv.getNominator()) {}
};

template <typename rconst>
struct rational_div_op<rational_var, rconst> : public rational<0,0> {
    rational_div_op(const rational_var& rv) : SuperType(rconst::denominator*rv.getNominator(), rconst::nominator*rv.getDenominator()) {}
};


template <int n1, int d1, int n2, int d2>
Rational operator/(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return Rational(r1_.getNominator()*r2_.getDenominator(), r1_.getDenominator()*r2_.getNominator());
}

//template <int n1, int d1>
//rational_div_op<rational<n1, d1>, rational_var> operator/(const rational_const<n1,d1>& r1_, const rational_var& r2_) { return rational_div_op<rational<n1,d1>, rational_var >(r2_); }
//
//template <int n1, int d1>
//rational_div_op<rational<n1, d1>, rational_var> operator/(const rational_var& r2_, const rational_const<n1,d1>& r1_) { return rational_div_op<rational_var, rational<n1,d1> >(r2_); }
//


template <int n1=0, int d1=0, int n2=0, int d2=0>
struct rational_max_op_impl : public rational<
   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
        typename if_else< rational_gt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
   >::type::nominator,
   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
        typename if_else< rational_gt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
   >::type::denominator
   > {

	typedef rational<
			   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
			        typename if_else< rational_gt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
			   >::type::nominator,
			   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
			        typename if_else< rational_gt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
			   >::type::denominator
			   > SuperType;
	rational_max_op_impl(const rational<n1,d1>& r1, const rational<n2,d2>& r2) {
		rational<> r1_ = rational<>(r1.getNominator(), r1.getDenominator());
		rational<> r2_ = rational<>(r2.getNominator(), r2.getDenominator());
		rational<> max = (r1_ > r2_) ? r1_ : r2_;
		if (d1 == 0 || d2 == 0) {
			SuperType::setNominator(max.getNominator());
			SuperType::setDenominator(max.getDenominator());
		}
	}
};

template<int n1, int d1, int n2, int d2>
rational_max_op_impl<n1,d1,n2,d2 > max(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return rational_max_op_impl<n1,d1,n2,d2>(r1_, r2_);
}



template <int n1=0, int d1=0, int n2=0, int d2=0>
struct rational_min_op_impl : public rational<
   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
        typename if_else< rational_lt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
   >::type::nominator,
   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
        typename if_else< rational_lt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
   >::type::denominator
   > {

	typedef rational<
			   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
			        typename if_else< rational_lt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
			   >::type::nominator,
			   if_else< d1 == 0 || d2 == 0, rational_const<0,0>,
			        typename if_else< rational_lt_op<rational<n1,d1>,rational<n2,d2> >::result, rational<n1,d1>,rational<n2,d2> >::type
			   >::type::denominator
			   > SuperType;
	rational_min_op_impl(const rational<n1,d1>& r1, const rational<n2,d2>& r2) {
		rational<> r1_ = rational<>(r1.getNominator(), r1.getDenominator());
		rational<> r2_ = rational<>(r2.getNominator(), r2.getDenominator());
		rational<> min = (r1_ < r2_) ? r1_ : r2_;
		if (d1 == 0 || d2 == 0) {
			SuperType::setNominator(min.getNominator());
			SuperType::setDenominator(min.getDenominator());
		}
	}
	rational_min_op_impl() {}
};

template<int n1, int d1, int n2, int d2>
rational_min_op_impl<n1,d1,n2,d2 > min(const rational<n1,d1>& r1_, const rational<n2,d2>& r2_) {
	return rational_min_op_impl<n1,d1,n2,d2>(r1_, r2_);
}


template <typename r1, typename r2>
struct rational_min_op : public rational_min_op_impl<r1::nominator, r1::denominator, r2::nominator, r2::denominator> {
	typedef rational_min_op_impl<r1::nominator, r1::denominator, r2::nominator, r2::denominator> SuperType;
	typedef rational<SuperType::nominator, SuperType::denominator> type;
};

template <typename r1, typename r2>
struct rational_max_op : public rational_max_op_impl<r1::nominator, r1::denominator, r2::nominator, r2::denominator> {
	typedef rational_max_op_impl<r1::nominator, r1::denominator, r2::nominator, r2::denominator> SuperType;
	typedef rational<SuperType::nominator, SuperType::denominator> type;
};

template <typename r1, typename r2>
struct rational_compare {
	enum { result = rational_gt_op< r1, r2 >::result - rational_lt_op< r1, r2 >::result };
};

} // namespace

#endif /* RATIONAL_H_ */
