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
#include "Arcadia/Ring1/Implementation/Object.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Names.h"
#include <assert.h>

static void*
Arcadia_allocateObject
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  );

static void
Arcadia_Object_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
_Arcadia_Object_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectDispatch* self
  );

static void
isIdenticalTo
  (
    Arcadia_Thread* thread
  );

static void
isEqualTo
  (
    Arcadia_Thread* thread
  );

static void
getHash
  (
    Arcadia_Thread* thread
  );

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  );

#define Operation(_Name) \
  static void \
  _type_##_Name \
    ( \
      Arcadia_Thread* thread \
    );

#include "Arcadia/Ring1/Implementation/Object/TypeFunctions.i"

#undef Operation

static const Arcadia_ObjectType_Operations _Arcadia_Object_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = &Arcadia_Object_constructImpl,
  .initializeDispatch = &_Arcadia_Object_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Object_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Object_objectTypeOperations,

#define Operation(_Name) \
  ._Name = &_type_##_Name,

#include "Arcadia/Ring1/Implementation/Object/TypeFunctions.i"

#undef Operation
};

#define Operation(_Name) \
  static void \
  _type_##_Name \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    if (!Arcadia_ValueStack_getSize(thread)) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
      Arcadia_Thread_jump(thread); \
    } \
    Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0); \
    if (!numberOfArgumentValues || numberOfArgumentValues == Arcadia_Natural8Value_Maximum) {\
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
      Arcadia_Thread_jump(thread); \
    } \
    Arcadia_Object* object = Arcadia_ValueStack_getObjectReferenceValue(thread, numberOfArgumentValues + 1); \
    Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(Arcadia_Object_getType(thread, object)); \
    operations->_Name(thread); \
  }

#include "Arcadia/Ring1/Implementation/Object/TypeFunctions.i"

#undef Operation

#define BINARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 3) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 2); \
  Arcadia_Value y = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 3);

#define UNARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 2) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (1 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 2);

void
Arcadia_Object_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Object);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Object);
}

static void
isIdenticalTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isObjectReferenceValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getObjectReferenceValue(&x) == Arcadia_Value_getObjectReferenceValue(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
isEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isObjectReferenceValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getObjectReferenceValue(&x) == Arcadia_Value_getObjectReferenceValue(&y));
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
getHash
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)(uintptr_t)Arcadia_Value_getObjectReferenceValue(&x));
}

#define ObjectTypeName u8"Arcadia.Object"

typedef struct ObjectTag ObjectTag;

struct ObjectTag {
  Arcadia_TypeValue type;
};

static bool g_objectRegistered = false;

static void
_Arcadia_Object_onObjectTypeRemoved
  (
    Arcadia_Process* process,
    uint8_t const* name,
    size_t nameLength
  );

static void
_Arcadia_Object_onFinalizeObject
  (
    Arcadia_Process* process,
    void* object
  );

static void
_Arcadia_Object_onVisitObject
  (
    Arcadia_Process* process,
    void* object
  );

static void
_Arcadia_Object_onObjectTypeRemoved
  (
    Arcadia_Process* process,
    const uint8_t* name,
    size_t nameLength
  )
{
  g_objectRegistered = false;
}

static void
_Arcadia_Object_onFinalizeObject
  (
    Arcadia_Process* process,
    void* object
  )
{
  ObjectTag* objectTag = (ObjectTag*)object;
  Arcadia_TypeValue type = (Arcadia_TypeValue)objectTag->type;
  if (Arcadia_Process_unlockObject(process, type)) {
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
  }
  if (type == _Arcadia_Memory_getType(Arcadia_Process_getThread(process))) {
    return;
  }
  while (type) {
    Arcadia_Object_DestructCallbackFunction* destruct = Arcadia_Type_getDestructObjectCallbackFunction(type);
    if (destruct) {
      destruct(Arcadia_Process_getThread(process), (Arcadia_Object*)(((ObjectTag*)object) + 1));
    }

    type = Arcadia_ObjectType_getParentObjectType(Arcadia_Process_getThread(process), type);
    objectTag->type = type;
  }
}

static void
_Arcadia_Object_onVisitObject
  (
    Arcadia_Process* process,
    void* object
  )
{
  ObjectTag* objectTag = (ObjectTag*)object;
  Arcadia_TypeValue type = (Arcadia_TypeValue)objectTag->type;
  if (type == _Arcadia_Memory_getType(Arcadia_Process_getThread(process))) {
    return;
  }
  while (type) {
    Arcadia_Object_VisitCallbackFunction* visit = Arcadia_Type_getVisitObjectCallbackFunction(type);
    if (visit) {
      visit(Arcadia_Process_getThread(process), (Arcadia_Object*)(((ObjectTag*)object) + 1));
    }
    type = Arcadia_ObjectType_getParentObjectType(Arcadia_Process_getThread(process), type);
  }
}

static void*
Arcadia_allocateObject
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  if (!type) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Type_isObjectKind(thread, type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Type* memoryType = _Arcadia_Memory_getType(thread);

  ObjectTag* tag = NULL;
  if (SIZE_MAX - sizeof(ObjectTag) < Arcadia_ObjectType_getValueSize(thread, (Arcadia_ObjectType*)type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&tag, ObjectTypeName, sizeof(ObjectTypeName) - 1, sizeof(ObjectTag) + Arcadia_ObjectType_getValueSize(thread, (Arcadia_ObjectType*)type));
  tag->type = memoryType;
  if (Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), memoryType)) {
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
  }
  Arcadia_Type_getOperations(type)->objectTypeOperations->construct(thread, (Arcadia_ObjectReferenceValue)(Arcadia_Object*)(tag + 1));
  return (void*)(tag + 1);
}

void*
_Arcadia_EndCreate0
  (
    Arcadia_Thread* thread,
    Arcadia_Type* type,
    Arcadia_SizeValue oldValueStackSize
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    void* self = Arcadia_allocateObject(thread, type);
    if (!self || oldValueStackSize != Arcadia_ValueStack_getSize(thread)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Thread_popJumpTarget(thread);
    return self;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (oldValueStackSize < Arcadia_ValueStack_getSize(thread)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - oldValueStackSize);
    }
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_TypeValue g__Arcadia_Object_type = NULL;

static void
_Arcadia_Object_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectDispatch* self
  )
{
  self->isIdenticalTo = &isIdenticalTo;
  self->isEqualTo = &isEqualTo;
  self->getHash = &getHash;
}

static void
_Arcadia_Object_typeDestructing
  (
    void* context
  )
{ g__Arcadia_Object_type = NULL; }

Arcadia_TypeValue
_Arcadia_Object_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_objectRegistered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 ObjectTypeName, sizeof(ObjectTypeName) - 1,
                                 Arcadia_Thread_getProcess(thread),
                                 (Arcadia_Process_TypeRemovedCallback*)&_Arcadia_Object_onObjectTypeRemoved,
                                 (Arcadia_Process_VisitCallback*)&_Arcadia_Object_onVisitObject,
                                 (Arcadia_Process_FinalizeCallback*)&_Arcadia_Object_onFinalizeObject);
    g_objectRegistered = Arcadia_BooleanValue_True;
  }
  if (!g__Arcadia_Object_type) {
    g__Arcadia_Object_type = Arcadia_registerObjectType(thread,
                                                        Arcadia_Names_getOrCreateName(thread, ObjectTypeName, sizeof(ObjectTypeName) - 1),
                                                        sizeof(Arcadia_Object), NULL,
                                                        sizeof(Arcadia_ObjectDispatch),
                                                        &_Arcadia_Object_typeOperations, &_Arcadia_Object_typeDestructing);
  }
  return g__Arcadia_Object_type;
}

void
Arcadia_Object_setType
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_TypeValue type
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  ObjectTag* objectTag = ((ObjectTag*)self) - 1;
  if (type) {
    if (Arcadia_Process_lockObject(process, type)) {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
    }
  }
  if (objectTag->type) {
    if (Arcadia_Process_unlockObject(process, objectTag->type)) {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
    }
  }
  objectTag->type = type;
}

void
Arcadia_Object_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ObjectTag* tag = ((ObjectTag*)self) - 1;
  Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), tag->type);
  Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), tag);
}

void
Arcadia_Object_addNotifyDestroyCallback
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    void* observer,
    void (*callback)(void* observer, Arcadia_Object*)
  )
{
  ObjectTag* tag = ((ObjectTag*)self) - 1;
  Arcadia_ARMS_Status status = Arcadia_ARMS_addNotifyDestroy(tag, observer, self, (Arcadia_ARMS_NotifyDestroyCallback*)callback);
  if (status) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Object_removeNotifyDestroyCallback
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    void* observer,
    void (*callback)(void* observer, Arcadia_Object*)
  )
{
  ObjectTag* tag = ((ObjectTag*)self) - 1;
  Arcadia_ARMS_Status status = Arcadia_ARMS_removeNotifyDestroy(tag, observer, self, (Arcadia_ARMS_NotifyDestroyCallback*)callback);
  if (status) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

#if Arcadia_Configuration_withBarriers

void
Arcadia_Object_forwardBarrier
  (
    Arcadia_Thread* thread,
    Arcadia_Object* source,
    Arcadia_Object* target
  )
{
  if (source && target) {
    ObjectTag* sourceTag = ((ObjectTag*)source) - 1;
    ObjectTag* targetTag = ((ObjectTag*)target) - 1;
    Arcadia_ARMS_forwardBarrier(sourceTag, targetTag);
  }
}

void
Arcadia_Object_backwardBarrier
  (
    Arcadia_Thread* thread,
    Arcadia_Object* source,
    Arcadia_Object* target
  )
{
  if (source && target) {
    ObjectTag* sourceTag = ((ObjectTag*)source) - 1;
    ObjectTag* targetTag = ((ObjectTag*)target) - 1;
    Arcadia_ARMS_backwardBarrier(sourceTag, targetTag);
  }
}

#endif // Arcadia_Configuration_withBarriers

void
Arcadia_Object_lock
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ObjectTag* tag = ((ObjectTag*)self) - 1;
  Arcadia_Status status = Arcadia_Process_lockObject(Arcadia_Thread_getProcess(thread), tag);
  if (status) {
    Arcadia_Thread_setStatus(thread, status);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Object_unlock
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ObjectTag* tag = ((ObjectTag*)self) - 1;
  Arcadia_Status status = Arcadia_Process_unlockObject(Arcadia_Thread_getProcess(thread), tag);
  if (status) {
    Arcadia_Thread_setStatus(thread, status);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_TypeValue
Arcadia_Object_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  assert(NULL != thread);
  assert(NULL != self);

  ObjectTag* objectTag = ((ObjectTag*)self) - 1;
  assert(NULL != objectTag->type);
  return objectTag->type;
}

Arcadia_Object*
Arcadia_Object_clone
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  assert(NULL != self);

  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self);
  assert(NULL != type);
  Arcadia_ObjectDispatch* objectDispatch = (Arcadia_ObjectDispatch*)Arcadia_ObjectType_getDispatch(type);
  assert(NULL != objectDispatch);
  assert(((Arcadia_ObjectDispatch*)objectDispatch)->type == type);
  assert(NULL != objectDispatch->clone);

  Arcadia_Value temporary = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_Natural8Value n = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushValue(thread, &temporary);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);

  objectDispatch->clone(thread);

  if (n + 1 != Arcadia_ValueStack_getSize(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* returnValue = Arcadia_ValueStack_getObjectReferenceValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);

  return returnValue;
}

Arcadia_BooleanValue
Arcadia_Object_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value const* other
  )
{
  assert(NULL != self);
  assert(NULL != other);

  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self);
  assert(NULL != type);
  Arcadia_ObjectDispatch* objectDispatch = (Arcadia_ObjectDispatch*)Arcadia_ObjectType_getDispatch(type);
  assert(NULL != objectDispatch);
  assert(((Arcadia_ObjectDispatch*)objectDispatch)->type == type);
  assert(NULL != objectDispatch->isEqualTo);

  Arcadia_Value temporary = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_Natural8Value n = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushValue(thread, &temporary);
  Arcadia_ValueStack_pushValue(thread, other);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);

  objectDispatch->isEqualTo(thread);

  if (n + 1 != Arcadia_ValueStack_getSize(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue returnValue = Arcadia_ValueStack_getBooleanValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);

  return returnValue;
}

Arcadia_SizeValue
Arcadia_Object_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self);
  assert(NULL != type);
  Arcadia_ObjectDispatch* objectDispatch = (Arcadia_ObjectDispatch*)Arcadia_ObjectType_getDispatch(type);
  assert(NULL != objectDispatch);
  assert(((Arcadia_ObjectDispatch*)objectDispatch)->type == type);

  Arcadia_Value temporary = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_Natural8Value n = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushValue(thread, &temporary);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);

  objectDispatch->getHash(thread);

  if (n + 1 != Arcadia_ValueStack_getSize(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue returnValue = Arcadia_ValueStack_getSizeValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);

  return returnValue;
}

Arcadia_BooleanValue
Arcadia_Object_isIdenticalTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value const* other
  )
{
  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self);
  assert(NULL != type);
  Arcadia_ObjectDispatch* objectDispatch = (Arcadia_ObjectDispatch*)Arcadia_ObjectType_getDispatch(type);
  assert(NULL != objectDispatch);
  assert(((Arcadia_ObjectDispatch*)objectDispatch)->type == type);

  Arcadia_Value temporary = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_Natural8Value n = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushValue(thread, &temporary);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);

  objectDispatch->isIdenticalTo(thread);

  if (n + 1 != Arcadia_ValueStack_getSize(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue returnValue = Arcadia_ValueStack_getBooleanValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);

  return returnValue;
}

Arcadia_RuntimeUTF8String*
Arcadia_Object_toString
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_TypeValue type = Arcadia_Object_getType(thread, self);
  assert(NULL != type);
  Arcadia_ObjectDispatch* objectDispatch = (Arcadia_ObjectDispatch*)Arcadia_ObjectType_getDispatch(type);
  assert(NULL != objectDispatch);
  assert(((Arcadia_ObjectDispatch*)objectDispatch)->type == type);

  Arcadia_Value temporary = Arcadia_Value_makeObjectReferenceValue(self);
  Arcadia_Natural8Value n = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushValue(thread, &temporary);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);

  objectDispatch->toString(thread);

  if (n + 1 != Arcadia_ValueStack_getSize(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_RuntimeUTF8String* string = Arcadia_ValueStack_getRuntimeUTF8StringValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);

  return string;
}
