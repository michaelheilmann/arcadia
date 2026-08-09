/**
 * @file maid_buffer_8.h
 * @brief Header file of the maid_buffer_u8 type.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#if !defined(MAID_BUFFER_U8_H_INCLUDED)
#define MAID_BUFFER_U8_H_INCLUDED

#include <inttypes.h>

/**
 * Type of a buffer for uint8_t values to be embedded into other objects.
 *
 * Example of its canonical usage:
 * @code
 * typedef struct maid_foo {
 *  maid_buffer_u8 buffer;
 * } maid_foo;
 *
 * int maid_foo_initialize(maid_foo* self) {
 *   if (maid_buffer_u8_initialize(&self->buffer)) {
 *     return 1;
 *   }
 *   return 0;
 * }
 *
 * int maid_foo_uninitialize(maid_foo* self) {
 *   maid_buffer_u8_uninitialize(&self->buffer);
 * }
 * @encode
 */
typedef struct maid_buffer_u8 maid_buffer_u8;

struct maid_buffer_u8 {
  char* p;
  size_t sz, cp;
};

/*
 * @brief Initialize this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_initialize(maid_buffer_u8* self);

/*
 * @brief Uninitialize this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_uninitialize(maid_buffer_u8* self);

/**
 * @brief Ensure the capacity of this maid_buffer_u8 object is greater than or equal to a required capacity.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param required The required capacity.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_ensure_capacity(maid_buffer_u8* self, size_t required);

/**
 * @brief Insert into the contents of this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param i The index at which to insert the Bytes. Must be greater than or equal to 0 and smaller than or equal to the size of this maid_buffer_u8 object.
 * @param p A pointer to an array of @a n uint8_t values.
 * @param n The number of values in the array pointed to by @a n.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_insert_from_cxx_bytes(maid_buffer_u8* self, size_t i, const uint8_t* p, size_t n);

/**
 * @brief Append to the contents of this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param p A pointer to an array of @a n uint8_t values.
 * @param n The number of values in the array pointed to by @a n.
 * @return The zero value on success. A non-zero value on failure.
 * Preconditions:
 * - self must point to a buffer
 * - p must point to an array of n Bytes
 */
int maid_buffer_u8_append_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n);

/**
 * @brief Prepend to the contents of this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param p A pointer to an array of @a n uint8_t values.
 * @param n The number of values in the array pointed to by @a n.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_prepend_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n);

/**
 * @brief Set the contents of this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param p A pointer to an array of @a n uint8_t values.
 * @param n The number of values in the array pointed to by @a n.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_set_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n);

/**
 * @brief Clear the contents of this maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_clear(maid_buffer_u8* self);

/**
 * @brief Set the contents of this maid_buffer_u8 object from the contents of another maid_buffer_u8 object.
 * @param self A pointer to this maid_buffer_u8 object.
 * @param other A pointer to the other maid_buffer_u8 object.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_buffer_u8_assign(maid_buffer_u8* self, maid_buffer_u8 *other);

#endif // MAID_BUFFER_U8_H_INCLUDED
