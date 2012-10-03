/*
 * map.h
 *
 *  Created on: Jul 7, 2012
 *      Author: arietal
 */

#ifndef MAP_H_
#define MAP_H_

#include "binarytree/redblacktree.h"

using namespace yatl;

namespace yatl {

template <typename KeyType, typename DataType,typename mapImpl=RedBlackTree<KeyType,DataType> >
class Map {
	mapImpl _map;
public:
	bool put(KeyType key, DataType data) {
		if (_map.searchNode(key))
			return false;
		typename mapImpl::NodeType *node = new typename mapImpl::NodeType(key, data);
		_map.insertNode(node);
		return true;
	}

	bool remove(KeyType key) {
		typename mapImpl::NodeType *node = _map.searchNode(key);
		if (!node)
			return false;
		_map.deleteNode(node);
		return true;
	}

	DataType get(KeyType key) {
		typename mapImpl::NodeType *node = _map.searchNode(key);
		if (node)
			return node->getData();
		return (DataType)0;
	}

	bool containsKey(KeyType key) {
		return (_map.searchNode(key)) ? true : false;
	}
};

} // namespace

#endif /* MAP_H_ */
