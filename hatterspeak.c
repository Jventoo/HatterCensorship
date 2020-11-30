#include "bf.h"
#include "hash.h"
#include "hs.h"
#include "ll.h"
#include "parser.h"

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPRESSION "^[A-za-z'-]+"
#define HATTERSPEAK_FILE "hatterspeak.txt"
#define OLDSPEAK_FILE "oldspeak.txt"
#define DEFAULT_HASH_SIZE 10000
#define DEFAULT_BF_SIZE 1048576
#define DEFAULT_MTF 0

#define OPTIONS "sh:f:mb"

bool move_to_front = DEFAULT_MTF;
uint32_t bit_count = 0;
uint32_t seeks = 0;
uint32_t links_searched = 0;

int main(int argc, char **argv) {
  int c = 0, hash_len = DEFAULT_HASH_SIZE, bf_size = DEFAULT_BF_SIZE;
  bool mtf_supplied = false, suppress_letter = false;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 's':
      suppress_letter = true;
      break;
    case 'h':
      hash_len = atoi(optarg);
      break;
    case 'f':
      bf_size = atoi(optarg);
      break;
    case 'm':
      if (mtf_supplied) {
        puts("Error: cannot supply both -m and -b!");
        return -1;
      } else {
        mtf_supplied = true;
        move_to_front = true;
      }
      break;
    case 'b':
      if (mtf_supplied) {
        puts("Error: cannot supply both -m and -b!");
        return -1;
      } else {
        mtf_supplied = true;
        move_to_front = false;
      }
      break;
    }
  }

  BloomFilter *bf = bf_create(bf_size);
  if (!bf) {
    exit(1);
  }
  HashTable *ht = ht_create(hash_len);
  if (!ht) {
    exit(1);
  }

  // Linked lists for tracking bad input
  ListNode *nonsense_words = NIL;
  ListNode *hatterspeak_words = NIL;

  // Open oldspeak file, declare buffer, and parse in lowercase
  FILE *f_old = fopen(OLDSPEAK_FILE, "r");
  char word_old[4096];
  while (fscanf(f_old, "%s", word_old) == 1) {
    for (int i = 0; word_old[i]; i++) {
      word_old[i] = tolower(word_old[i]);
    }

    bf_insert(bf, word_old);
    ht_insert(ht, hs_create(word_old, "\0"));
  }
  fclose(f_old);

  // Parse hatterspeak.txt for pairs and insert into BF + HT
  FILE *f_hatter = fopen(HATTERSPEAK_FILE, "r");
  memset(word_old, 0, sizeof(word_old)); // Reuse old buffer
  char word_hatter[4096];
  while (fscanf(f_hatter, "%s %s", word_old, word_hatter) == 2) {
    // Convert pair to lowercase
    for (int i = 0; word_old[i]; i++) {
      word_old[i] = tolower(word_old[i]);
    }
    for (int i = 0; word_hatter[i]; i++) {
      word_hatter[i] = tolower(word_hatter[i]);
    }

    bf_insert(bf, word_old);
    ht_insert(ht, hs_create(word_old, word_hatter));
  }
  fclose(f_hatter);

  // Compile regex to parse stdin
  regex_t reg;
  regcomp(&reg, EXPRESSION, REG_EXTENDED);

  // Parse standard input, convert to lowercase
  char *input = NULL;
  while ((input = next_word(stdin, &reg)) != NULL) {
    for (int i = 0; input[i]; i++) {
      input[i] = tolower(input[i]);
    }

    // If word is in BF and HT, track it
    if (bf_probe(bf, input)) {
      ListNode *node = ht_lookup(ht, input);
      if (node) {
        // Create copy of hatterspeak data for tracking, simplifies frees
        HatterSpeak *gs_copy
            = hs_create(node->gs->oldspeak, node->gs->hatterspeak);

        if (node->gs->hatterspeak[0] == '\0') {
          nonsense_words = ll_insert(&nonsense_words, gs_copy);
        } else {
          hatterspeak_words = ll_insert(&hatterspeak_words, gs_copy);
        }
      }
    }
  }
  clear_words();
  regfree(&reg);

  // Calculate statistics
  if (suppress_letter) {
    double seek_avg = (double)links_searched / seeks;

    // Calculate average LL length
    double list_avg = 0.0;
    for (uint32_t i = 0; i < ht->length; i++) {
      ListNode *search = ht->heads[i];
      // Add the amount of nodes to the avg (node count)
      while (search) {
        list_avg += 1.0;
        search = search->next;
      }
    }
    list_avg /= ht->length; // Calculate final avg

    // HT Load (%) = (Size / length) * 100
    double ht_load = 100.0 * ht_count(ht) / ht->length;
    // BF Load (%) = (Amt of 1 bits / length) * 100
    double bf_load = 100.0 * bit_count / bf->filter->length;

    printf("Seeks: %" PRIu32 "\nAverage seek length: %f\n", seeks, seek_avg);
    printf("Average Linked List length: %f\n", list_avg);
    printf("Hash table load: %f%%\n", ht_load);
    printf("Bloom filter load: %f%%\n", bf_load);
  } else if (nonsense_words || hatterspeak_words) {
    puts("Dear Wonderlander,\n");

    if (nonsense_words) {
      puts("You have chosen to use words that the queen has decreed oldspeak.");
      puts("Due to your infraction you will be sent to the dungeon where you "
           "will be taught hatterspeak.");
      puts("\nYour errors:");

      ListNode *node = nonsense_words;
      while (node) {
        printf("\n%s", node->gs->oldspeak);
        node = node->next;
      }
    }

    if (hatterspeak_words) {
      if (!nonsense_words) {
        puts("The decree for hatterspeak finds your message lacking. Some of "
             "the words that you used are not hatterspeak.");
        puts("The list shows how to turn the oldspeak words into hatterspeak.");
      } else {
        puts("\n\nAppropriate hatterspeak translations.");
      }

      ListNode *node = hatterspeak_words;
      while (node) {
        printf("\n%s -> %s", node->gs->oldspeak, node->gs->hatterspeak);
        node = node->next;
      }
    }
    printf("\n");
  }

  ll_delete(nonsense_words);
  ll_delete(hatterspeak_words);
  bf_delete(bf);
  ht_delete(ht);
  return 0;
}
