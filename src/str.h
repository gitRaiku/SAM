#ifndef STR_H
#define STR_H
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CS(name)                                                               \
  struct str _##name;                                                          \
  struct str *__restrict name = &_##name;

struct str;

static void strs(struct str *__restrict s, uint32_t p, char c) {
  if (m == 0) {
    s->m = 4;
    s->s = calloc(s->m, 1);
  }
  while (p >= m) {
    s->m *= 2;
    s->s = realloc(s->s, s->m);
    memset(s->s + s->m / 2, 0, s->m / 2);
  }
  s->s[p] = c;
}

static void strc(struct str *__restrict s, char *__restrict ss) { while (*ss != 0) { strs(s, s->c, *ss); ++ss; } }

#endif
