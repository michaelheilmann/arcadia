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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPES_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPES_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/TypeSystem/Type.h"
#include "Arcadia/Ring1/Implementation/Value.h"
typedef struct Arcadia_Object Arcadia_Object;
typedef struct Arcadia_ObjectDispatch Arcadia_ObjectDispatch;

/// @brief Type of a construct callback function for values of Arcadia.Object or derived type.
/// Each object type has its construct callback function.
/// For an object dispatch of type A1 <: A2 <: ... <: An the callback functions are invoked from the top mpost to the bottom most type.
typedef void (Arcadia_Object_ConstructCallbackFunction)(Arcadia_Thread* thread, Arcadia_Object* self);

/// @brief Type of a destruct callback function for values of Arcadia.Object or derived type.
/// Each object type may have its own destruct callback function.
/// For an object of type A1 <: A2 <: ... <: An the callback functions are invoked from the bottom most to the top most type.
/// @warning Such a function MAY set the by-thread status variable. Such a function MAY NOT jump.
typedef void (Arcadia_Object_DestructCallbackFunction)(Arcadia_Thread* thread, Arcadia_Object* self);

/// @brief Type of a visti callback function for object types.
/// Each object type may have its own visit callback function.
/// For an object of type A1 <: A2 <: ... <: An the callback functions are invoked from the bottom most to the top most type.
/// @warning Such a function MAY set the by-thread status variable. Such a function MAY NOT jump.
typedef void (Arcadia_Object_VisitCallbackFunction)(Arcadia_Thread* thread, Arcadia_Object* self);

/// The type of an initializer function for object type dispatches.
/// Each object type may have its initialize function for object type dispatches.
/// For an object dispatch of type A1 <: A2 <: ... <: An the callback functions are invoked from the top mpost to the bottom most type.
typedef void (Arcadia_ObjectDispatch_InitializeCallbackFunction)(Arcadia_Thread* thread, Arcadia_ObjectDispatch* self);


/// Type operations for object types.
typedef struct Arcadia_ObjectType_Operations {
  Arcadia_Object_ConstructCallbackFunction* construct;
  Arcadia_Object_DestructCallbackFunction* destruct;
  Arcadia_Object_VisitCallbackFunction* visit;
  Arcadia_ObjectDispatch_InitializeCallbackFunction* initializeDispatch;
} Arcadia_ObjectType_Operations;

#define Arcadia_ObjectType_Operations_Initializer \
  .construct = NULL, \
  .destruct = NULL, \
  .visit = NULL, \
  .initializeDispatch = NULL

/// Type operations for all types.
typedef struct Arcadia_Type_Operations {

  /// Pointer to the object type operations if the type is an object type.
  /// The null pointer otherwise.
  Arcadia_ObjectType_Operations const* objectTypeOperations;

#define Operation(Name) \
  Arcadia_ForeignProcedure* Name;

#include "Arcadia/Ring1/Implementation/Object/TypeFunctions.i"

#undef Operation

} Arcadia_Type_Operations;

#define Arcadia_Type_Operations_Initializer \
  .objectTypeOperations = NULL, \
  .add = NULL, \
  .and = NULL, \
  .concatenate = NULL, \
  .divide = NULL, \
  .getHash = NULL, \
  .isEqualTo = NULL, \
  .isGreaterThan = NULL, \
  .isGreaterThanOrEqualTo = NULL, \
  .isIdenticalTo = NULL, \
  .isLowerThan = NULL, \
  .isLowerThanOrEqualTo = NULL, \
  .isNotEqualTo = NULL, \
  .multiply = NULL, \
  .negate = NULL, \
  .not = NULL, \
  .or = NULL, \
  .subtract = NULL, \
  .toString = NULL

/// @brief Type of a type destructing callback function.
/// Invoked when the type is destructing.
typedef void (Arcadia_Type_TypeDestructingCallbackFunction)(void* context);

static inline void
Arcadia_Type_visit
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{/*Intentionally empty.*/}

/// @brief Get the size, in Bytes, of a value of this type.
/// @param self A pointer to this type.
/// @return The size, in Bytes, of a value of this type.
Arcadia_SizeValue
Arcadia_EnumerationType_getValueSize
  (
    Arcadia_Thread* thread,
    Arcadia_EnumerationType* self
  );

/// @brief Get the size, in Bytes, of a value of this type.
/// @param self A pointer to this type.
/// @return The size, in Bytes, of a value of this type.
Arcadia_SizeValue
Arcadia_ObjectType_getValueSize
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  );

/// @brief Get the parent object type of this type.
/// @param self A pointer to this type.
/// @return A pointer to the parent object type of this type if any. The null pointer otherwise.
Arcadia_TypeValue
Arcadia_ObjectType_getParentObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self
  );

/// @brief Get the visit object callback function of this type.
/// @param self A pointer to this type.
/// @return A pointer to the visit callback function of this type if any. The null pointer otherwise.
Arcadia_Object_VisitCallbackFunction*
Arcadia_Type_getVisitObjectCallbackFunction
  (
    Arcadia_TypeValue self
  );

/// @brief Get the destruct object callback function of this type.
/// @param self A pointer to this type.
/// @return A pointer to the destruct callback function of this type if any. The null pointer otherwise.
Arcadia_Object_DestructCallbackFunction*
Arcadia_Type_getDestructObjectCallbackFunction
  (
    Arcadia_TypeValue self
  );

/// @brief Get if this type has child types.
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if this type has child types. #Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_Type_hasChildren
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  );

/// @brief Get if this type is a descendant type of another type.
/// @param self A pointer to this type.
/// @param other A pointer to the other type.
/// @return #Arcadia_BooleanValue_True if this type is a descendant type of the other type. #Arcadia_BooleanValue_False otherwise.
/// @todo Rename to isDescendantType.
Arcadia_BooleanValue
Arcadia_Type_isDescendantType
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self,
    Arcadia_TypeValue other
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
Arcadia_TypeValue
Arcadia_registerEnumerationType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
// Interfaces cannot have type operations.
Arcadia_TypeValue
Arcadia_registerInterfaceType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t dispatchSize,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
Arcadia_TypeValue
Arcadia_registerInternalType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
Arcadia_TypeValue
Arcadia_registerObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeValue parentObjectType,
    size_t dispatchSize,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
Arcadia_TypeValue
Arcadia_registerScalarType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing
  );

Arcadia_ObjectDispatch*
Arcadia_ObjectType_getDispatch
  (
    Arcadia_ObjectType* type
  );

Arcadia_Type_Operations const*
Arcadia_Type_getOperations
  (
    Arcadia_TypeValue type
  );

/// @brief Get the "Arcadia.Memory" type.
/// @param thread A pointer to the Arcadia_Thread object.
/// @return The "Arcadia.Memory" type.
Arcadia_TypeValue
_Arcadia_Memory_getType
  (
    Arcadia_Thread* thread
  );

/// @brief Get the "Arcadia.Type" type.
/// @param thread A pointer to the Arcadia_Thread object.
/// @return The "Arcadia.Type" type.
Arcadia_TypeValue
_Arcadia_Type_getType
  (
    Arcadia_Thread* thread
  );

/// @brief Get the "Arcadia.Atom" type.
/// @param thread A pointer to the Arcadia_Thread object.
/// @return The "Arcadia.Atom" type.
Arcadia_TypeValue
_Arcadia_AtomValue_getType
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPES_H_INCLUDED
