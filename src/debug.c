#include <stdio.h>
#include "str.h"

extern unsigned char signInputTable1[];
extern unsigned char signInputTable2[];

void PrintPhonemes(struct str *phonemeindex, struct str *phonemeLength,
                   struct str *stress) {
  int i = 0;
  printf("===========================================\n");

  printf("Internal Phoneme presentation:\n\n");
  printf(" idx    phoneme  length  stress\n");
  printf("------------------------------\n");

  while (G(phonemeindex,i) != 255) {

		if (G(phonemeindex,i) < 81) {
      printf(" %3i      %c%c      %3i       %i\n", G(phonemeindex,i),
             signInputTable1[G(phonemeindex,i)], signInputTable2[G(phonemeindex,i)],
             G(phonemeLength,i), G(stress,i));
    } else {
      printf(" %3i      ??      %3i       %i\n", G(phonemeindex,i),
             G(phonemeLength,i), G(stress,i));
    }
    i++;
  }
  printf("===========================================\n");
  printf("\n");
}

void PrintOutput(unsigned char *flag, unsigned char *f1, unsigned char *f2,
                 unsigned char *f3, unsigned char *a1, unsigned char *a2,
                 unsigned char *a3, unsigned char *p) {
  int i = 0;
  printf("===========================================\n");
  printf("Final data for speech output:\n\n");
  printf(" flags ampl1 freq1 ampl2 freq2 ampl3 freq3 pitch\n");
  printf("------------------------------------------------\n");
  while (i < 255) {
    printf("%5i %5i %5i %5i %5i %5i %5i %5i\n", flag[i], a1[i], f1[i], a2[i],
           f2[i], a3[i], f3[i], p[i]);
    i++;
  }
  printf("===========================================\n");
}

extern unsigned char GetRuleByte(unsigned short mem62, unsigned char Y);

void PrintRule(unsigned short offset) {
  unsigned char i = 1;
  unsigned char A = 0;
  printf("Applying rule: ");
  do {
    A = GetRuleByte(offset, i);
    if ((A & 127) == '=')
      printf(" -> ");
    else
      printf("%c", A & 127);
    i++;
  } while ((A & 128) == 0);
  printf("\n");
}
