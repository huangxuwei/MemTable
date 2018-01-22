#ifndef _MEMLIST_HPP_
#define _MEMLIST_HPP_

#include <cstddef>

#define _offsetof(type, member) ((size_t) &((type *)0)->member)

#define container_of(ptr, type, member) ({ \
        const decltype(((type *)0)->member)*__mptr = (ptr); \
        (type *)((char *)__mptr - _offsetof(type, member)); })

struct list_head {
	list_head *prev, *next;
};

void list_del(list_head* entry);
void list_add(list_head* entry, list_head* head, bool tonext = true);
void list_move(list_head* entry, list_head* head);
void list_init(list_head* entry);

#endif
