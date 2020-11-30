#include "hash.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashTable *ht_create(uint32_t length) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht) {
    ht->salt[0] = 0x85ae998311115ae3;
    ht->salt[1] = 0xb6fac2ae33a40089;
    ht->length = length;
    ht->heads = (ListNode **)calloc(length, sizeof(ListNode *));
    return ht;
  }
  return (HashTable *)NIL;
}

void ht_delete(HashTable *ht) {
  if (ht) {
    for (uint32_t i = 0; i < ht->length; i++) {
      ll_delete(ht->heads[i]);
    }
    free(ht->heads);
  }
  free(ht);
  ht = NIL;
  return;
}

uint32_t ht_count(HashTable *ht) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < ht->length; i++) {
    if (ht->heads[i]) {
      count += 1;
    }
  }
  return count;
}

ListNode *ht_lookup(HashTable *ht, char *key) {
  uint32_t pos = hash(ht->salt, key) % ht->length;
  ListNode **head = &(ht->heads[pos]);
  return ll_lookup(head, key);
}

void ht_insert(HashTable *ht, HatterSpeak *gs) {
  uint32_t pos = hash(ht->salt, gs->oldspeak) % ht->length;
  ListNode **head = &(ht->heads[pos]);
  ListNode *node = ll_lookup(head, gs->oldspeak);

  if (node) { // Found duplicate node, clear the memory
    if (node->gs->hatterspeak[0] == '\0') {
      strcpy(node->gs->hatterspeak, gs->hatterspeak);
    }
    hs_delete(gs);
    gs = NIL;
  } else { // Unique node, insert it
    *head = ll_insert(head, gs);
  }
  return;
}
