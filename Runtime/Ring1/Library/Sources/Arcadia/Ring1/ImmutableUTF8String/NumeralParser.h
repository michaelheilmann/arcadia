#if !defined(ARCADIA_RING1_IMMUTABLEUTF8STRING_NUMERALPARSER_H_INCLUDED)
#define ARCADIA_RING1_IMMUTABLEUTF8STRING_NUMERALPARSER_H_INCLUDED

#include "Arcadia/Ring1/Boolean.h"
#include "Arcadia/Ring1/Natural32.h"
#include "Arcadia/Ring1/Process.h"
#include "Arcadia/Ring1/StaticAssert.h"
#include <assert.h>

#define _Unicode_CodePoint_Last (0x0010ffff)
Arcadia_StaticAssert(_Unicode_CodePoint_Last <= Arcadia_Natural32Value_Maximum, "<internal error>");

#define _Unicode_CodePoint_Start (_Unicode_CodePoint_Last + 1)
Arcadia_StaticAssert(_Unicode_CodePoint_Start <= Arcadia_Natural32Value_Maximum, "<internal error>");

#define _Unicode_CodePoint_End (_Unicode_CodePoint_Last + 2)
Arcadia_StaticAssert(_Unicode_CodePoint_End <= Arcadia_Natural32Value_Maximum, "<internal error>");

#define _Unicode_CodePoint_Error (_Unicode_CodePoint_Last + 3)
Arcadia_StaticAssert(_Unicode_CodePoint_Error <= Arcadia_Natural32Value_Maximum, "<internal error>");

typedef struct _State {
  uint32_t codePoint;
  const uint8_t* start;
  const uint8_t* end;
  const uint8_t* current;
} _State;

static inline void
_State_init
  (
    _State* state,
    const uint8_t* start,
    size_t length
  )
{
  state->start = start;
  state->current = start;
  state->end = start + length;
  state->codePoint = _Unicode_CodePoint_Start;
}

static inline void
_State_uninit
  (
    _State* state
  )
{/*Intentionally empty.*/}

static void
next
  (
    _State* state
  )
{
  if (state->codePoint == _Unicode_CodePoint_End || state->codePoint == _Unicode_CodePoint_Error) {
    return;
  }
  if (state->current == state->end) {
    state->codePoint = _Unicode_CodePoint_End;
  } else {
    assert(state->current != state->end);
    state->codePoint = *state->current;
    if (state->codePoint > 0x7f) {
      // We only allow 1 Byte sequences.
      state->codePoint = _Unicode_CodePoint_Error;
    } else {
      state->current++;
    }
  }
}

static Arcadia_BooleanValue
isEnd
  (
    _State* state
  )
{ return (_Unicode_CodePoint_End == state->codePoint); }

static Arcadia_BooleanValue
isZero
  (
    _State* state
  )
{
  return ('0' == state->codePoint);
}

static Arcadia_BooleanValue
isDigit
  (
    _State* state
  )
{
  return ('0' <= state->codePoint)
      && (state->codePoint <= '9');
}

static Arcadia_BooleanValue
isSign
  (
    _State* state
  )
{
  return ('+' == state->codePoint)
      || ('-' == state->codePoint);
}

static Arcadia_BooleanValue
isPlus
  (
    _State* state
  )
{
  return ('+' == state->codePoint);
}

static Arcadia_BooleanValue
isMinus
  (
    _State* state
  )
{
  return ('-' == state->codePoint);
}

static Arcadia_BooleanValue
isPeriod
  (
    _State* state
  )
{ return ('.' == state->codePoint); }

static Arcadia_BooleanValue
isExponentPrefix
  (
    _State* state
  )
{
  return ('e' == state->codePoint)
      || ('E' == state->codePoint);
}

static Arcadia_BooleanValue
is
  (
    _State* state,
    uint32_t x
  )
{ return x == state->codePoint; }

#endif // ARCADIA_RING1_IMMUTABLEUTF8STRING_NUMERALPARSER_H_INCLUDED
