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

#include "Arcadia/Engine/Demo/Audials.h"

#include "Arcadia/Engine/Demo/Configuration.h"
#include "Arcadia/Audials/Implementation/Include.h"

static void
setMasterVolume
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    Arcadia_Integer32Value value
  )
{
  char const* path[] = {
    u8"audials",
    u8"masterVolume",
  };
  Cfg2_setInteger32(thread, configuration, path, 2, value);
}

static Arcadia_Integer32Value
getMasterVolume
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration
  )
{
  char const* path[] = {
    u8"audials",
    u8"masterVolume",
  };
  return Cfg2_getInteger32(thread, configuration, path, 2);
}

static Arcadia_BooleanValue
findTypeByName
  (
    Arcadia_Thread* thread,
    Arcadia_Value x,
    Arcadia_Value y
  )
{
  Arcadia_Type* yy = Arcadia_Value_getTypeValue(&y);
  Arcadia_Name* b = Arcadia_Type_getName(thread, yy);
  Arcadia_String* a = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&x);
  return Arcadia_String_getNumberOfBytes(thread, a) == Arcadia_Name_getNumberOfBytes(thread, b)
      && !Arcadia_Memory_compare(thread, Arcadia_String_getBytes(thread, a), Arcadia_Name_getBytes(thread, b), Arcadia_String_getNumberOfBytes(thread, a));
}

void
Arcadia_Engine_Application_startupAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_DDL_Node* configuration
  )
{
  // (1.1) Register audials backends.
  Arcadia_Engine_Audials_Implementation_registerBackends(thread, engine->audialsBackendTypes);
  // (1.2) Register the audials node factory.
  Arcadia_Engine_Audials_registerNodeFactories(thread, engine->audialsNodeFactoryTypes);
  // (2) Select audials system.
  {
    char const* path[] = {
      u8"audials",
      u8"backend",
    };
    Arcadia_Engine_Backend* backend = NULL;
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_String* backendTypeName = Cfg2_getString(thread, (Arcadia_DDL_Node*)configuration, path, 2);
      Arcadia_Value backendTypeValue = Arcadia_HashSet_findFirst(thread, (Arcadia_HashSet*)engine->audialsBackendTypes, Arcadia_Value_makeObjectReferenceValue(backendTypeName), &findTypeByName);
      if (!Arcadia_Value_isTypeValue(&backendTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* backendType = Arcadia_Value_getTypeValue(&backendTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, backendType, _Arcadia_Engine_Audials_Backend_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      backend = (Arcadia_Engine_Backend*)_Arcadia_EndCreate0(thread, backendType, Arcadia_ValueStack_getSize(thread) - 1);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_String* backendTypeName = Arcadia_String_createFromCxxString(thread, u8"Arcadia.Engine.Audials.Implementation.OpenAL.Backend");
      Cfg2_setString(thread, (Arcadia_DDL_Node*)configuration, path, 2, backendTypeName);
      Arcadia_Value backendTypeValue = Arcadia_HashSet_findFirst(thread, (Arcadia_HashSet*)engine->audialsBackendTypes, Arcadia_Value_makeObjectReferenceValue(backendTypeName), &findTypeByName);
      if (!Arcadia_Value_isTypeValue(&backendTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* backendType = Arcadia_Value_getTypeValue(&backendTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, backendType, _Arcadia_Engine_Audials_Backend_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      backend = (Arcadia_Engine_Backend*)_Arcadia_EndCreate0(thread, backendType, Arcadia_ValueStack_getSize(thread) - 1);
    }
    Arcadia_Engine_Audials_BackendContext* temporary = (Arcadia_Engine_Audials_BackendContext*)Arcadia_Engine_Backend_createBackendContext(thread, backend);
    Arcadia_Object_lock(thread, (Arcadia_Object*)temporary);
    engine->audialsBackendContext = (Arcadia_Engine_BackendContext*)temporary;
  }
  // (3) Set the master volume.
  {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_Integer32Value masterVolume = getMasterVolume(thread, configuration);
      if (masterVolume < 0 || masterVolume > 10) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      setMasterVolume(thread, configuration, 10);
    }
  }
  // (4) Create the node factory.
  {
    Arcadia_Engine_NodeFactory* nodeFactory = NULL;
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_List* temporary = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      Arcadia_Set_getAll(thread, engine->audialsNodeFactoryTypes, temporary);
      Arcadia_Value scneNodeFactoryTypeValue = Arcadia_List_getAt(thread, temporary, 0);
      if (!Arcadia_Value_isTypeValue(&scneNodeFactoryTypeValue)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Type* scneNodeFactoryType = Arcadia_Value_getTypeValue(&scneNodeFactoryTypeValue);
      if (!Arcadia_Type_isDescendantType(thread, scneNodeFactoryType, _Arcadia_Engine_Audials_NodeFactory_getType(thread))) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      nodeFactory = (Arcadia_Engine_NodeFactory*)_Arcadia_EndCreate0(thread, scneNodeFactoryType, Arcadia_ValueStack_getSize(thread) - 1);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_jump(thread);
    }
    engine->audialsNodeFactory = nodeFactory;
  }
}
