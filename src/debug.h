#ifndef DEBUG_H
#define DEBUG_H
#include "str.h"

void PrintPhonemes(struct str *phonemeindex, struct str *phonemeLength,
                   struct str *stress);
void PrintOutput(struct str *flag, struct str *f1, struct str *f2,
                 struct str *f3,   struct str *a1, struct str *a2,
                 struct str *a3,   struct str *p);

void PrintRule(unsigned short offset);

#endif
