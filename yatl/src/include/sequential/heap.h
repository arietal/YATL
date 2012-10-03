/*
 * heap.h
 *
 *  Created on: Jan 6, 2012
 *      Author: arietal
 */

#ifndef HEAP_H_
#define HEAP_H_

namespace yatl {

template <typename T, int maxSize>
class heap {
   T data[maxSize];
   int lastElement;
   void swap(T &d1, T &d2) {
     T t = d1;
     d1 = d2;
     d2 = t;
   }

   void siftdown(int index) {
     int largest=index;
     if (index*2+1 < lastElement && data[largest] > data[index*2+1])
       largest = index*2+1;
     if (index*2+2 < lastElement && data[largest] > data[index*2+2])
       largest = index*2+2;
     if (index != largest) {
       swap(data[index], data[largest]);
       siftdown(largest);
     }
   }
   void trickleup(int index) {
     if (index == 0)
       return;
     int parent = (index-1)/2;
     if (data[parent] > data[index]) {
       swap(data[parent], data[index]);
       trickleup(parent);
     }
   }
public:
   heap() : lastElement(1) {
   }
   T pull() {
     T e = data[0];
     if (lastElement > 1) {
       data[0] = data[--lastElement];
       siftdown(0);
     }
     return e;
   }
   void push(const T& e, bool nostore=false) {
      if (lastElement < maxSize) {
	if (!nostore) data[lastElement-1] = e;
	trickleup((lastElement++)-1);
      }
   }
   bool isEmpty() {
     return (lastElement <= 1);
   }

   void print() {
	   std::cout << "[";
	   for (int i=0; i < lastElement-1; i++) {
		   std::cout << data[i];
		   if (i < lastElement-2)
			   std::cout << ", " ;
	   }
	   std::cout << "]" << std::endl;
   }
};

} // namespace

#endif /* HEAP_H_ */
