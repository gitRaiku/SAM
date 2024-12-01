#ifndef RENDER_H
#define RENDER_H

#include "str.h"

void Render();
void SetMouthThroat(uint32_t mouth, uint32_t throat);

void ProcessFrames(uint32_t mem48);
void RenderSample(uint32_t *mem66, uint32_t consonantFlag,
                  uint32_t mem49);
uint32_t CreateTransitions();

#define PHONEME_PERIOD (1)
#define PHONEME_QUESTION (2)

#define RISING_INFLECTION (1)
#define FALLING_INFLECTION (255)

#endif
