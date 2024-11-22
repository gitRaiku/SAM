#ifndef STR_H
#define STR_H
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CS(name)                                                               \
  struct str _ ##name;                                                          \
  struct str *__restrict name = &_ ##name;

struct str {
  uint8_t *__restrict s;
  uint32_t c, m;
};

uint32_t __inline__ __attribute((pure)) max(uint32_t o1, uint32_t o2) { return o1 > o2 ? o1 : o2; }
#define IMAX(a, b) { (a) = max(a, b); }

static uint8_t G(struct str *__restrict s, uint32_t p) {
  return (p >= s->m) ? 0 : s->s[p];
}

static void strs(struct str *__restrict s, uint32_t p, char c) {
  if (s->m == 0) {
    s->m = 4;
    s->s = calloc(s->m, 1);
  }
  while (p >= s->m) {
    s->m *= 2;
    s->s = realloc(s->s, s->m);
    memset(s->s + s->m / 2, 0, s->m / 2);
  }
  s->s[p] = c;
}

static void strc(struct str *__restrict s, char *__restrict ss) { 
  if (s->s) { s->c = strlen(s->s); }
  while (*ss != 0) { strs(s, s->c, *ss); ++ss; ++s->c; } 
}

#endif
