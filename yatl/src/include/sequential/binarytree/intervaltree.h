/*
 * intervaltree.h
 *
 *  Created on: Jul 5, 2012
 *      Author: arietal
 */

#ifndef INTERVALTREE_H_
#define INTERVALTREE_H_

#include "redblacktree.h"

using namespace yatl;

namespace yatl {

template <typename RangeType>
struct Interval {
	RangeType low, high;
	Interval(RangeType low, RangeType high) : low(low), high(high) {}
	Interval() {}
	bool getOverlaps(const Interval& interval) {
		return low <= interval.high && high >= interval.low;
	}
	Interval<RangeType>& operator=(const Interval<RangeType> &o) {
		low = o.low;
		high = o.high;
		return *this;
	}
};

template <typename RangeType>
class _IntervalAugment {
	Interval<RangeType> _interval;
	RangeType _max;
public:
	void printAugment(ostream &os) { os << "[" << _interval.low << " - " << _interval.high << ", max: " << _max << "]"; }
	void setInterval(Interval<RangeType> interval) {
		_interval.low = interval.low;
		_interval.high = interval.high;
		_max = interval.high;
	}
	void setMax(RangeType max) {
		_max = max;
	}

	RangeType getMax() {
		return _max;
	}

	Interval<RangeType>& getInterval() { return _interval ; }

	bool updateMax(_IntervalAugment<RangeType> *v1, _IntervalAugment<RangeType> *v2) {
		RangeType oldMax = _max;
		_max = _interval.high;
		if (v1 != NULL && v1->getMax() > _max) _max = v1->getMax();
		if (v2 != NULL && v2->getMax() > _max) _max = v2->getMax();
		return _max != oldMax;
	}

	bool update(void *left, void *right) {
		_IntervalAugment<RangeType> *v1 = static_cast<_IntervalAugment<RangeType> *>(left);
		_IntervalAugment<RangeType> *v2 = static_cast<_IntervalAugment<RangeType> *>(right);
		return updateMax(v1,v2);
	}
};


template <typename KeyType, typename DataType>
class IntervalTree {
public:
	class IntervalType : public BinaryTreeNode<KeyType,DataType,_RedBlackExtend<_IntervalAugment<KeyType> > > {
	public:
		IntervalType(const Interval<KeyType>& interval, DataType data) :
			BinaryTreeNode<KeyType,DataType,_RedBlackExtend<_IntervalAugment<KeyType> > >(interval.low,data) {
            BinaryTreeNode<KeyType,DataType,_RedBlackExtend<_IntervalAugment<KeyType> > >::setInterval(interval);
		}
	};
	void intervalInsert(IntervalType *interval) {
		_redBlackTree.insertNode(interval);
	}

	void intervalDelete(IntervalType *interval) {
		_redBlackTree.deleteNode(interval);
	}

	IntervalType *intervalSearch(const Interval<KeyType>& interval) {
		IntervalType *node = (IntervalType *)_redBlackTree.getRoot();
		while (node != NULL && !(node->getInterval()).getOverlaps(interval)) {
			IntervalType *n = (IntervalType *)node->get(dir_left_child);
			if (n != NULL && n->getMax() >= interval.low)
				node = n;
			else
				node = (IntervalType *)node->get(dir_right_child);
		}
		return node;
	}

	void printTree(ostream& os) {
		_redBlackTree.printTree(os);
	}
private:
	RedBlackTree<KeyType,DataType,_IntervalAugment<KeyType> > _redBlackTree;
};

} // namespace

#endif /* INTERVALTREE_H_ */
