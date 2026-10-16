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

#ifndef ARCADIA_ENGINE_UI_CONTAINER_H_INCLUDED
#define ARCADIA_ENGINE_UI_CONTAINER_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include `Arcadia/Engine/UI/Container.h` directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Object Arcadia_Object;
typedef struct Arcadia_Object Arcadia_Object;
typedef struct Arcadia_Object Arcadia_Object;

Arcadia_declareInterfaceType(u8"Arcadia.Engine.UI.Container", Arcadia_Engine_UI_Container)

struct Arcadia_Engine_UI_ContainerDispatch {
  Arcadia_InterfaceDispatch parent;
  Arcadia_Natural64Value (*getChildCount)(Arcadia_Thread* thread, Arcadia_Object* self);
  Arcadia_Object* (*getChildAt)(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Natural64Value index);
  void (*addChild)(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Object* child);
  void (*removeChild)(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Object* child);
};

Arcadia_Natural64Value
Arcadia_Engine_UI_Container_getChildCount
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

Arcadia_Object*
Arcadia_Engine_UI_Container_getChildAt
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  );

void
Arcadia_Engine_UI_Container_addChild
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

void
Arcadia_Engine_UI_Container_removeChild
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

#endif // ARCADIA_ENGINE_UI_CONTAINER_H_INCLUDED
