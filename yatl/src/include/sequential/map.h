/*
 * map.h
 *
 *  Created on: Jul 7, 2012
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
