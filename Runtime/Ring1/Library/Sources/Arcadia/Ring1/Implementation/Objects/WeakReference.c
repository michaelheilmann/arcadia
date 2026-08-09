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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Objects/WeakReference.h"

#include "Arcadia/ARMS/Include.h"
#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"
#include "Arcadia/Ring1/Implementation/Object.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

static void
Arcadia_WeakReference_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  );

static void
Arcadia_WeakReference_visit
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  );

static void
Arcadia_WeakReference_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  );

static void
Arcadia_WeakReference_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReferenceDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_WeakReference_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_WeakReference_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_WeakReference_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_WeakReference_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.WeakReference", Arcadia_WeakReference,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
callback
  (
    Arcadia_WeakReference* argument1,
    void* argument2
  )
{
  Arcadia_Value_setVoidValue(&argument1->value, Arcadia_VoidValue_Void);
}

static void
Arcadia_WeakReference_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  )
{
  switch (Arcadia_Value_getTag(&self->value)) {
    case Arcadia_ValueTag_Atom: {
      Arcadia_ARMS_removeNotifyDestroy(self->value.atomValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback);
    } break;
    case Arcadia_ValueTag_BigInteger: {
    } break;
    case Arcadia_ValueTag_Boolean:
    case Arcadia_ValueTag_ForeignProcedure: {
    } break;
    case Arcadia_ValueTag_RuntimeByteArray: {
      Arcadia_ARMS_removeNotifyDestroy(self->value.runtimeByteArrayValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback);
    } break;
    case Arcadia_ValueTag_RuntimeUTF8String: {
      Arcadia_ARMS_removeNotifyDestroy(self->value.runtimeUTF8StringValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback);
    } break;
    case Arcadia_ValueTag_Integer16:
    case Arcadia_ValueTag_Integer32:
    case Arcadia_ValueTag_Integer64:
    case Arcadia_ValueTag_Integer8:
    case Arcadia_ValueTag_Natural16:
    case Arcadia_ValueTag_Natural32:
    case Arcadia_ValueTag_Natural64:
    case Arcadia_ValueTag_Natural8: {
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_Object_removeNotifyDestroyCallback(thread, self->value.objectReferenceValue, self, (void (*)(void*, Arcadia_Object*))&callback);
    } break;
    case Arcadia_ValueTag_Real32:
    case Arcadia_ValueTag_Real64:
    case Arcadia_ValueTag_Size:
    case Arcadia_ValueTag_Type:
    case Arcadia_ValueTag_Void: {
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
Arcadia_WeakReference_visit
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  )
{ }

static void
Arcadia_WeakReference_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  )
{
  Arcadia_EnterConstructor(Arcadia_WeakReference);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 == _numberOfArguments) {
    self->value = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  } else if (1 == _numberOfArguments) {
    self->value = Arcadia_ValueStack_getValue(thread, 1);
    switch (Arcadia_Value_getTag(&self->value)) {
      case Arcadia_ValueTag_Atom: {
        if (Arcadia_ARMS_addNotifyDestroy(self->value.atomValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
      } break;
      case Arcadia_ValueTag_BigInteger: {
      } break;
      case Arcadia_ValueTag_Boolean:
      case Arcadia_ValueTag_ForeignProcedure: {
      } break;
      case Arcadia_ValueTag_RuntimeByteArray: {
        if (Arcadia_ARMS_addNotifyDestroy(self->value.runtimeByteArrayValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
      } break;
      case Arcadia_ValueTag_RuntimeUTF8String: {
        if (Arcadia_ARMS_addNotifyDestroy(self->value.runtimeUTF8StringValue, self, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&callback)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
      } break;
      case Arcadia_ValueTag_Integer16:
      case Arcadia_ValueTag_Integer32:
      case Arcadia_ValueTag_Integer64:
      case Arcadia_ValueTag_Integer8:
      case Arcadia_ValueTag_Natural16:
      case Arcadia_ValueTag_Natural32:
      case Arcadia_ValueTag_Natural64:
      case Arcadia_ValueTag_Natural8: {
      } break;
      case Arcadia_ValueTag_ObjectReference: {
        Arcadia_Object_addNotifyDestroyCallback(thread, self->value.objectReferenceValue, self, (void (*)(void*, Arcadia_Object*)) &callback);
      } break;
      case Arcadia_ValueTag_Real32:
      case Arcadia_ValueTag_Real64:
      case Arcadia_ValueTag_Size:
      case Arcadia_ValueTag_Type:
      case Arcadia_ValueTag_Void: {
      } break;
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_WeakReference);
}

static void
Arcadia_WeakReference_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReferenceDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_WeakReference*
Arcadia_WeakReference_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  _Arcadia_BeginCreate(Arcadia_WeakReference);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_WeakReference);
}

Arcadia_Value
Arcadia_WeakReference_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_WeakReference* self
  )
{ return self->value; }
