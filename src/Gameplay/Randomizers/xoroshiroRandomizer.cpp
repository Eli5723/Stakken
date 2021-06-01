#include "./xoroshiroRandomizer.h"

/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
   It has excellent (sub-ns) speed, a state (256 bits) that is large
   enough for any parallel application, and it passes all tests we are
   aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

#include <SDL2/SDL.h>


void xoroshiroRandomizer::seed(int s1){
    s[0] = s1;
    s[1] = s1 - 2354234;
    s[2] = s1 - 7423145;
    s[3] = s1 - 10;
}

uint64_t xoroshiroRandomizer::rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

uint64_t xoroshiroRandomizer::next(void) {
    const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
}