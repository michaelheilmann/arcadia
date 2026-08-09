/**
 * @file maid-atexit.h
 * @brief Header file of the atexit functionality.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#if !defined(MAID_ATEXIT_H_INCLUDED)
#define MAID_ATEXIT_H_INCLUDED

#include <stdint.h>

/**
 * @brief An unsigned integer value, the ID of an atexit handler.
 */
typedef uintptr_t maid_atexit_handler_id;

/**
 * @brief Register an atexit handler.
 * @param context The context.
 * @param callback The callback.
 * @return The zero value on success. A non-zero value on failure.
 * @post Success: *id was assigned the ID of the handler.
 * @remarks Handlers cannot be registered while the callbacks are running.
 * @remarks Handlers are executed in the inverse order of their addition.
 */
int maid_atexit_register(maid_atexit_handler_id *id, void* context, void (*callback)(void* context));

/**
 * @brief Unregister an atexit handler.
 * @param id The ID of the handler.
 * @return The zero value on success. A non-zero value on failure.
 * @remarks Handlers cannot be unregistered while the callbacks are running.
 */
int maid_atexit_unregister(maid_atexit_handler_id id);

/**
 * @brief The opaque type of a string.
 */
typedef struct maid_string maid_string;

/**
 * @brief Create an UTF-8 string.
 * @param [out] result A pointer to a maid_string* variable.
 * @param p A pointer to an array of @a n Bytes.
 * @param n The number of Bytes in the array pointed to by @a p.
 * @return The zero value on success. A non-zero value on failure.
 * @post Success: *result was assigned a pointer to the maid_string object. The caller acquired a reference to that object.
 */
int maid_string_create(maid_string** result, const uint8_t* p, size_t n);

/**
 * @brief Increment the reference count of a string.
 * @param self A pointer to this string.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_string_acquire(maid_string* self);

/**
 * @brief Decrement the reference count of a string.
 * @param self A pointer to this string.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_string_relinquish(maid_string* self);

#endif // MAID_ATEXIT_H_INCLUDED
