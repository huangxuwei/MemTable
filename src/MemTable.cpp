#include "MemTable.hpp"

#include <assert.h>
#include <stdio.h>

void MemTable::removeNode(MemNode* node) {
	if (!node)
		return;

	MemNode *prev = node->getPrevNode();
	if (!prev)
		return;

	if (node->hole_follows) {
		assert(node->getHoldStart() != node->getHoldEnd());
		list_del(&node->hole_stack);
	} else {
		assert(node->getHoldStart() == node->getHoldEnd());
	}

	if (!prev->hole_follows) {
		prev->hole_follows = 1;
		list_add(&prev->hole_stack, &hole_stack);
	} else {
		list_move(&prev->hole_stack, &hole_stack);
	}

	list_del(&node->node_list);

	printf("Destroy Node: %p start: %lu size: %lu hashole: %d\n",
						&node->node_list, node->start, node->size,
						node->hole_follows);

	delete node;
	node = nullptr;
}

MemNode* MemTable::addNode(size_t size) {
	MemNode* holenode = searchHoleStack(size);
	if (!holenode)
		return nullptr;

	MemNode* newNode = new MemNode;
	if (!newNode)
		return nullptr;

	insertNode(holenode, newNode, size);
	return newNode;
}

void MemTable::insertNode(MemNode* hole_node, MemNode* node,
		size_t size, enum allocator_flags flags) {
	size_t hole_start = hole_node->getHoldStart();
	size_t hole_end = hole_node->getHoldEnd();
	size_t adj_start = hole_start;
	size_t adj_end = hole_end;

	if (flags & CREATE_TOP)
		adj_start = adj_end - size;

	assert(adj_start >= hole_start);

	if (adj_start == hole_start) {
		hole_node->hole_follows = 0;
		list_del(&hole_node->hole_stack);
	}

	node->start = adj_start;
	node->size = size;

	list_init(&node->hole_stack);
	list_add(&node->node_list, &hole_node->node_list);

	assert(node->start + node->size <= adj_end);

	node->hole_follows = 0;
	if (node->getHoldStart() < hole_end) {
		list_add(&node->hole_stack, &hole_stack);
		node->hole_follows = 1;
	}
}

MemNode* MemTable::searchHoleStack(size_t size,
		enum search_flags flags) {
	MemNode *best = nullptr;
	Node *entry = nullptr;
	size_t best_size = ~0UL;
	size_t hole_start = 0;
	size_t hole_end = 0;
	size_t hole_size = 0;

	bool isbackwards = flags & SEARCH_BELOW;

	entry = container_of((isbackwards) ? \
				 hole_stack.prev : hole_stack.next, Node, hole_stack);

	while (entry && (&entry->hole_stack != &hole_stack)) {
		MemNode * memnode = dynamic_cast<MemNode*>(entry);
		if (!memnode)
			return nullptr;

		hole_start = memnode->getHoldStart();
		hole_end = memnode->getHoldEnd();
		hole_size = hole_end - hole_start;

		if (hole_size < size)
			continue;

		if (!(flags & SEARCH_BEST))
			return memnode;

		if (hole_size < best_size) {
			best = memnode;
			best_size = hole_size;
		}

		entry = container_of((isbackwards) ? \
				     entry->hole_stack.prev : entry->hole_stack.next, Node, hole_stack);
	}

	return best;
}

void MemTable::dump() {
	Node *entry = container_of(head_node.node_list.next, Node, node_list);

	while (entry && (&entry->node_list != &head_node.node_list)) {
		printf("INFO Node: %p start: %lu size: %lu hashole: %d\n",
				&entry->node_list, entry->start, entry->size,
				entry->hole_follows);
		entry = container_of(entry->node_list.next, Node, node_list);
	}
}

void MemTable::cleanup() {
	Node *entry = container_of(head_node.node_list.next, Node, node_list);

	while (entry && (&entry->node_list != &head_node.node_list)) {
		Node *temp = container_of(entry->node_list.next, Node, node_list);

		MemNode *memnode = dynamic_cast<MemNode*>(entry);
		if (memnode)
			removeNode(memnode);

		entry = temp;
	}
}
