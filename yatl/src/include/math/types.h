/*
 * types.h
 *
 *  Created on: Sep 15, 2012
 *      Author: arietal
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <iostream>

using namespace std;

namespace yatl {


template <typename type, typename varType, type val>
struct LiteralType;


template <typename litType>
struct VarType {
	typedef litType type;
	typedef LiteralType<type, VarType<litType>, type(0)> literalType;
	type val_;
	VarType(type val=0) : val_(val) {}
	operator type() const { return val_; }
	bool operator==(type v) const { return val_ == v; }
	typedef VarType o_type;


	template <typename v> VarType add() const { return val_ + v::value; }
	template <typename v> VarType sub() const { return val_ - v::value; }
	template <typename v> VarType mpy() const { return val_ * v::value; }
	template <typename v> VarType div() const { return val_ / v::value; }
	template <typename v> VarType mod() const { return val_ % v::value; }
	template <typename v> VarType iand() const { return val_ & v::value; }
	template <typename v> VarType ior() const { return val_ | v::value; }
	template <typename multiplier, typename addend> VarType ma() const { return val_ * multiplier::value + addend::value; }
	VarType inot() const { return ~val_; }

	template <typename v> bool eq() const { return val_ == v::value; }
	template <typename v> bool ne() const { return val_ != v::value; }
	template <typename v> bool lt() const { return val_ < v::value; }
	template <typename v> bool le() const { return val_ <= v::value; }
	template <typename v> bool gt() const { return val_ > v::value; }
	template <typename v> bool ge() const { return val_ >= v::value; }

	template <typename v> VarType max() const { return (val_ >= v::value) ? val_ : v::value; }
	template <typename v> VarType min() const { return (val_ >= v::value) ? val_ : v::value; }


	VarType operator+(const VarType& v) const { return val_ + v.val_; }
	VarType operator-(const VarType& v) const { return val_ - v.val_; }
	VarType operator-() const { return -val_; }

	VarType operator*(const VarType& v) const { return val_ * v.val_; }
	VarType operator*(type val) const { return val_ * val; }
	VarType operator/(const VarType& v) const { return val_ / v.val_; }
	VarType operator/(type val) const { return val_ / val; }
	VarType operator%(const VarType& v) const { return val_ % v.val_; }
	VarType operator%(type val) const { return val_ % val; }

	VarType& operator=(const VarType& v) { val_ = v.val_; return *this; }
	VarType& operator+=(const VarType& v) { val_ += v.val_; return *this; }
	VarType& operator-=(const VarType& v) { val_ -= v.val_; return *this; }

	VarType& operator*=(const VarType& v) { val_ *= v.val_; return *this; }
	VarType& operator/=(const VarType& v) { val_ /= v.val_; return *this; }
	VarType& operator%=(const VarType& v) { val_ %= v.val_; return *this; }

	bool operator==(const VarType& v) const { return val_ == v.val_; }
	bool operator!=(const VarType& v) const { return val_ != v.val_; }
	bool operator<=(const VarType& v) const { return val_ <= v.val_; }
	bool operator<(const VarType& v) const { return val_ < v.val_; }
	bool operator>=(const VarType& v) const { return val_ >= v.val_; }
	bool operator>(const VarType& v) const { return val_ > v.val_; }

	template <typename r>
	static int compareConst(const VarType &a) {
		return (a.val_ > r::value) - (a.val_ < r::value);
	}

	int sign() const {
		return (val_ > 0) - (val_ < 0);
	}

	VarType abs() const { return (val_ >= 0) ? val_ : -val_; }

	operator VarType() const { return *this; }

};


typedef VarType<char> Char;
typedef VarType<short> Short;
typedef VarType<int> Integer;
typedef VarType<long> Long;
typedef VarType<long long> LongLong;
typedef VarType<unsigned char> UnsignedChar;
typedef VarType<unsigned short> UnsignedShort;
typedef VarType<unsigned int> UnsignedInteger;
typedef VarType<unsigned long> UnsignedLong;
typedef VarType<unsigned long long> UnsignedLongLong;


template <typename type>
ostream& operator<<(ostream& os, const VarType<type>& v) {
	return os << v.val_;
}

template <typename type, typename varType, type val>
struct LiteralType {
	typedef varType o_type;
	typedef LiteralType<type, varType, type(1)> one;
	typedef LiteralType<type, varType, type(0)> zero;

	static const type value = val;

	template <typename v>
	struct mpy : public LiteralType<type, varType, val * v::value> {};
	template <typename v>
	struct add : public LiteralType<type, varType, val + v::value> {};
	template <typename v>
	struct sub : public LiteralType<type, varType, val - v::value> {};
	template <typename v>
	struct div : public LiteralType<type, varType, val / v::value> {};
	template <typename v>
	struct iand : public LiteralType<type, varType, val & v::value> {};
	template <typename v>
	struct ior : public LiteralType<type, varType, val | v::value> {};
	struct inot : public LiteralType<type, varType, ~val> {};

	template <typename v>
	struct max : public LiteralType<type, varType, (value>v::value)?value:v::value> {};

	template <typename v>
	struct min : public LiteralType<type, varType, (value<v::value)?value:v::value> {};

	template <typename v>
	struct eq {
		static const bool result = value == v::value;
		operator bool() { return result; }
	};
	template <typename v>
	struct ne {
		static const bool result = value != v::value;
		operator bool() { return result; }
	};
	template <typename v>
	struct lt {
		static const bool result = value < v::value;
		operator bool() { return result; }
	};
	template <typename v>
	struct le {
		static const bool result = value <= v::value;
		operator bool() { return result; }
	};
	template <typename v>
	struct gt {
		static const bool result = value > v::value;
		operator bool() { return result; }
	};
	template <typename v>
	struct ge {
		static const bool result = value >= v::value;
		operator bool() { return result; }
	};

	template <typename v>
	struct compare {
		static const int result = (value > v::value) - (value < v::value);
	};

	varType operator+(const varType& v) const { return value + v.val_; }
	varType operator-(const varType& v) const { return value - v.val_; }

	varType operator*(const varType& v) const { return value * v.val_; }
	varType operator/(const varType& v) const { return value / v.val_; }
	varType operator%(const varType& v) const { return value % v.val_; }

	varType& operator+=(const varType& v) { value += v.val_; return *this; }
	varType& operator-=(const varType& v) { value -= v.val_; return *this; }

	varType& operator*=(const varType& v) { value *= v.val_; return *this; }
	varType& operator/=(const varType& v) { value /= v.val_; return *this; }
	varType& operator%=(const varType& v) { value %= v.val_; return *this; }

	bool operator==(const varType& v) const { return value == v.val_; }
	bool operator!=(const varType& v) const { return value != v.val_; }
	bool operator<=(const varType& v) const { return value <= v.val_; }
	bool operator<(const varType& v) const { return value < v.val_; }
	bool operator>=(const varType& v) const { return value >= v.val_; }
	bool operator>(const varType& v) const { return value > v.val_; }

	operator varType() const { return varType(value); }

	static const int sign = (value > 0) - (value < 0);

	typedef LiteralType<type, varType, (value >= 0) ? value : -value> abs;

    static o_type *to_o(o_type *where) {
    	return new (where) o_type(value);
    }

};

template <typename type, typename varType, int val>
ostream& operator<<(ostream& os, const LiteralType<type, varType, val> &v) {
	return os << val;
}

template <char val> struct C : public LiteralType<char, Char, val> {};
template <short val> struct S : public LiteralType<short, Short, val> {};
template <int val> struct I : public LiteralType<int, Integer, val> {};
template <long val> struct L : public LiteralType<long, Long, val> {};
template <long long val> struct LL : public LiteralType<long long, LongLong, val> {};
template <unsigned char val> struct UC : public LiteralType<unsigned char, UnsignedChar, val> {};
template <unsigned short val> struct US : public LiteralType<unsigned short, UnsignedShort, val> {};
template <unsigned int val> struct UI : public LiteralType<unsigned int, UnsignedInteger, val> {};
template <unsigned long val> struct UL : public LiteralType<unsigned long, UnsignedLong, val> {};
template <unsigned long long val> struct ULL : public LiteralType<unsigned long long, UnsignedLongLong, val> {};


// operators

template <typename v1, typename v2> struct add_op : public v1::template add<v2> {};
template <typename v1, typename v2> struct sub_op : public v1::template sub<v2> {};
template <typename v1, typename v2> struct mpy_op : public v1::template mpy<v2> {};
template <typename v1, typename v2> struct div_op : public v1::template div<v2> {};
template <typename v1, typename v2> struct max_op : public v1::template max<v2> {};
template <typename v1, typename v2> struct min_op : public v1::template min<v2> {};
template <typename v1, typename v2> struct eq_op : public v1::template eq<v2> {};
template <typename v1, typename v2> struct ne_op : public v1::template ne<v2> {};
template <typename v1, typename v2> struct lt_op : public v1::template lt<v2> {};
template <typename v1, typename v2> struct le_op : public v1::template le<v2> {};
template <typename v1, typename v2> struct gt_op : public v1::template gt<v2> {};
template <typename v1, typename v2> struct ge_op : public v1::template ge<v2> {};
template <typename v1, typename v2> struct cmp : public v1::template compare<v2> {};

template <typename t1, typename t2>
typename t1::o_type max(const t1& a1, const t2& a2) {
	return (a1 >= a2) ? a1.operator typename t1::o_type()  : a2.operator typename t1::o_type();
}

template <typename t1, typename t2>
typename t1::o_type min(const t1& a1, const t2& a2) {
	return (a1 <= a2) ? a1.operator typename t1::o_type()  : a2.operator typename t1::o_type();
}


}


#endif /* TYPES_H_ */
