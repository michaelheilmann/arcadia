/**
 * @file c_bigint.h
 * @brief Header file for the C bigint library.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 * @copyright Copyright (c) 2026 Michael Heilmann. All rights reserved.
 */
#if !defined(C_BIGINT_H_INCLUDED)
#define C_BIGINT_H_INCLUDED

/**@{*/

/**
 * @briefCreate a c_bigint object.
 * @param [out] result A pointer to a c_bigint* variable.
 * @post *result was assigned a pointer to the created c_bigint object on success.
 * The value of that c_bigint object is @a 0.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_create(c_bigint** result);

/**
 * @brief Destroy a c_bigint object.
 * @param self A pointer to a c_bigint object.
 * @post The c_bigint object pointed to by @a self was destroyed on success.
 * @return The zero value on success. An non-zero value on failure.
 */
int c_bigint_destroy(c_bigint* self);

/**@}*/

/**@{*/

/**
 * @brief Set the value of a c_bigint object from an int16_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The int16_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_int16(c_bigint* self, int16_t value);

/**
 * @brief Set the value of a c_bigint object from an int32_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The int32_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_int32(c_bigint* self, int32_t value);

/**
 * @brief Set the value of a c_bigint object from an int64_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The int64_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_int64(c_bigint* self, int64_t value);

/**
 * @brief Set the value of a c_bigint object from an int8_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The int8_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_int8(c_bigint* self, int8_t value);

/**@}*/

/**(@{*/

/**
 * @brief Set the value of a c_bigint object from an uint16_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The uint16_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_uint16(c_bigint* self, uint16_t value);

/**
 * @brief Set the value of a c_bigint object from an uint32_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The uint32_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_uint32(c_bigint* self, uint32_t value);

/**
 * @brief Set the value of a c_bigint object from an uint64_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The uint64_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_uint64(c_bigint* self, uint64_t value);

/**
 * @brief Set the value of a c_bigint object from an uint8_t value.
 * @param self A pointer to the c_bigint object.
 * @param value The uint8_t value.
 * @post The c_bigint object pointed to by @a self was assigned the value specified by @a value.
 * @return The zero value on success. A non-zero value on failure.
 */
int c_bigint_set_from_uint8(c_bigint* self, uint8_t value);

/**@}*/

#endif /* C_BIGINT_H_INCLUDED */
