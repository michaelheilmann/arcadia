/**
 * @file maid-reference-counter.h
 * @brief Source file of the maid_referencce_counter type.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#include "maid-reference-counter.h"

#include <inttypes.h>

typedef int64_t __atomic_i64;

#if defined(_WIN32)

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#endif

// Perform atomically:
// @code
// if (*variable@old == expected) *variable@new = desired;
// return *variable@old
// @endcode
static inline int64_t
__atomic_i64_compare_exchange
  (
    __atomic_i64* variable,
    __atomic_i64 expected,
    __atomic_i64 desired
  )
{
#if defined(_WIN32)
  return InterlockedCompareExchange64(variable, desired, expected);
#else
  return __sync_val_compare_and_swap(variable, expected, desired);
#endif
}

// Perform atomically:
// @code
// *variable@new = *variable@old + value
// return *variable@old
// @endcode
// Addition is performed in two's complement arithmetic, there is no undefined behavior
static inline int64_t
__atomic_i64_fetch_add
  (
    __atomic_i64* variable,
    __atomic_i64 value
  )
{
#if defined(_WIN32)
  return InterlockedExchangeAdd64(variable, value);
#else
  return __sync_fetch_and_add(variable, value);
#endif
}

static inline int64_t
__atomic_i64_fetch_sub
  (
    __atomic_i64* variable,
    __atomic_i64 value
  )
{
#if defined(_WIN32)
  /* Simulate subtraction via negation and addition. */
  return InterlockedExchangeAdd64(variable, -value);
#else
  return __sync_fetch_and_sub(variable, value);
#endif
}

// Perform atomically:
// @code
// *variable@new = *variable@old + 1
// return *variable@old
// @endcode
// Addition is performed in two's complement arithmetic, there is no undefined behavior
static inline int64_t
__atomic_64_fetch_inc
  (
    __atomic_i64* variable
  )
{
#if defined(_WIN32)
  #if 1 /* Use InterlockedIncrement64 instead of emulating using __atomic_i64_fetch_add. */
    return InterlockedIncrement64(variable);
  #else
    return __atomic_i64_fetch_add(variable, INT64_C(+1));
  #endif
#else
  return __sync_fetch_and_add(variable, INT64_C(+1));
#endif
}

// Perform atomically:
// @code
// *variable@new = *variable@old - 1
// return *variable@old
// @endcode
// Addition is performed in two's complement arithmetic, there is no undefined behavior
static inline int64_t
__atomic_64_fetch_dec
  (
    __atomic_i64* variable
  )
{
#if defined(_WIN32)
  #if 1 /* Use InterlockedDecrement64 instead of emulating using __atomic_i64_fetch_sub. */
    return InterlockedDecrement64(variable);
  #else
    return __atomic_i64_fetch_sub(variable, INT64_C(+1));
  #endif
#else
  return __sync_fetch_and_sub(variable, INT64_C(+1));
#endif
}

typedef int32_t maid_atomic_i32;

// @warning non-atomic dummy implementation.
static inline uint32_t atomic_load_u32(uint32_t* v) {
  return *v;
}

// compare *target to expected. If equal, replace *target by desired and return true.
// The function may determine *target != expected spuriously.
// @warnig non-atomic dummy implementation.
static inline bool compare_exchange_weak_u32(uint32_t* target, uint32_t expected, uint32_t desired) {
  if (*target == expected) {
    *target = desired;
    return true;
  } else {
    return false;
  }
}

int maid_reference_counter_increment(maid_reference_counter* self) {
  if (!self) {
    return 1;
  }
  uint32_t expected;
  do {
    expected = atomic_load_u32(self);
    if (expected == UINT32_MAX) return 1;
  } while(compare_exchange_weak_u32(self, expected, expected + 1));
  return 0;
}

int maid_reference_counter_decrement(maid_reference_counter* self, bool* one_to_zero) {
  if (!self) {
    return 1;
  }
  bool one_to_zero_now;
  uint32_t expected;
  do {
    expected = atomic_load_u32(self);
    if (expected == UINT32_MIN) return 1;
    one_to_zero_now = expected == UINT32_C(1);
  } while (compare_exchange_weak_u32(self, expected, expected - 1));
  if (one_to_zero) *one_to_zero = one_to_zero_now;
  return 0;
}
