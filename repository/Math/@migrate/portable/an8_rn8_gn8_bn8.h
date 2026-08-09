#if !defined(AN8_RN8_GN8_BN8_H_INCLUDED)
#define AN8_RN8_GN8_BN8_H_INCLUDED

#include <stdint.h>

typedef struct an8_rn8_gn8_bn8 {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} an8_rn8_gn8_bn8;

static inline void
an8_rn8_gn8_bn8_set
  (
    an8_rn8_gn8_bn8* target,
    uint8_t a,
    uint8_t r,
    uint8_t g,
    uint8_t b
  )
{
  target->a = a;
  target->r = r;
  target->g = g;
  target->b = b;
}

#endif // AN8_RN8_GN8_BN8_H_INCLUDED
