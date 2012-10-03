/*
 * intrange.h
 *
 *  Created on: Sep 12, 2012
 *      Author: arietal
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
