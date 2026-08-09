#if !defined(LN8_H_INCLUDED)
#define LN8_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef struct ln8_an8 {
  uint8_t l;
  uint8_t a;
} ln8_an8;

static inline void
ln8_an8_set
  (
    ln8_an8* target,
    uint8_t l,
    uint8_t a
  )
{
  target->l = l;
  target->a = a;
}

static inline bool
ln8_an8_is_equal_to
  (
    const ln8_an8* this,
    const ln8_an8* other
  )
{ 
  return this->l == other->l
      && this->a == other->a;
}

#endif // LN8_H_INCLUDED
