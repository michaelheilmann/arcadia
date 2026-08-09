/**
 * @file maid-buffer-8.h
 * @brief Source file of the maid_buffer_u8 type.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#include "maid-buffer-u8.h"

#include <malloc.h>
#include <string.h>

int maid_buffer_u8_initialize(maid_buffer_u8* self) {
  if (!self) {
    return 1;
  }
  self->p = malloc(8);
  if (!self->p) {
    return 1;
  }
  self->cp = 8;
  self->sz = 0;
  return 0;
}

int maid_buffer_u8_uninitialize(maid_buffer_u8* self) {
  if (!self) {
    return 1;
  }
  self->sz = 0;
  free(self->p);
  self->p = NULL;
  self->cp = 0;
  return 0;
}

int maid_buffer_u8_ensure_capacity(maid_buffer_u8* self, size_t required) {
  if (!self) {
    return 1;
  }
  if (self->cp < required) {
    void* new_p = realloc(self->p, required);
    if (!new_p) {
      return 1;
    }
    self->p = new_p;
    self->cp = required;
  }
  return 0;
}

int maid_buffer_u8_insert_from_cxx_bytes(maid_buffer_u8* self, size_t i, const uint8_t* p, size_t n) {
  if (!self || !p) {
    return 1;
  }
  if (i > self->sz) {
    return 1;
  }
  size_t old_cp = self->cp;
  size_t available_cp = old_cp - self->sz;
  size_t required_cp = n;
  if (available_cp < required_cp) {
    size_t additional_cp = required_cp - available_cp;
    if (SIZE_MAX - old_cp < additional_cp) {
      return 1;
    }
    size_t new_cp = old_cp + additional_cp;
    void* new_p = realloc(self->p, new_cp);
    if (!new_p) {
      return 1;
    }
    self->cp = new_cp;
    self->p = new_p;
  }
  if (i < self->sz) {
    memmove(((char*)self->p) + i + n, ((char*)self->p) + i, self->sz - i);
  }
  memcpy(((char*)self->p) + i, p, n);
  self->sz += n;
  return 0;
}

int maid_buffer_u8_append_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n) {
  if (!self || !p) {
    return 1;
  }
  return maid_buffer_u8_insert_from_cxx_bytes(self, self->sz, p, n);
}

int maid_buffer_u8_prepend_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n) {
  if (!self || !p) {
    return 1;
  }
  return maid_buffer_u8_insert_from_cxx_bytes(self, 0, p, n);
}

int maid_buffer_u8_set_from_cxx_bytes(maid_buffer_u8* self, const uint8_t* p, size_t n) {
  if (maid_buffer_u8_ensure_capacity(self, n)) {
    return 1;
  }
  memcpy(self->p, p, n);
  self->sz += n;
  return 0;
}

int maid_buffer_u8_clear(maid_buffer_u8* self) {
  if (!self) {
    return 1;
  }
  self->sz = 0;
  return 0;
}

int maid_buffer_u8_assign(maid_buffer_u8* self, maid_buffer_u8* other) {
  if (!self || !other) {
    return 1;
  }
  if (self == other) {
    return 0;
  }
  if (self->cp < other->sz) {
    size_t new_cp = other->sz;
    void* new_p = realloc(self->p, new_cp);
    if (!new_p) {
      return 1;
    }
    self->cp = new_cp;
    self->p = new_p;
  }
  memcpy(self->p, other->p, other->sz);
  self->sz = other->sz;
  return 0;
}
