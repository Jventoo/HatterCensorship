#include "bf.h"
#include "speck.h"
#include <stdlib.h>

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->primary[0] = 0xfc28ca6885711cf7;
    bf->primary[1] = 0x2841af568222f773;
    bf->secondary[0] = 0x85ae998311115ae3;
    bf->secondary[1] = 0xb6fac2ae33a40089;
    bf->tertiary[0] = 0xd37b01df0ae8f8d0;
    bf->tertiary[1] = 0x911d454886ca7cf7;
    bf->filter = bv_create(size);
    return bf;
  }
  return (BloomFilter *)NIL;
}

void bf_delete(BloomFilter *bf) {
  if (bf) {
    bv_delete(bf->filter);
    bf->filter = NIL;
  }
  free(bf);
  return;
}

void bf_insert(BloomFilter *bf, char *key) {
  // Do not need to do any bounds checking; bv functions already do
  uint32_t len = bf->filter->length;
  uint32_t idx = hash(bf->primary, key) % len;

  // Check for 0 bit to avoid double counting in case of false positive
  if (bv_get_bit(bf->filter, idx) == 0) {
    bit_count += 1;
    bv_set_bit(bf->filter, idx);
  }

  idx = hash(bf->secondary, key) % len;
  if (bv_get_bit(bf->filter, idx) == 0) {
    bit_count += 1;
    bv_set_bit(bf->filter, idx);
  }

  idx = hash(bf->tertiary, key) % len;
  if (bv_get_bit(bf->filter, idx) == 0) {
    bit_count += 1;
    bv_set_bit(bf->filter, idx);
  }

  return;
}

bool bf_probe(BloomFilter *bf, char *key) {
  uint32_t len = bf->filter->length;
  bool first = bv_get_bit(bf->filter, hash(bf->primary, key) % len);
  bool second = bv_get_bit(bf->filter, hash(bf->secondary, key) % len);
  bool third = bv_get_bit(bf->filter, hash(bf->tertiary, key) % len);

  return first && second && third;
}
