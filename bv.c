#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

BitVector *bv_create(uint32_t bit_len) {
  BitVector *v = (BitVector *)malloc(sizeof(BitVector));

  if (v) {
    v->length = bit_len;

    if (bit_len % 8 == 0) {
      v->vector = (uint8_t *)calloc(bit_len / 8, sizeof(uint8_t));
    } else {
      v->vector = (uint8_t *)calloc(bit_len / 8 + 1, sizeof(uint8_t));
    }
    return v;
  }
  return NULL;
}

void bv_delete(BitVector *v) {
  free(v->vector);
  v->vector = NULL;
  free(v);
  return;
}

uint32_t bv_get_len(BitVector *v) {
  return v ? v->length : 0;
}

void bv_set_bit(BitVector *v, uint32_t i) {
  if (v && i < v->length && v->vector) {
    uint8_t x = 1;
    x = x << (7 - (i % 8));

    uint8_t *y = v->vector + i / 8;
    *y = *y | x;
  }
  return;
}

void bv_clr_bit(BitVector *v, uint32_t i) {
  if (v && i < v->length && v->vector) {
    uint8_t x = 1;
    x = x << (7 - (i % 8));
    x = ~x;

    uint8_t *y = v->vector + i / 8;
    *y = *y & x;
  }
  return;
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  if (v && i < v->length && v->vector) {
    uint8_t x = 1;
    uint8_t y = v->vector[i / 8];
    x = x << (7 - (i % 8));
    x = x & y;
    x = x >> (7 - (i % 8));
    return x;
  }
  return 0;
}

void bv_set_all_bits(BitVector *v) {
  if (v && v->vector) {
    uint32_t bounds = v->length / 8;
    if (v->length % 8 != 0) {
      bounds++;
    }

    for (uint32_t i = 0; i < bounds; i++) {
      v->vector[i] = ~(v->vector[i]);
    }
  }
  return;
}
