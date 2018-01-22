#ifndef _MEMNODE_HPP_
#define _MEMNODE_HPP_

#include <list>
#include <assert.h>

#include "MemList.hpp"

class Node {
public:
	Node() {}
	virtual ~Node() {}

public:
	list_head node_list;
	list_head hole_stack;
	size_t start = 0;
	size_t size = 0;
	bool hole_follows = 1;
};

class MemNode: public Node {
public:
	MemNode() {}
	virtual ~MemNode() {}

public:
	MemNode* getPrevNode()
	{
		Node* prev = container_of(node_list.prev, Node, node_list);
		return dynamic_cast<MemNode*>(prev);
	}
	MemNode* getNextNode()
	{
		Node* next = container_of(node_list.next, Node, node_list);
		return dynamic_cast<MemNode*>(next);
	}
	size_t getHoldStart()
	{
		return start + size;
	}
	size_t getHoldEnd()
	{
		Node* next = container_of(node_list.next, Node, node_list);
		assert(next != nullptr);
		return next->start;
	}
};

#endif
