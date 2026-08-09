#if !defined(AN8_LN8_H_INCLUDED)
#define AN8_LN8_H_INCLUDED

#include <stdint.h>

typedef struct an8_ln8 {
  uint8_t a;
  uint8_t l;
} an8_ln8;

static inline void
an8_ln8_set
  (
    an8_ln8* target,
    uint8_t a,
    uint8_t l
  )
{
  target->a = a;
  target->l = l;
}

static inline bool
an8_ln8_is_equal_to
  (
    const an8_ln8* this,
    const an8_ln8* other
  )
{ 
  return this->a == other->a
      && this->l == other->l;
}

#endif // AN8_LN8_H_INCLUDED
