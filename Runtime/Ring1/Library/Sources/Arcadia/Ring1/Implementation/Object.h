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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECT_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECT_H_INCLUDED

#include "Arcadia/Ring1/Implementation/TypeSystem/Include.h"
#include "Arcadia/ARMS/Include.h"
typedef struct Arcadia_Value Arcadia_Value;

typedef struct Arcadia_Object Arcadia_Object;

typedef struct Arcadia_ObjectDispatch {
  Arcadia_Type* type;

  Arcadia_ForeignProcedure* add;
  Arcadia_ForeignProcedure* and;
  Arcadia_ForeignProcedure* clone;
  Arcadia_ForeignProcedure* concatenate;
  Arcadia_ForeignProcedure* divide;
  Arcadia_ForeignProcedure* getHash;
  Arcadia_ForeignProcedure* isIdenticalTo;
  Arcadia_ForeignProcedure* isEqualTo;
  Arcadia_ForeignProcedure* isGreaterThan;
  Arcadia_ForeignProcedure* isGreaterThanOrEqualTo;
  Arcadia_ForeignProcedure* isLowerThan;
  Arcadia_ForeignProcedure* isLowerThanOrEqualTo;
  Arcadia_ForeignProcedure* multiply;
  Arcadia_ForeignProcedure* negate;
  Arcadia_ForeignProcedure* not;
  Arcadia_ForeignProcedure* or;
  Arcadia_ForeignProcedure* subtract;
  Arcadia_ForeignProcedure* toString;

} Arcadia_ObjectDispatch;

Arcadia_TypeValue
_Arcadia_Object_getType
  (
    Arcadia_Thread* thread
  );

struct Arcadia_Object {
  int dummy;
};

#define Arcadia_Configuration_withBarriers (Arcadia_ARMS_Configuration_WithBarriers)

#define Arcadia_superTypeConstructor(_thread, _type, _self) \
  { \
    Arcadia_Type_getOperations(Arcadia_ObjectType_getParentObjectType(thread, _type))->objectTypeOperations->construct(_thread, (Arcadia_Object*)_self); \
  }

/// R(untime) ex(tension) macro.
/// @param _cilName, _cilParentName UTF8 string literals for the Common Intermediate Language type names of the type and its parent type.
#define Arcadia_declareObjectType(_cilName, _cName, _cilParentName) \
  typedef struct _cName##Dispatch _cName##Dispatch; \
  typedef struct _cName _cName; \
  Arcadia_TypeValue \
  _##_cName##_getType \
    ( \
      Arcadia_Thread* thread \
    );

/// R(untime) ex(tension) macro.
/// @param _cilName, _cilParentName UTF8 string literals for the Common Intermediate Language type names of the type and its parent type.
#define Arcadia_defineObjectType(_cilName, _cName, _cilParentName, _cParentName, _cTypeOperations) \
  static Arcadia_TypeValue g_##_cName##_type = NULL; \
  \
  static void \
  _##_cName##_typeDestructing \
    ( \
      void *context \
    ) \
  { \
    g_##_cName##_type = NULL; \
  } \
  \
  Arcadia_TypeValue \
  _##_cName##_getType \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    if (!g_##_cName##_type) { \
      Arcadia_TypeValue parentType = _##_cParentName##_getType(thread); \
      g_##_cName##_type = Arcadia_registerObjectType \
        ( \
          thread, \
          Arcadia_Names_getOrCreateName \
            ( \
              thread, \
              _cilName, \
              sizeof(_cilName) - 1 \
            ), \
          sizeof(_cName), \
          parentType, \
          sizeof(_cName##Dispatch), \
          _cTypeOperations, \
          &_##_cName##_typeDestructing \
        ); \
    } \
    return g_##_cName##_type; \
  }


/// @brief
/// Allocate an "Arcadia.Object" or derived type value.
///
/// @details
/// a) Raise "Arcadia.Status.ArgumentValueInvalid" if "type" is a null pointer.
///    Raise "Arcadia.Status.ArgumentTypeInvalid" if "type" is not an "Arcadia.Object" or derived type.
///
/// b) Allocate memory of size "Arcadia_Type_getValueSize(type)" at address "a".
///    Raise "Arcadia.Status.AllocationFailed" if the allocation fails.
///    Assign this memory the "Arcadia.Memory" type.
///
/// c) Cast that memory into an object
///
/// d) Invoke the constructor of type "type" with the specified number of argument values.
///
/// e) Assert the stack is not corrupted if a) - c) are successfull. Otherwise raise Arcadia.Status.StackCorruption.
void*
_Arcadia_EndCreate0
  (
    Arcadia_Thread* thread,
    Arcadia_Type* type,
    Arcadia_SizeValue oldValueStackSize
  );

/// @deprecated
#define _Arcadia_EndCreate(type) \
  return _Arcadia_EndCreate0(thread, _##type##_getType(thread), oldValueStackSize);

#define _Arcadia_BeginCreate(type) \
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);

#define _Arcadia_EndCreate(type) \
  return _Arcadia_EndCreate0(thread, _##type##_getType(thread), oldValueStackSize);

void
Arcadia_Object_setType
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_TypeValue type
  );

/// @brief Visit an object.
/// @param self A pointer to the object.
void
Arcadia_Object_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

void
Arcadia_Object_addNotifyDestroyCallback
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    void* observer,
    void (*callback)(void* observer, Arcadia_Object*)
  );

void
Arcadia_Object_removeNotifyDestroyCallback
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    void* observer,
    void (*callback)(void* observer, Arcadia_Object*)
  );

#if Arcadia_Configuration_withBarriers

/// @brief A "forward" barrier.
/// If both @a source and @a target are not null,
/// and if @a source is black and if @a target is white,
/// then @a target becomes gray.
void
Arcadia_Object_forwardBarrier
  (
    Arcadia_Thread * thread,
    Arcadia_Object * source,
    Arcadia_Object * target
  );

/// @brief A "backward" barrier.
/// If both @a source and @a target are not null,
/// and if @a source is black and if @a target is white,
/// then @a source becomes gray.
void
Arcadia_Object_backwardBarrier
  (
    Arcadia_Thread * thread,
    Arcadia_Object* source,
    Arcadia_Object* target
  );

#endif

/// @brief Increment the lock count of the object.
/// @param self A pointer to the object.
void
Arcadia_Object_lock
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

/// @brief Decrement the lock count of the object.
/// @param self A pointer to the object.
void
Arcadia_Object_unlock
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

/// @brief Get the type of an object.
/// @param self A pointer to the object.
/// @return The type of an object.
Arcadia_TypeValue
Arcadia_Object_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static inline Arcadia_BooleanValue
Arcadia_Object_isInstanceOf
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Type* type
  )
{ return Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, object), type); }

// "clone"
Arcadia_Object*
Arcadia_Object_clone
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

/// "isEqualTo"
Arcadia_BooleanValue
Arcadia_Object_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value const* other
  );

/// "hash"
Arcadia_SizeValue
Arcadia_Object_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

/// "isIdenticalTo"
Arcadia_BooleanValue
Arcadia_Object_isIdenticalTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value const* other
  );

/// "toString"
Arcadia_RuntimeUTF8String*
Arcadia_Object_toString
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

/// Utility macro to define the body of a virtual call with return value.
#define Arcadia_VirtualCallWithReturn(Type, Function, ...) \
  Arcadia_Type* t = Arcadia_Object_getType(thread, (Arcadia_Object*)self); \
  Type##Dispatch* d = (Type##Dispatch*)Arcadia_ObjectType_getDispatch(t); \
  return d->Function(thread, __VA_ARGS__);

/// Utility macro to define the body of a virtual call without return value.
#define Arcadia_VirtualCall(Type, Function, ...) \
  Arcadia_Type* t = Arcadia_Object_getType(thread, (Arcadia_Object*)self); \
  Type##Dispatch* d = (Type##Dispatch*)Arcadia_ObjectType_getDispatch(t); \
  d->Function(thread, __VA_ARGS__);

/// Utility macro to define the header of a constructor.
#define Arcadia_EnterConstructor(Type) \
  Arcadia_TypeValue _type = _##Type##_getType(thread); \
  if (Arcadia_ValueStack_getSize(thread) < 1) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Natural8Value _numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);

/// Utility macro to define the footer of a constructor.
#define Arcadia_LeaveConstructor(Type) \
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type); \
  Arcadia_ValueStack_popValues(thread, _numberOfArguments + 1);

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECT_H_INCLUDED
