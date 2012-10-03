/*
 * concurrentcontainer_test.h
 *
 *  Created on: Jul 4, 2012
 *      Author: arietal
 */

#ifndef CONCURRENTCONTAINER_TEST_H_
#define CONCURRENTCONTAINER_TEST_H_

#include <iostream>
#include "../../include/concurrent/atomic.h"
#include "../../include/concurrent/driver.h"
#include "../test.h"

using namespace std;
using namespace yatl;

#define VALUE_RANGE 32768
#define NUMBER_OF_OPERATIONS 20000

class UpdateOp : public Op {
	int *shared;
public:
	UpdateOp(int *shared) : Op("Update", 1, 1, new RandomizedInputSource(VALUE_RANGE)), shared(shared) {}

	virtual int execute(int *args) {
		while (true) {
			int r = *shared;
			if (CAS(shared, r, args[0]))
				return 1;
		}
		return 0;
	}
};

class AddOp : public Op {
	int *shared;
public:
	AddOp(int *shared) : Op("Increment", 2), shared(shared) {}
	virtual int execute(int *args) {
		while (true) {
			int r = *shared;
			if (CAS(shared, r, r+1))
				return 1;
		}
		return 0;
	}
};

template <typename ListType>
class InsertOp : public Op {
	ListType *shared;
public:
	InsertOp(ListType *shared, int valueRange) : Op("Insert", 1, 1, new RandomizedInputSource(valueRange)), shared(shared) {}

	virtual int execute(int *args) {
		return shared->insert(0, args[0]);
	}
};

template <typename ListType>
class DeleteOp : public Op {
	ListType *shared;
public:
	DeleteOp(ListType *shared, int valueRange) : Op("Delete", 1, 1, new RandomizedInputSource(valueRange)), shared(shared) {}

	virtual int execute(int *args) {
		return shared->remove(args[0]);
	}
};

template <typename ListType>
class SearchOp : public Op {
	ListType *shared;
public:
	SearchOp(ListType *shared, int valueRange) : Op("Search", 1,  1, new RandomizedInputSource(valueRange)), shared(shared) {}

	virtual int execute(int *args) {
		return shared->search(args[0]);
	}
};

template <typename Container>
class ConcurrentContainerTest : public Test {
public:
	const char *getName() const { return "Concurrent Container Test"; }
	bool runTest() {


	   int numThreads[] = {  1 , 2, 4, 8 };//, 16, 32, 64};//, 16, 32} ;
	   int numThreadRuns = sizeof(numThreads)/sizeof(int);
	   int valueRanges[] = { 2, 4, 8, 16, 32, 64, 128};//, 256, 512, 1024, 2048, 4096};//, 64*1024, 512*1024 };
	   int valueRangeRuns = sizeof(valueRanges)/sizeof(int);
	   double ratios[numThreadRuns][valueRangeRuns];
	   for (int i=0; i < numThreadRuns; i++)
		   for (int j=0; j < valueRangeRuns; j++) {
			   ratios[i][j] = 0.0;
		   }
	   int totalRepeatRuns = 3;
	   int repeats = 3;

	   for (int r = 0; r < repeats; r++) {
		   for (int i=0; i < numThreadRuns; i++) {
			   for (int vr=0; vr < valueRangeRuns; vr++) {
				  double runTotalTimes[] = { 0.0, 0.0 };
				  for (int run=0; run < totalRepeatRuns; run++) {
					   srandom(1111);
					   Container sharedList;
					   Op *listops[] = {
						   new InsertOp<Container>(&sharedList, valueRanges[vr]),
						   new DeleteOp<Container>(&sharedList, valueRanges[vr]),
						   new SearchOp<Container>(&sharedList, valueRanges[vr])
					   };

					   DriverManager d(numThreads[i], sizeof(listops)/sizeof(Op*), listops, NUMBER_OF_OPERATIONS);

					   d.start();
					   d.join();

					   d.report(true);
					   runTotalTimes[0] += d.getDriverTotalTime();
				  }
			   }
		   }
	   } /* r */
	   return true;
	}
};


#endif
