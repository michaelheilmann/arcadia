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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#include "Arcadia/Ring1/Implementation/TypeSystem/TypeKind.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Names.h"

/// @brief The opaque C type representing a type.
/// Types are not garbage collected. Once a type was added, it remains there until the process terminates.
typedef void Arcadia_Type;

/// @brief The opaque C type representing an enumeration type.
typedef void Arcadia_EnumerationType;

/// @brief The opaque C type representing an interface type.
typedef void Arcadia_InterfaceType;

/// @brief The opaque C type representing an internal type.
typedef void Arcadia_InternalType;

/// @brief The opaque C type representing an object type.
typedef void Arcadia_ObjectType;

/// @brief The opaque C type representing a scalar type.
typedef void Arcadia_ScalarType;

/// @brief A pointer to a type or the null pointer.
typedef Arcadia_Type* Arcadia_TypeValue;

/// @brief Get the type of a specified name.
/// @param thread A pointer to this thread.
/// @param name A poiner to the name.
/// @return A pointer to the type.
/// @error #Arcadia_Status_ArgumentValueInvalid @a name is a null pointer
/// @error Arcadia_Status_TypeNotExists no type of the specified name was found
Arcadia_TypeValue
Arcadia_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name
  );

/// @brief Get the name of this type.
/// @param thread A pointer to this thread.
/// @param self A pointer to this type.
/// @return A pointer to the name off this type.
Arcadia_Name*
Arcadia_Type_getName
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  );

/// @brief Get the hash of this type.
/// @param thread A pointer to this thread.
/// @param self A pointer to this type.
/// @return The hash of this type.
size_t
Arcadia_Type_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  );

/// @brief Get the type kind of this type.
/// @param thread A pointer to this thread.
/// @param self A pointer to this type.
/// @return The kind of this type.
Arcadia_TypeKind
Arcadia_Type_getKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  );

/// @brief Get if this type is of the kind of type "enumeration".
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if the type is of the kind of type "enumeration". #Arcadia_BooleanValue_False otherwise.
static inline bool
Arcadia_Type_isEnumerationtKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{ return Arcadia_TypeKind_Enumeration == Arcadia_Type_getKind(thread, self); }

/// @brief Get if this type is of the kind of type "interface".
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if the type is of the kind of type "interface". #Arcadia_BooleanValue_False otherwise.
static inline bool
Arcadia_Type_isInterfacelKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{ return Arcadia_TypeKind_Interface == Arcadia_Type_getKind(thread, self); }

/// @brief Get if this type is of the kind of type "internal".
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if the type is of the kind of type "internal". #Arcadia_BooleanValue_False otherwise.
static inline bool
Arcadia_Type_isInternalKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{ return Arcadia_TypeKind_Internal == Arcadia_Type_getKind(thread, self); }

/// @brief Get if this type is of the kind of type "object".
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if the type is of the kind of type "object". #Arcadia_BooleanValue_False otherwise.
static inline bool
Arcadia_Type_isObjectKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{ return Arcadia_TypeKind_Object == Arcadia_Type_getKind(thread, self); }

/// @brief Get if this type is of the kind of type "scalar".
/// @param self A pointer to this type.
/// @return #Arcadia_BooleanValue_True if the type is of the kind of type "scalar". #Arcadia_BooleanValue_False otherwise.
static inline bool
Arcadia_Type_isScalarKind
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue self
  )
{ return Arcadia_TypeKind_Scalar == Arcadia_Type_getKind(thread, self); }

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPE_H_INCLUDED
