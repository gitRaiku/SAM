#include "render.h"
#include <stdio.h>
#include <stdlib.h>

#include "str.h"

// CREATE TRANSITIONS
//
// Linear transitions are now created to smoothly connect each
// phoeneme. This transition is spread between the ending frames
// of the old phoneme (outBlendLength), and the beginning frames
// of the new phoneme (inBlendLength).
//
// To determine how many frames to use, the two phonemes are
// compared using the blendRank[] table. The phoneme with the
// smaller score is used. In case of a tie, a blend of each is used:
//
//      if blendRank[phoneme1] ==  blendRank[phomneme2]
//          // use lengths from each phoneme
//          outBlendFrames = outBlend[phoneme1]
//          inBlendFrames = outBlend[phoneme2]
//      else if blendRank[phoneme1] < blendRank[phoneme2]
//          // use lengths from first phoneme
//          outBlendFrames = outBlendLength[phoneme1]
//          inBlendFrames = inBlendLength[phoneme1]
//      else
//          // use lengths from the second phoneme
//          // note that in and out are swapped around!
//          outBlendFrames = inBlendLength[phoneme2]
//          inBlendFrames = outBlendLength[phoneme2]
//
//  Blend lengths can't be less than zero.
//
// For most of the parameters, SAM interpolates over the range of the last
// outBlendFrames-1 and the first inBlendFrames.
//
// The exception to this is the Pitch[] parameter, which is interpolates the
// pitch from the center of the current phoneme to the center of the next
// phoneme.

// From render.c
extern struct str *__restrict phonemeIndexOutput;  // tab47296
extern struct str *__restrict phonemeLengthOutput; // tab47416

// from RenderTabs.h
extern unsigned char blendRank[];
extern unsigned char outBlendLength[];
extern unsigned char inBlendLength[];

extern struct str *__restrict pitches;

extern struct str *__restrict frequency1;
extern struct str *__restrict frequency2;
extern struct str *__restrict frequency3;

extern struct str *__restrict amplitude1;
extern struct str *__restrict amplitude2;
extern struct str *__restrict amplitude3;

// written by me because of different table positions.
//  mem[47] = ...
//  168=pitches
//  169=frequency1
//  170=frequency2
//  171=frequency3
//  172=amplitude1
//  173=amplitude2
//  174=amplitude3
unsigned char Read(unsigned char p, unsigned char Y) {
  switch (p) {
  case 168:
    return G(pitches,Y);
  case 169:
    return G(frequency1,Y);
  case 170:
    return G(frequency2,Y);
  case 171:
    return G(frequency3,Y);
  case 172:
    return G(amplitude1,Y);
  case 173:
    return G(amplitude2,Y);
  case 174:
    return G(amplitude3,Y);
  default:
    printf("Error reading from tables");
    return 0;
  }
}

void Write(unsigned char p, unsigned char Y, unsigned char value) {
  switch (p) {
  case 168:
    strs(pitches, Y, value);
    return;
  case 169:
    strs(frequency1, Y, value);
    return;
  case 170:
    strs(frequency2, Y, value);
    return;
  case 171:
    strs(frequency3, Y, value);
    return;
  case 172:
    strs(amplitude1, Y, value);
    return;
  case 173:
    strs(amplitude2, Y, value);
    return;
  case 174:
    strs(amplitude3, Y, value);
    return;
  default:
    printf("Error writing to tables\n");
    return;
  }
}

// linearly interpolate values
void interpolate(uint32_t width, uint32_t table, uint32_t frame,
                 uint32_t mem53) {
  uint32_t sign = (mem53 < 0);
  uint32_t remainder = abs(mem53) % width;
  uint32_t div = mem53 / width;

  uint32_t error = 0;
  uint32_t pos = width;
  uint32_t val = Read(table, frame) + div;

  while (--pos) {
    error += remainder;
    if (error >= width) { // accumulated a whole integer error, so adjust output
      error -= width;
      if (sign)
        val--;
      else if (val)
        val++; // if input is 0, we always leave it alone
    }
    Write(table, ++frame, val); // Write updated value back to next frame.
    val += div;
  }
}

void interpolate_pitch(unsigned char pos, unsigned char mem49,
                       unsigned char phase3) {
  // unlike the other values, the pitches[] interpolates from
  // the middle of the current phoneme to the middle of the
  // next phoneme

  // half the width of the current and next phoneme
  unsigned char cur_width = G(phonemeLengthOutput,pos) / 2;
  unsigned char next_width = G(phonemeLengthOutput,pos + 1) / 2;
  // sum the values
  unsigned char width = cur_width + next_width;
  char pitch = G(pitches,next_width + mem49) - G(pitches,mem49 - cur_width);
  interpolate(width, 168, phase3, pitch);
}

unsigned char CreateTransitions() {
  unsigned char mem49 = 0;
  unsigned char pos = 0;
  while (1) {
    unsigned char next_rank;
    unsigned char rank;
    unsigned char speedcounter;
    unsigned char phase1;
    unsigned char phase2;
    unsigned char phase3;
    unsigned char transition;

    unsigned char phoneme = G(phonemeIndexOutput,pos);
    unsigned char next_phoneme = G(phonemeIndexOutput,pos + 1);

    if (next_phoneme == 255)
      break; // 255 == end_token

    // get the ranking of each phoneme
    next_rank = blendRank[next_phoneme];
    rank = blendRank[phoneme];

    // compare the rank - lower rank value is stronger
    if (rank == next_rank) {
      // same rank, so use out blend lengths from each phoneme
      phase1 = outBlendLength[phoneme];
      phase2 = outBlendLength[next_phoneme];
    } else if (rank < next_rank) {
      // next phoneme is stronger, so us its blend lengths
      phase1 = inBlendLength[next_phoneme];
      phase2 = outBlendLength[next_phoneme];
    } else {
      // current phoneme is stronger, so use its blend lengths
      // note the out/in are swapped
      phase1 = outBlendLength[phoneme];
      phase2 = inBlendLength[phoneme];
    }

    mem49 += G(phonemeLengthOutput,pos);

    speedcounter = mem49 + phase2;
    phase3 = mem49 - phase1;
    transition = phase1 + phase2; // total transition?

    if (((transition - 2) & 128) == 0) {
      unsigned char table = 169;
      interpolate_pitch(pos, mem49, phase3);
      while (table < 175) {
        // tables:
        // 168  pitches[]
        // 169  frequency1
        // 170  frequency2
        // 171  frequency3
        // 172  amplitude1
        // 173  amplitude2
        // 174  amplitude3

        char value = Read(table, speedcounter) - Read(table, phase3);
        interpolate(transition, table, phase3, value);
        table++;
      }
    }
    ++pos;
  }

  // add the length of this phoneme
  return mem49 + G(phonemeLengthOutput,pos);
}
