// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/FontCache.h"

typedef struct FontCacheEntry {
  char const* fontPath;
  Arcadia_Natural32Value pixelSize;
  Arcadia_FontIO_AtlasBitmapFont* font;
} FontCacheEntry;

struct Arcadia_Engine_FontCache {
  Arcadia_Object parent;
  FontCacheEntry* entries;
  Arcadia_SizeValue count;
  Arcadia_SizeValue capacity;
};

static void
Arcadia_Engine_FontCache_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  );

static void
Arcadia_Engine_FontCache_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  );

static void
Arcadia_Engine_FontCache_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCacheDispatch* self
  );

static void
Arcadia_Engine_FontCache_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_FontCache_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_FontCache_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_FontCache_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_FontCache_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_FontCache_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_FontCache_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_FontCache_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.FontCache", Arcadia_Engine_FontCache,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Engine_FontCache_typeOperations);

static void
Arcadia_Engine_FontCache_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_FontCache);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->entries = NULL;
  self->count = 0;
  self->capacity = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_FontCache);
}

static void
Arcadia_Engine_FontCache_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  )
{
  if (self->entries) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->entries);
    self->entries = NULL;
  }
  self->count = 0;
  self->capacity = 0;
}

static void
Arcadia_Engine_FontCache_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCacheDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_FontCache_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self
  )
{
  for (Arcadia_SizeValue i = 0; i < self->count; ++i) {
    if (self->entries[i].font) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->entries[i].font);
    }
  }
}

Arcadia_Engine_FontCache*
Arcadia_Engine_FontCache_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_FontCache);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_FontCache);
}

Arcadia_FontIO_AtlasBitmapFont*
Arcadia_Engine_FontCache_getOrCreate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_FontCache* self,
    char const* fontPath,
    Arcadia_Natural32Value pixelSize
  )
{
  for (Arcadia_SizeValue i = 0; i < self->count; ++i) {
    if (self->entries[i].pixelSize == pixelSize && self->entries[i].fontPath == fontPath) {
      return self->entries[i].font;
    }
  }
  if (self->count == self->capacity) {
    Arcadia_SizeValue const oldCapacity = self->capacity;
    Arcadia_SizeValue const newCapacity = oldCapacity ? oldCapacity * 2 : 4;
    FontCacheEntry* newEntries = Arcadia_Memory_allocateUnmanaged(thread, newCapacity * sizeof(FontCacheEntry));
    for (Arcadia_SizeValue i = 0; i < self->count; ++i) {
      newEntries[i] = self->entries[i];
    }
    if (self->entries) {
      Arcadia_Memory_deallocateUnmanaged(thread, self->entries);
    }
    self->entries = newEntries;
    self->capacity = newCapacity;
  }
  Arcadia_FontIO_AtlasBitmapFont* font = Arcadia_FontIO_AtlasBitmapFont_create(thread, Arcadia_String_createFromCxxString(thread, fontPath), pixelSize);
  self->entries[self->count].fontPath = fontPath;
  self->entries[self->count].pixelSize = pixelSize;
  self->entries[self->count].font = font;
  ++self->count;
  return font;
}
