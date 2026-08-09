#if !defined(LN8_H_INCLUDED)
#define LN8_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef struct ln8 {
  uint8_t l;
} ln8;

static inline void
ln8_set
  (
    ln8* target,
    uint8_t l
  )
{
  target->l = l;
}

static inline bool
ln8_is_equal_to
  (
    const ln8* this,
    const ln8* other
  )
{ 
  return this->l == other->l;
}

#endif // LN8_H_INCLUDED
