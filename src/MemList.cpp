#include "MemList.hpp"

void list_del(list_head* entry) {
	list_head *prev = entry->prev;
	list_head *next = entry->next;
	next->prev = prev;
	prev->next = next;
	prev = nullptr;
	next = nullptr;
}

void list_add(list_head* entry, list_head* head, bool tonext) {
	list_head *prev = head;
	list_head *next = head->next;
	if (!tonext) {
		prev = head->prev;
		next = head;
	}
	next->prev = entry;
	entry->next = next;
	entry->prev = prev;
	prev->next = entry;
}

void list_move(list_head* entry, list_head* head) {
	list_head *prev = entry->prev;
	list_head *next = entry->next;
	next->prev = prev;
	prev->next = next;
	list_add(entry, head);
}

void list_init(list_head* entry) {
	entry->next = entry;
	entry->prev = entry;
}
