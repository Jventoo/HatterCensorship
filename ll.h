#ifndef __LL_H__
#define __LL_H__

#ifndef NIL
#define NIL (void *)0
#endif

#include "hs.h"
#include <inttypes.h>
#include <stdbool.h>

extern bool move_to_front;
extern uint32_t seeks;
extern uint32_t links_searched;

typedef struct ListNode ListNode;

//
// Struct definition of a ListNode.
//
// gs:		Hatterspeak struct containing oldspeak and hatterspeak.
// next:	Pointer to next node in the Linked List.
//
struct ListNode {
  HatterSpeak *gs;
  ListNode *next;
};

//
// Constructor for a ListNode.
//
// gs:  HatterSpeak struct containing oldspeak and hatterspeak.
//
ListNode *ll_node_create(HatterSpeak *gs);

//
// Destructor for a ListNode.
//
// n:   ListNode to free.
//
void ll_node_delete(ListNode *n);

//
// Destructor for a linked list.
//
// head:    Head of the list.
//
void ll_delete(ListNode *head);

//
// Creates and inserts a ListNode into the list.
//
// head:    Head of linked list to insert into.
// gs:      HatterSpeak struct.
//
ListNode *ll_insert(ListNode **head, HatterSpeak *gs);

//
// Searches for a specific key in a linked list.
// Returns the ListNode found (NULL otherwise).
//
// head:    Head of linked list to search.
// key:     Key to search for (oldspeak).
//
ListNode *ll_lookup(ListNode **head, char *key);

#endif
