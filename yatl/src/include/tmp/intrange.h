/*
 * intrange.h
 *
 *  Created on: Sep 12, 2012
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

#ifndef INTRANGE_H_
#define INTRANGE_H_

#include <iostream>

using namespace std;

#include "../math/types.h"
#include "list.h"

namespace yatl {

using namespace List;

template <int rangeStart, int rangeEnd>
struct intRange :public list<list_< I<rangeStart>, typename intRange<rangeStart+1, rangeEnd>::List_> > {};

template <int rangeEnd>
struct intRange<rangeEnd,rangeEnd> : public list<_NIL> {};

}

#endif /* INTRANGE_H_ */
