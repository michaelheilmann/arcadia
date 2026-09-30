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

#include "Arcadia/Engine/Examples/TextRendering/TextRun.h"

static void
Arcadia_Engine_Examples_TextRendering_TextRun_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  );

static void
Arcadia_Engine_Examples_TextRendering_TextRun_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  );

static void
Arcadia_Engine_Examples_TextRendering_TextRun_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRunDispatch* self
  );

static void
Arcadia_Engine_Examples_TextRendering_TextRun_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Examples_TextRendering_TextRun_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Examples_TextRendering_TextRun_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Examples_TextRendering_TextRun_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Examples_TextRendering_TextRun_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Examples_TextRendering_TextRun_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Examples_TextRendering_TextRun_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Examples_TextRendering_TextRun_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Examples.TextRendering.TextRun", Arcadia_Engine_Examples_TextRendering_TextRun,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Engine_Examples_TextRendering_TextRun_typeOperations);

static void
Arcadia_Engine_Examples_TextRendering_TextRun_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Examples_TextRendering_TextRun);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->modelNode = NULL;
  self->fontAtlasWidth = 0;
  self->fontAtlasHeight = 0;
  self->fixedPlacement = Arcadia_BooleanValue_False;
  self->alignment = Arcadia_Engine_Text_TextAlignment_Center;
  self->hasRectangle = Arcadia_BooleanValue_False;
  self->rectangleLeft = 0;
  self->rectangleBottom = 0;
  self->rectangleWidth = 0;
  self->rectangleHeight = 0;
  self->allCodePoints = NULL;
  self->allGlyphInformation = NULL;
  self->allCodePointCount = 0;
  self->glyphs = NULL;
  self->maximumNumberOfGlyphs = 0;
  self->numberOfGlyphs = 0;
  self->red = 0.f;
  self->green = 0.f;
  self->blue = 0.f;
  self->alpha = 0.f;
  Arcadia_LeaveConstructor(Arcadia_Engine_Examples_TextRendering_TextRun);
}

static void
Arcadia_Engine_Examples_TextRendering_TextRun_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  )
{
  if (self->allCodePoints) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->allCodePoints);
    self->allCodePoints = NULL;
  }
  if (self->allGlyphInformation) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->allGlyphInformation);
    self->allGlyphInformation = NULL;
  }
  if (self->glyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->glyphs);
    self->glyphs = NULL;
  }
  self->allCodePointCount = 0;
  self->maximumNumberOfGlyphs = 0;
  self->numberOfGlyphs = 0;
}

static void
Arcadia_Engine_Examples_TextRendering_TextRun_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRunDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Examples_TextRendering_TextRun_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun* self
  )
{
  if (self->modelNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNode);
  }
}

Arcadia_Engine_Examples_TextRendering_TextRun*
Arcadia_Engine_Examples_TextRendering_TextRun_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Examples_TextRendering_TextRun);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Examples_TextRendering_TextRun);
}
