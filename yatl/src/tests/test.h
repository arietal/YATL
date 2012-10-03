/*
 * test.h
 *
 *  Created on: Jul 4, 2012
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

#ifndef TEST_H_
#define TEST_H_

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define __STR_MAX_SIZE 10240

class __form {
	char *_str;
public:
	__form(const char *format, ...) : _str(NULL) {
		va_list args;
		va_start(args, format);
		_str = new char[__STR_MAX_SIZE];
		if (_str != NULL) {
			vsprintf(_str, format, args);
		}
		va_end(args);
	}
	~__form() {
		if (_str != NULL)
			delete _str;
	}
	const char *getString() const {
		if (_str != NULL)
			return _str;
		return "null-string";
	}
};

#define Assume(cond,str) {\
		if (!(cond)) {\
			cerr << (char *)__form("-->Assume failed(%s:%d): ", __FILE__, __LINE__).getString();\
			cerr << (char *)__form str .getString() << endl;\
			return false;\
		}\
	}

class Test {
public:
	bool beforeTest() { return true; }
	bool afterTest() { return true; }
	bool runTest() { return true; }
	const char *getName() const { return "generic name"; }
};

typedef Test Harness;

template <typename TestClass>
class Tester : public TestClass {
public:
	bool run() {
		if (!TestClass::beforeTest()) {
			cerr << "	Initialization phase of test " << TestClass::getName() << " failed!";
			return false;
		}
		if (!TestClass::runTest()) {
			cerr << "	Test " << TestClass::getName() << " failed!" << endl;
			return false;
		}
		if (!TestClass::afterTest()) {
			cerr << "	Cleanup phase of test " << TestClass::getName() << " failed!";
			return false;
		}
		cout << "	Test " << TestClass::getName() << " passed!" << endl;
		return true;
	}
};

template <typename HarnessClass>
class TestHarness : public HarnessClass {
public:
	bool run() {
		cout << "Running harness " << HarnessClass::getName() << endl;
		if (!HarnessClass::beforeTest()) {
			cerr << "Initialization phase of harness " << HarnessClass::getName() << " failed!";
			return false;
		}
		if (!HarnessClass::runTest()) {
			cerr << "Harness " << HarnessClass::getName() << " failed!" << endl;
			return false;
		}
		if (!HarnessClass::afterTest()) {
			cerr << "Cleanup phase of harness " << HarnessClass::getName() << " failed!";
			return false;
		}
		cout << "Harness " << HarnessClass::getName() << " passed!" << endl;
		return true;

	}
};

#endif /* TEST_H_ */
