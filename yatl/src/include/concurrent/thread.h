/*
 * thread.h
 *
 *  Created on: Jan 2, 2012
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

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

/*
 * A simple C++ thread interface using Mixins
 */

namespace yatl {

class ThreadPrivate {
};


template <typename T>
class Thread : public T {
   pthread_t thread;
   ThreadPrivate *threadPrivate;
   typedef void* (*__workload)(void *);
public:

   Thread(ThreadPrivate *threadPrivate) : threadPrivate(threadPrivate) {}

   int start() {
      return pthread_create(&thread, NULL, (__workload)&T::run, (void *)threadPrivate);
   }

   int join() {
      return pthread_join(thread, NULL);
   }

   ThreadPrivate *getThreadPrivate() { return threadPrivate; }
};




/**
 * sample workload
 *
struct SampleWorkload {
   static void *run(void *arg) {
      // work here
      return NULL;
   }
};

 *
 * Sample definition of a thread using a workload
 *
typedef Thread<SampleWorkload> SampleThread;

 *
 * Start the thread
 *
 SampleThread t;
 t.start(arg);


 *
 * Wait for thread to exit
 *
 t.join();
*/

} // namespace

#endif /* THREAD_H_ */
