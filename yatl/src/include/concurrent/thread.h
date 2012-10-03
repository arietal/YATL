/*
 * thread.h
 *
 *  Created on: Jan 2, 2012
 *      Author: arietal
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
