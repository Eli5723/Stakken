#pragma once
#include <stdint.h>

struct xoroshiroRandomizer {
    uint64_t s[4];

    void seed(int s1);

    inline uint64_t rotl(const uint64_t x, int k);
    uint64_t next(void);
};