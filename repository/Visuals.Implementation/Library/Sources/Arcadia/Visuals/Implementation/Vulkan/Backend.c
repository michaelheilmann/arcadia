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

#define ARCADIA_VISUALS_IMPLEMENTATION_PRIVATE (1)
#include "Arcadia/Visuals/Implementation/Vulkan/Backend.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

static Arcadia_String*
Arcadia_Visuals_Implementation_Vulkan_Backend_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  );

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  );

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  );

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Visuals_Implementation_Vulkan_Backend_objectTypeOperations = {
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Visuals_Implementation_Vulkan_Backend_construct,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Visuals_Implementation_Vulkan_Backend_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Visuals_Implementation_Vulkan_Backend_visit,
};

static const Arcadia_Type_Operations _Arcadia_Visuals_Implementation_Vulkan_Backend_typeOperations = {
  .objectTypeOperations = &_Arcadia_Visuals_Implementation_Vulkan_Backend_objectTypeOperations,
  .add = NULL,
  .and = NULL,
  .concatenate = NULL,
  .divide = NULL,
  .equalTo = NULL,
  .greaterThan = NULL,
  .greaterThanOrEqualTo = NULL,
  .hash = NULL,
  .lowerThan = NULL,
  .lowerThanOrEqualTo = NULL,
  .multiply = NULL,
  .negate = NULL,
  .not = NULL,
  .notEqualTo = NULL,
  .or = NULL,
  .subtract = NULL,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.Vulkan.Backend", Arcadia_Visuals_Implementation_Vulkan_Backend,
                         u8"Arcadia.Visuals.Backend", Arcadia_Visuals_Backend,
                         &_Arcadia_Visuals_Implementation_Vulkan_Backend_typeOperations);

static Arcadia_String*
Arcadia_Visuals_Implementation_Vulkan_Backend_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  )
{ return Arcadia_String_createFromCxxString(thread, u8"Arcadia Visuals Vulkan Backend"); }

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Visuals_Implementation_Vulkan_Backend);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  ((Arcadia_Engine_Backend*)self)->getName = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Engine_Backend*))& Arcadia_Visuals_Implementation_Vulkan_Backend_getNameImpl;

  Arcadia_LeaveConstructor(Arcadia_Visuals_Implementation_Vulkan_Backend);
}

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Visuals_Implementation_Vulkan_Backend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Implementation_Vulkan_Backend* self
  )
{/*Intentionally empty.*/}

Arcadia_Visuals_Implementation_Vulkan_Backend*
Arcadia_Visuals_Implementation_Vulkan_Backend_create
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  ARCADIA_CREATEOBJECT(Arcadia_Visuals_Implementation_Vulkan_Backend);
}
