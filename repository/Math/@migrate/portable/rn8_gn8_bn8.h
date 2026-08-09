#if !defined(RN8_GN8_BN8_H_INCLUDED)
#define RN8_GN8_BN8_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef struct rn8_gn8_bn8 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rn8_gn8_bn8;

static inline void
rn8_gn8_bn8_set
  (
    rn8_gn8_bn8* target,
    uint8_t r,
    uint8_t g,
    uint8_t b
  )
{
  target->r = r;
  target->g = g;
  target->b = b;
}

static inline bool
rn8_gn8_bn8_is_equal_to
  (
    const rn8_gn8_bn8* this,
    const rn8_gn8_bn8* other
  )
{ 
  return this->r == other->r
      && this->g == other->g
      && this->b == other->b;
}

#endif // RN8_GN8_BN8_H_INCLUDED
