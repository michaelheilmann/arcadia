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

#if !defined(ARCADIA_RING1_TYPES_H_INCLUDED)
#define ARCADIA_RING1_TYPES_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/TypeSystem/Type.h"
#include "Arcadia/Ring1/Value.h"
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

/// @brief The leading, mandatory part of the dispatch of an interface type implementation.
/// @note This is part of the dispatch size of every interface type.
typedef struct Arcadia_InterfaceDispatch {
  /// The type of the object the operation is invoked on.
  Arcadia_Type* objectType;

  /// The interface type this dispatch was registered for.
  /// This is the interface type a dispatch is obtained for.
  /// @see Arcadia_ObjectType_getInterfaceDispatch
  Arcadia_Type* interfaceType;
} Arcadia_InterfaceDispatch;

/// @brief The type of a dispatch initializer for an interface type.
typedef void (Arcadia_InterfaceDispatch_InitializeCallbackFunction)(Arcadia_Thread* thread, Arcadia_InterfaceDispatch* self);


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

#include "Arcadia/Ring1/Object/TypeFunctions.i"

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
///
/// @details
/// A child type of a type is one of:
/// - For an object type: an object type having it as parent object type.
/// - For an interface type: an interface type extending it, or an object type implementing it
///   (directly, through an implemented sub-interface, or through an ancestor object type).
///
/// @note
/// This is used to destruct type nodes in dependency order, so that a type never is destructed
/// before a type it depends on. A type depending on another type is a child of that other type.
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
///
/// @details
/// This is the subtype relation, which is defined for three combinations of kinds:
/// - An object type is a descendant of an ancestor object type.
/// - An interface type is a descendant of an interface type it extends, directly or indirectly.
/// - An object type is a descendant of an interface type it implements, directly or indirectly.
///   Implementations of ancestor object types are inherited, so an object type implements every
///   interface type implemented by any of its ancestor object types.
///   An interface type is implemented if any interface type it implements is a descendant of it.
///   A type never is a descendant of a type of another kind, except as described above.
///   In particular an interface type never is a descendant of an object type.
///
/// @warning
/// A #Arcadia_BooleanValue_True result does NOT imply that a value of this type may be accessed
/// as a value of the other type by casting a pointer. That only is the case if both types are
/// object types. If @a other is an interface type, the result states that this type implements
/// @a other, and an interface dispatch must be used to invoke operations on it.
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
/// @param dispatchSize The size, in Bytes, of the dispatch of this interface type.
///                     Must be greater than or equal to <code>sizeof(Arcadia_InterfaceDispatch)</code>,
///                     because the leading part of an interface dispatch always is an #Arcadia_InterfaceDispatch.
///                     The dispatch of an interface type is independent of the dispatches of the
///                     interface types it extends, so this is not required to be greater than or equal
///                     to their dispatch sizes.
/// @note An interface type may be implemented by an object type. An object type which implements an
///       interface type must implement each interface type that interface type extends as well, with a
///       dispatch of its own.
///       @see Arcadia_ObjectType_getInterfaceDispatch
Arcadia_TypeValue
Arcadia_registerInterfaceType
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t dispatchSize,
    size_t numberOfExtendedInterfaces,
    Arcadia_TypeValue const* extendedInterfaces,
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

/// @brief A specification of an interface type implemented by an object type.
///
/// @details
/// The implemented interface types of an object type must be closed under the interface types those
/// interface types extend, because the dispatch of an interface type never is the dispatch of a
/// derived interface type. Each implemented interface type has a dispatch of its own, and the
/// dispatches of unrelated interface types have no relationship to each other.
/// @see Arcadia_registerObjectTypeWithInterfaces
typedef struct Arcadia_ObjectType_InterfaceSpecification {
  /// The implemented interface type.
  Arcadia_TypeValue interfaceType;

  /// The size, in Bytes, of the dispatch for @a interfaceType.
  /// Must be greater than or equal to the dispatch size of @a interfaceType.
  size_t dispatchSize;

  /// A pointer to the dispatch initializer for @a interfaceType or null.
  Arcadia_InterfaceDispatch_InitializeCallbackFunction* initializeDispatch;
  } Arcadia_ObjectType_InterfaceSpecification;

/// @brief Get the dispatch of an interface type for an object type.
///
/// @param self A pointer to the object type.
/// @param interfaceType A pointer to the interface type.
/// @return The pointer to the dispatch of @a interfaceType for @a self,
///         or the null pointer if @a self does not implement @a interfaceType.
///
/// @details
/// Implementations of ancestor object types are inherited, so the dispatch of an
/// implementation on an ancestor object type is returned if @a self does not implement
/// @a interfaceType itself.
///
/// Only an implementation of @a interfaceType itself matches. The dispatch of an interface type is
/// an independent dispatch which never is a dispatch of one of its ancestors, so an implementation
/// of a descendant of @a interfaceType does not match. An object type which implements a derived
/// interface type implements its ancestors as well, with a dispatch of their own.
///
/// @note The returned dispatch begins with an #Arcadia_InterfaceDispatch.
///       Its <code>objectType</code> member is the type the implementation was registered for,
///       which is @a self if the implementation was registered for @a self, and an ancestor of
///       @a self otherwise. Its <code>interfaceType</code> member is @a interfaceType.
///
/// @warning This is not a cast from an object value to an interface value; use this dispatch instead.
void*
Arcadia_ObjectType_getInterfaceDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectType* self,
    Arcadia_InterfaceType* interfaceType
  );

/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
/// @brief Register an object type.
/// @note This is equivalent to <code>Arcadia_registerObjectTypeWithInterfaces</code> with no implemented interface types.
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

/// @brief Register an object type which implements interface types.
/// @note An object type inherits the interface types implemented by its ancestor object types.
///       An inherited implementation is overridden by an implementation of the same interface type on this type.
/// @note The dispatch of an implemented interface type is initialized with a copy of the dispatch of
///       that interface type on the nearest ancestor object type, if any, before the dispatch
///       initializer of the interface specification is invoked. So an object type which overrides
///       only some of the operations of an inherited interface implementation does not have to
///       repeat the operations it does not override. An interface implementation which is not
///       inherited is initialized with all zero members, and the dispatch initializer has to set
///       every operation.
/* Arcadia_Status_ArgumentValueInvalid, Arcadia_Status_AllocationFailed, Arcadia_Status_TypeExists */
Arcadia_TypeValue
Arcadia_registerObjectTypeWithInterfaces
  (
    Arcadia_Thread* thread,
    Arcadia_Name* name,
    size_t valueSize,
    Arcadia_TypeValue parentObjectType,
    size_t dispatchSize,
    Arcadia_Type_Operations const* typeOperations,
    Arcadia_ObjectType_InterfaceSpecification const* interfaceSpecifications,
    size_t numberOfInterfaceSpecifications,
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

/// R(untime) ex(tension) macro.
/// @brief Declare an interface type.
/// @param _cilName A UTF8 string literal for the Common Intermediate Language name of the type.
/// @param _cName The C name of the type.
/// @note The dispatch of an interface type is declared by the definition of the dispatch structure
///       of the interface type, which must have a leading @c Arcadia.InterfaceDispatch.
#define Arcadia_declareInterfaceType(_cilName, _cName) \
  typedef struct _cName##Dispatch _cName##Dispatch; \
  Arcadia_TypeValue \
  _##_cName##_getType \
    ( \
      Arcadia_Thread* thread \
    );

/// R(untime) ex(tension) macro.
/// @brief Define an interface type.
/// @param _cilName A UTF8 string literal for the Common Intermediate Language name of the type.
/// @param _cName The C name of the type.
/// @param ... The calls of the @c _getType functions of the extended interface types, followed by
///        @c NULL. Pass only @c NULL if the interface type extends no interface type.
/// @note The dispatch size of the interface type is the size of the dispatch structure of the
///       interface type.
#define Arcadia_defineInterfaceType(_cilName, _cName, ...) \
  static Arcadia_TypeValue g_##_cName##_type = NULL; \
  \
  static void \
  _##_cName##_typeDestructing \
    ( \
      void* context \
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
      Arcadia_TypeValue extends[] = { __VA_ARGS__ }; \
      size_t numberOfExtends = 0; \
      while (extends[numberOfExtends]) { \
        ++numberOfExtends; \
      } \
      g_##_cName##_type = Arcadia_registerInterfaceType \
        ( \
          thread, \
          Arcadia_Names_getOrCreateName \
            ( \
              thread, \
              _cilName, \
              sizeof(_cilName) - 1 \
            ), \
          sizeof(_cName##Dispatch), \
          numberOfExtends, \
          extends, \
          &_##_cName##_typeDestructing \
        ); \
    } \
    return g_##_cName##_type; \
  }

/// R(untime) ex(tension) macro.
/// @brief Define an object type which implements interface types.
/// @param _cilName A UTF8 string literal for the Common Intermediate Language name of the type.
/// @param _cName The C name of the type.
/// @param _cilParentName A UTF8 string literal for the Common Intermediate Language name of the
///        parent type.
/// @param _cParentName The C name of the parent type.
/// @param _cTypeOperations A pointer to the type operations of the type.
/// @param ... A comma separated list of brace enclosed initializers of
///        @c Arcadia_ObjectType_InterfaceSpecification, one for each implemented interface type.
///        Do not enclose the list in a further pair of braces.
/// @note The type is declared by @c Arcadia_declareObjectType. Each implemented interface type and
///       each interface type those interface types extend must have a specification of its own.
#define Arcadia_defineObjectTypeWithInterfaces(_cilName, _cName, _cilParentName, _cParentName, _cTypeOperations, ...) \
  static Arcadia_TypeValue g_##_cName##_type = NULL; \
  \
  static void \
  _##_cName##_typeDestructing \
    ( \
      void* context \
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
      Arcadia_ObjectType_InterfaceSpecification interfaceSpecifications[] = { \
        __VA_ARGS__, \
        { NULL, 0, NULL } \
      }; \
      size_t numberOfInterfaceSpecifications = 0; \
      while (interfaceSpecifications[numberOfInterfaceSpecifications].interfaceType) { \
        ++numberOfInterfaceSpecifications; \
      } \
      g_##_cName##_type = Arcadia_registerObjectTypeWithInterfaces \
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
          interfaceSpecifications, \
          numberOfInterfaceSpecifications, \
          &_##_cName##_typeDestructing \
        ); \
    } \
    return g_##_cName##_type; \
  }

#endif // ARCADIA_RING1_TYPES_H_INCLUDED
