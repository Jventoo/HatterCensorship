#include "hs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HatterSpeak *hs_create(char *old, char *hatter) {
  HatterSpeak *hs = (HatterSpeak *)malloc(sizeof(HatterSpeak));
  if (hs) {
    // Allocate enough spaces for both strings
    hs->oldspeak = (char *)calloc(strlen(old) + 1, sizeof(char));
    hs->hatterspeak = (char *)calloc(strlen(hatter) + 1, sizeof(char));

    // Copy into the strings if calloc succeeded
    if (hs->oldspeak) {
      strcpy(hs->oldspeak, old);
    }
    if (hs->hatterspeak) {
      strcpy(hs->hatterspeak, hatter);
    }
    return hs;
  }
  return (HatterSpeak *)NIL;
}

void hs_delete(HatterSpeak *hs) {
  if (hs) {
    free(hs->oldspeak);
    free(hs->hatterspeak);
  }
  free(hs);
  hs = NIL;
  return;
}
