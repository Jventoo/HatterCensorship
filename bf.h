#ifndef __BF_H__
#define __BF_H__

#ifndef NIL
#define NIL (void *)0
#endif

#include "bv.h"
#include <inttypes.h>
#include <stdbool.h>

extern uint32_t bit_count;

//
// Struct definition for a Bloom Filter.
//
// primary:     Primary hash salt.
// secondary:   Secondary hash salt.
// tertiary:    Tertiary hash salt.
// filter:      BitVector that determines membership of a key.
//
typedef struct BloomFilter {
  uint64_t primary[2];
  uint64_t secondary[2];
  uint64_t tertiary[2];
  BitVector *filter;
} BloomFilter;

//
// Constructor for a BloomFilter.
//
// size:    Number of entries in the BF.
//
BloomFilter *bf_create(uint32_t size);

//
// Destructor for a BloomFilter.
//
// bf:  The BF.
//
void bf_delete(BloomFilter *bf);

//
// Inserts a new key into the BloomFilter.
// Indices to set bits are given by the hash functions.
//
// bf:      The BF.
// key:     Key to insert.
//
void bf_insert(BloomFilter *bf, char *key);

//
// Probes a BloomFilter to check if a key is present.
//
// bf:      The BF.
// key:     The key to check membership of.
//
bool bf_probe(BloomFilter *bf, char *key);

#endif
