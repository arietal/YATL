/*
 * driver.h
 *
 *  Created on: Jan 3, 2012
 *      Author: arietal
 */

#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

#include "thread.h"

using namespace yatl;

namespace yatl {

class InputSource {
public:
	virtual ~InputSource() {}
	virtual int getNext() = 0;
};

class RandomizedInputSource : public InputSource {
	int minRange, maxRange;
public:
	RandomizedInputSource(int minRange, int maxRange) : minRange(minRange), maxRange(maxRange) {}
	RandomizedInputSource(int maxRange) : minRange(0), maxRange(maxRange) {}

	virtual int getNext() {
		long r = random();
		return (r % (maxRange - minRange)) + minRange;
	}
};

class BufferedRandomizedInputSource : public RandomizedInputSource {
	int *buffer;
	int bufferSize;
	int next;

	void fillupBuffer() {
		for (int i=0; i < bufferSize; i++) {
			buffer[i] = RandomizedInputSource::getNext() ;
		}

	}
public:
	BufferedRandomizedInputSource(int minRange, int maxRange, int bufferSize) : RandomizedInputSource(minRange, maxRange), bufferSize(bufferSize), next(0) {
		buffer = new int[bufferSize];
		fillupBuffer();
	}

	~BufferedRandomizedInputSource() {
		delete[] buffer;
	}

	virtual int getNext() {
		if (next >= bufferSize) {
			fillupBuffer();
			next = 0;
		}
		return buffer[next++];
	}

};


class Op {
	const char *opName;
	int weight;
	int opIndex;
	int numberOfOperands;
	InputSource *pInputSource;
	InputSource **inputSources;

public:
	Op(const char *opName, int weight, int numberOfOperands=0, InputSource **inputSources=NULL) :
		opName(opName), weight(weight), numberOfOperands(numberOfOperands), inputSources(inputSources) {}

	Op(const char *opName, int weight, int numberOfOperands, InputSource *inputSource) :
		opName(opName), weight(weight), numberOfOperands(numberOfOperands), pInputSource(inputSource), inputSources(&pInputSource) {}


	virtual ~Op() {}

	int getWeight() { return weight; }

	void setOpIndex(int opIndex) { this->opIndex = opIndex; }

	int getOpIndex() const { return opIndex; }

	const char *getOpName() const { return opName; }

	int getNumberOfOperands() const { return numberOfOperands; }

	void loadArgs(int *args) {
		if (numberOfOperands > 0) {
			for (int i=0; i < numberOfOperands; i++) {
				args[i] = inputSources[i]->getNext();
			}
		}
	}

	int run(int *args) {
		return execute(args);
	}

	virtual int execute(int *args) = 0;

};



class Driver : public ThreadPrivate {
	int numberOfOps;
	Op **ops;
	int numberOfSteps;
	int *opCounts;
	double *opTotalTimes;
	RandomizedInputSource *nextOpPicker;

	Op *getNextOp() {
		Op *selected = ops[nextOpPicker->getNext()];
		opCounts[selected->getOpIndex()]++;
		return selected;
	}

public:
	Driver(int numberOfOps, Op **operations, int numberOfSteps) : numberOfOps(numberOfOps), numberOfSteps(numberOfSteps) {
		int totalWeight = 0;
		opCounts = new int[numberOfOps];
		opTotalTimes = new double[numberOfOps];
		for (int i=0; i < numberOfOps; i++) {
			totalWeight += operations[i]->getWeight();
			opCounts[i] = 0;
			operations[i]->setOpIndex(i);
			opTotalTimes[i] = 0.0;
		}
		ops = new Op*[totalWeight];
		int lastIndex = 0;
		for (int i=0; i < numberOfOps; i++)
			for (int j=0; j < operations[i]->getWeight(); j++)
				ops[lastIndex++] = operations[i];

		nextOpPicker = new RandomizedInputSource(totalWeight);
		thread = NULL;
	}

	~Driver() {
		delete[] ops;
		delete[] opTotalTimes;
		delete[] opCounts;
		if (thread != NULL)
			delete thread;
	}

	int runNextOp() {
		Op *op = getNextOp();
		timeval t1, t2;
		int opNumOp = op->getNumberOfOperands();
		int operandCount = (opNumOp > 0) ? opNumOp : 1;
		int args[operandCount];
		if (operandCount > 0)
			op->loadArgs(args);

		gettimeofday(&t1, NULL);
		int r = op->run(args);
		gettimeofday(&t2, NULL);


		double diffTime = ((double)t2.tv_sec*1.0E6+(double)t2.tv_usec) -
				((double)t1.tv_sec*1.0E6+(double)t1.tv_usec);
//		double diffTime = (double)(t2.tv_sec - t1.tv_sec)*1.0E6 + (double)(t2.tv_usec - t1.tv_usec);
		opTotalTimes[op->getOpIndex()] += diffTime;
		return r;
	}

	int *getOpCounts() const { return opCounts; }

	int getNumberOfOps() const { return numberOfOps; }

	double *getOpTotalTimes() const { return opTotalTimes; }

	class Workload {
	public:
		static void *run(void *arg) {
			Driver *driver = (Driver*)arg;
			while (driver->numberOfSteps > 0) {
				driver->runNextOp();
				driver->numberOfSteps--;
			}
			return NULL;
		}
	};

	typedef ::Thread<Driver::Workload> Thread;
private:
	Driver::Thread *thread;

public:

	void report() {
		std::cout << "Driver Report:" << std::endl ;
		double driverTotalTime = 0;
		for (int i=0; i < numberOfOps; i++) {
			std::cout << "  " << ops[i]->getOpName() << ":   count:" << opCounts[i] << "  totalTime:" << opTotalTimes[i] << "  timePerOp:" << (opTotalTimes[i]/(double)opCounts[i]) << std::endl;
			driverTotalTime += opTotalTimes[i];
		}
		std::cout << "  totalTime:" << driverTotalTime << std::endl;
	}

	void start() {

	}

	Driver::Thread *getThread() {
		if (thread == NULL)
			thread = new Driver::Thread(this);
		return thread;
	}

};

class DriverManager {
	Driver **drivers;
	int numberOfThreads;
	int numberOfOps;
	Op **ops;
	int numberOfSteps;
	double driverTotalTime;
public:
	DriverManager(int numberOfThreads, int numberOfOps, Op **ops, int numberOfSteps) : numberOfThreads(numberOfThreads), numberOfOps(numberOfOps), ops(ops), numberOfSteps(numberOfSteps) {
		drivers = new Driver*[numberOfThreads];

		for (int i=0; i < numberOfThreads; i++) {
			drivers[i] = new Driver(numberOfOps, ops, numberOfSteps);
		}
		driverTotalTime=0;
	}

	~DriverManager() {
		for (int i=0; i < numberOfThreads; i++) {
			delete drivers[i];
		}
		delete[] drivers;
	}

	void start() {
		for (int i=0; i < numberOfThreads; i++) {
			drivers[i]->getThread()->start();
		}
	}

	void join() {
		for (int i=0; i < numberOfThreads; i++) {
			drivers[i]->getThread()->join();
		}
	}

	void report(bool silent=false) {
		if (!silent) std::cout << "DriverManager Report:" << std::endl ;
		driverTotalTime = 0;
		for (int i=0; i < numberOfOps; i++) {
			int opCount=0;
			double opTotalTime=0.0;
			for (int j=0; j < numberOfThreads; j++) {
				opCount += drivers[j]->getOpCounts()[i];
				opTotalTime += drivers[j]->getOpTotalTimes()[i];
			}
			if (!silent) std::cout << "  " << ops[i]->getOpName() << ":   count:" << opCount << "  totalTime:" << opTotalTime << "  timePerOp:" << (opTotalTime/(double)opCount) << std::endl;
			driverTotalTime += opTotalTime;
		}
		if (!silent) std::cout << "  totalTime:" << driverTotalTime << std::endl;

	}

	double getDriverTotalTime() { return driverTotalTime; }

};

} // namespace

#endif /* DRIVER_H_ */
