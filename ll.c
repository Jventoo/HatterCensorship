#include "ll.h"
#include <stdlib.h>
#include <string.h>

ListNode *ll_node_create(HatterSpeak *gs) {
  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  if (node) {
    node->gs = gs;
    node->next = NIL;
    return node;
  }
  return (ListNode *)NIL;
}

void ll_node_delete(ListNode *n) {
  hs_delete(n->gs);
  free(n);
  n = NIL;
  return;
}

void ll_delete(ListNode *head) {
  ListNode *delete = head;
  ListNode *next;
  while (delete) {
    next = delete->next;
    ll_node_delete(delete);
    delete = next;
  }
  head = NIL;
  return;
}

ListNode *ll_insert(ListNode **head, HatterSpeak *gs) {
  ListNode *node = ll_node_create(gs);
  if (node) {
    node->next = *head;
    *head = node;
    return node;
  }
  return (ListNode *)NIL;
}

ListNode *ll_lookup(ListNode **head, char *key) {
  seeks += 1;

  // If we have an invalid list, return null
  if (!head || !(*head)) {
    return NIL;
  }
  // Check the first node to avoid missing it in main loop
  links_searched += 1;
  if (strcmp((*head)->gs->oldspeak, key) == 0) {
    return *head;
  }

  // Search the rest of the lists if length > 1, return NIL if length = 1
  ListNode *search = (*head)->next;
  ListNode *prev = *head;
  while (search) {
    links_searched += 1;
    if (strcmp(search->gs->oldspeak, key) == 0) {
      if (move_to_front) {
        prev->next = search->next;
        search->next = *head;
        *head = search;
      }
      return search;
    }
    prev = search;
    search = search->next;
  }
  return NIL;
}
