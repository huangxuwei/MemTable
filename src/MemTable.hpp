#ifndef _MEMTABLE_HPP_
#define _MEMTABLE_HPP_

#include "MemNode.hpp"

class MemTable {
public:
	MemTable(size_t start, size_t size)
	{
		list_init(&hole_stack);
		list_init(&head_node.node_list);
		list_init(&head_node.hole_stack);
		head_node.hole_follows = 1;
		head_node.start = start + size;
		head_node.size = start - head_node.start;
		list_add(&head_node.hole_stack, &hole_stack, false);
	}
	~MemTable()
	{
		cleanup();
	}
	void removeNode(MemNode* node);
	MemNode* addNode(size_t size);
	void dump();

private:
	enum search_flags {
		SEARCH_DEFAULT =            0,
		SEARCH_BEST =		    1,
		SEARCH_BELOW =		    2,
	};
	enum allocator_flags {
		CREATE_DEFAULT =            0,
		CREATE_TOP =		    1,
	};
	list_head hole_stack;
	MemNode head_node;

private:
	MemNode* searchHoleStack(size_t size,
			enum search_flags flags = SEARCH_DEFAULT);
	void insertNode(MemNode* hole_node, MemNode* node,
			size_t size, enum allocator_flags flags = CREATE_DEFAULT);
	void cleanup();
};

#endif
