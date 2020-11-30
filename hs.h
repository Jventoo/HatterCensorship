#ifndef __HS_H__
#define __HS_H__

#ifndef NIL
#define NIL (void *)0
#endif

typedef struct HatterSpeak {
  char *oldspeak;
  char *hatterspeak;
} HatterSpeak;

//
// Constructor for a HatterSpeak structure.
//
// old:     String containing oldspeak word
// hatter:  String containing hatterspeak translation (if applicable)
//
HatterSpeak *hs_create(char *old, char *hatter);

//
// Destructor for a HatterSpeak structure.
//
// hs:  The HatterSpeak structure
//
void hs_delete(HatterSpeak *hs);

#endif
