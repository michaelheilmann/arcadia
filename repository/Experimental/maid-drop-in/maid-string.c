#include "maid-string.h"

#include <limits.h> // INT_MAX
#include <stdio.h> // vsnprintf
#include <malloc.h> // malloc, realloc, free

int STRING_create(STRING** result) {
  if (!result) {
    return 1;
  }
  STRING* this = malloc(sizeof(STRING));
  if (!this) {
    return 1;
  }
  this->p = malloc(1);
  if (!this->p) {
    return 1;
  }
  this->p[0] = '\0';
  this->n = 0;
  *result = this;
  return 0;
}

int STRING_destroy(STRING* this) {
  if (!this) {
    return 1;
  }
  free(this->p);
  this->p = NULL;
  free(this);
  return 0;
}

int STRING_printfv(STRING* this, const char* format, va_list args) {
  int required = vsnprintf(NULL, 0, format, args);
  if (required < 0 || required == INT_MAX) {
    return 1;
  }
  if (required > this->n) {
    char* p = realloc(this->p, required + 1);
    if (!p) {
      return 1;
    }
    this->p = p;
    this->n = required;
  }
  vsnprintf(this->p, required + 1, format, args);
  return 0;
}

int STRING_printf(STRING* this, const char* format, ...) {
  va_list args;
  va_start(args, format);
  if (STRING_printfv(this, format, args)) {
    va_end(args);
    return 1;
  }
  va_end(args);
  return 0;
}
