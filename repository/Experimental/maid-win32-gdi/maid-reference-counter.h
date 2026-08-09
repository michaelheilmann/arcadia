/**
 * @file maid-reference-counter.h
 * @brief Header file of the maid_reference_counter type.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#if !defined(MAID_REFERENCE_COUNTER_H_INCLUDED)
#define MAID_REFERENCE_COUNTER_H_INCLUDED

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>

/**
 * @brief The type of a reference counter. An unsigned integer.
 * @code
 * struct my_object {
 *   maid_reference_counter reference_counter;
 * };
 *
 * int my_object_ref(struct my_object* self) {
 *   if (maid_reference_counter_increment(&self->reference-counter)) {
 *     return 1;
 *   }
 *   return 0;
 * }
 *
 * int my_object_unref(struct my_object* self) {
 *   if (maid_reference_counter_decrement(&one_to_zero, &self->reference-counter)) {
 *     return 1;
 *   }
 *   if (one_to_zero) {
 *     // kill the object
 *   }
 *   return 0;
 * }
 *
 * @endcode
 */
typedef uint32_t maid_reference_counter;

/**
 * @brief The minimum value of an uint32_t value.
 */
#define UINT32_MIN (UINT32_C(0))

/**
 * @brief The minimum value of a reference counter.
 */
#define MAID_REFERENCE_COUNTER_MINIMUM (UINT32_C(0))

/**
 * @brief The maximum value of a reference counter.
 */
#define MAID_REFERENCE_COUNTER_MAXIMUM (UINT32_MAX)

/**
 * @brief Increment the reference counter.
 * @param self A pointer to the reference counter.
 * @return The zero value on success. A non-zero value on failure.
 * This function fails if and only if one of two conditions are fulfilled.
 * - self is a zero pointer
 * - *self is MAID_REFERENCE_COUNTER_MAXIMUM
 */
int maid_reference_counter_increment(maid_reference_counter* self);

/**
 * @brief Decrement the reference counter.
 * @param self A pointer to the reference counter.
 * @param one_to_zero A pointer to a bool variable or the null pointer.
 * The variable is assigned on success if the counter was decremented from one to zero.
 * @return The zero value on success. A non-zero value on failure.
 * This function fails if and only if one of two conditions are fulfilled.
 * - self is a zero pointer
 * - *self is MAID_REFERENCE_COUNTER_MINIMUM
 */
int maid_reference_counter_decrement(maid_reference_counter* self, bool* one_to_zero);

#endif // MAID_REFERENCE_COUNTER_H_INCLUDED
