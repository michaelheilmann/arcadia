#if !defined(BN8_GN8_RN8_AN8_H_INCLUDED)
#define BN8_GN8_RN8_AN8_H_INCLUDED

#include <stdint.h>

typedef struct bn8_gn8_rn8_an8 {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} bn8_gn8_rn8_an8;

static inline void
bn8_gn8_rn8_an8_set
  (
    bn8_gn8_rn8_an8* target,
    uint8_t b,
    uint8_t g,
    uint8_t r,
    uint8_t a
  )
{
  target->b = b;
  target->g = g;
  target->r = r;
  target->a = a;
}

static inline bool
bn8_gn8_rn8_an8_is_equal_to
  (
    const bn8_gn8_rn8_an8* this,
    const bn8_gn8_rn8_an8* other
  )
{ 
  return this->b == other->b
      && this->g == other->g
      && this->r == other->r
      && this->a == other->a;
}

#endif // BN8_GN8_RN8_AN8_H_INCLUDED
