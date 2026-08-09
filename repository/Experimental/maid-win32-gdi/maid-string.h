#if !defined(MAID_STRING_H_INCLUDED)
#define MAID_STRING_H_INCLUDED

#include <stddef.h> /* https://en.cppreference.com/w/c/header/stddef */
#include <stdarg.h> /* https://en.cppreference.com/w/c/header/stdarg.html */

typedef struct STRING {
  char* p; /* A pointer to an array of n plus 1 characters. */
  size_t n; /* The pre-allocated number of characters minus 1. */
} STRING;

/**
 * @summary Create an empty string.
 * @param [out] result A pointer to a @code{STRING*} variable.
 * @return @a 0 on success. @a 1 on failure.
 * @success @code{*result} was assigned a pointer to the @code{STRING} object.
 */
int STRING_create(STRING** result);

/**
 * @summary Destroy a string.
 * @param this A pointer to the STRING object.
 */
int STRING_destroy(STRING* this);

/**
 * @brief Print a formatted string to this STRING object.
 * @param this A pointer to this STRING object.
 * @param format, arguments The format string and its variadic argument list.
 * @return @a 0 on success. @a 1 on failure.
 */
int STRING_printfv(STRING* this, const char* format, va_list args);

/**
 * @brief Print a formatted string to this STRING object.
 * @param this A pointer to this STRING object.
 * @param format, ... The format string and its variadic argument list.
 * @return @a 0 on success. @a 1 on failure.
 */
int STRING_printf(STRING* this, const char* format, ...);

#endif // MAID_STRING_H_INCLUDED
