#ifndef __HASH_H__
#define __HASH_H__

#ifndef NIL
#define NIL (void *)0
#endif

#include "ll.h"
#include <inttypes.h>

//
// Struct definition for a HashTable.
//
// salt:    Salt used for hashing
// length:  Maximum number of entries in the HT.
// heads:   Array of Linked List heads.
//
typedef struct HashTable {
  uint64_t salt[2];
  uint32_t length;
  ListNode **heads;
} HashTable;

//
// Constructor for a HashTable.
//
// length:  Length of the HashTable.
//
HashTable *ht_create(uint32_t length);

//
// Destructor for a HashTable.
//
// ht:  The HashTable.
//
void ht_delete(HashTable *ht);

//
// Returns number of entries in HashTable.
//
// ht:  The HashTable.
//
uint32_t ht_count(HashTable *ht);

//
// Searches a HashTable for a key.
// Returns the ListNode if found (NULL otherwise).
// Wrapper for the ll_lookup() function.
//
// ht:      The HashTable.
// key:     Key to search for.
//
ListNode *ht_lookup(HashTable *ht, char *key);

//
// First creates a new ListNode from HatterSpeak.
// The created ListNode is then inserted into a HashTable.
// This should call the ll_insert() function.
//
// ht:  The HashTable.
// gs:  The HatterSpeak data to add to the HashTable.
//
void ht_insert(HashTable *ht, HatterSpeak *gs);

#endif
