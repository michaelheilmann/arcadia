#if !defined(BN8_GN8_RN8_H_INCLUDED)
#define BN8_GN8_RN8_H_INCLUDED

#include <stdint.h>

typedef struct bn8_gn8_rn8 {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} bn8_gn8_rn8;

static inline void
bn8_gn8_rn8_set
  (
    bn8_gn8_rn8* target,
    uint8_t b,
    uint8_t g,
    uint8_t r
  )
{
  target->b = b;
  target->g = g;
  target->r = r;
}

#endif // BN8_GN8_RN8_H_INCLUDED
