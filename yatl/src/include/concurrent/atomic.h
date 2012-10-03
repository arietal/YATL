/*
 * atomic.h
 *
 *  Created on: Jan 3, 2012
 *      Author: arietal
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_

namespace yatl {

template <typename T>
bool CAS(T* ptr,T oldval,T newval) {
   return __sync_bool_compare_and_swap (ptr, oldval, newval);
}

} // namespace

#endif // ATOMIC_H_
