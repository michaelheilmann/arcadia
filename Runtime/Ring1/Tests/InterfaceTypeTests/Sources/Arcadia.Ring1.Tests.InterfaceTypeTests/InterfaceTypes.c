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

#include <string.h>
#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/Helper.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceTypes.h"

Helper_defineObjectTypeOperations(objectA)
Helper_defineObjectTypeOperations(objectB)
Helper_defineObjectTypeOperations(objectC)
Helper_defineObjectTypeOperations(objectD)

#undef Helper_defineObjectTypeOperations

/// Register the interface types "Arcadia.Tests.InterfaceTypeTests.A", "B", and "C", where "B"
/// extends "A" and "C" extends "B". Also register the object types "ObjectA" (implements "A" and
/// "B"), "ObjectB" (derived from "ObjectA", so it inherits those implementations), "ObjectC"
/// (implements no interface type), and "ObjectD" (has no parent object type). Then verify that:
/// - An interface type is an interface kind and not an object kind, and is a descendant of
///   itself. "B" and "C" are descendants of "A", "C" is a descendant of "B", and no interface
///   type is a descendant of its own descendant. "A" and "B" have children, "C" does not.
/// - An object type which implements an interface type is a descendant of that interface type,
///   and the implemented interface types are closed under the interface types they extend.
///   "ObjectB" is a descendant of the interface types implemented by its ancestor "ObjectA".
///   "ObjectC" and "ObjectD" are not, and "ObjectD" is not a descendant of the root object type
///   either. An interface type is never a descendant of an object type, and object type
///   inheritance is unaffected. Implemented interface types are parent types, so they are
///   destructed after the objects.
/// - Interface dispatch lookup matches the interface type exactly. The dispatches of "A" and "B"
///   are distinct allocations. The derived "C" is not implemented just because the ancestor "B"
///   is. A descendant object type inherits the implementation of its ancestor object type: the
///   dispatch of the ancestor is returned unchanged. An object type with no implementation has no
///   dispatch, and a non-interface type has no dispatch.
/// - An interface reference in C is just the "Arcadia.Object". An object reference value has the
///   type of the referenced object, is an instance of the interface types the object implements
///   and of their ancestors, reference value equality and hashing work, and a checked extraction
///   accepts an implemented interface type and its ancestors.
void
testInterfaceTypes
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue interfaceA =
    Arcadia_registerInterfaceType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.A", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.A") - 1),
        sizeof(Arcadia_InterfaceDispatch),
        0,
        NULL,
        &Helper_typeDestructing
      );

  Arcadia_TypeValue interfaceBExtends[] = { interfaceA };
  Arcadia_TypeValue interfaceB =
    Arcadia_registerInterfaceType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.B", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.B") - 1),
        sizeof(Arcadia_InterfaceDispatch),
        1,
        interfaceBExtends,
        &Helper_typeDestructing
      );

  Arcadia_TypeValue interfaceCExtends[] = { interfaceB };
  Arcadia_TypeValue interfaceC =
    Arcadia_registerInterfaceType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.C", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.C") - 1),
        sizeof(Arcadia_InterfaceDispatch),
        1,
        interfaceCExtends,
        &Helper_typeDestructing
      );

  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isInterfacelKind(thread, interfaceA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isObjectKind(thread, interfaceA));

  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, interfaceA, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, interfaceB, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, interfaceC, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, interfaceC, interfaceB));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, interfaceA, interfaceB));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, interfaceB, interfaceC));

  Arcadia_Tests_assertTrue(thread, Arcadia_Type_hasChildren(thread, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_hasChildren(thread, interfaceB));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_hasChildren(thread, interfaceC));

  // An object type which implements an interface type is a descendant of that interface type. The
  // implemented interface types must be closed under the interface types they extend, because the
  // dispatch of a derived interface type never is a dispatch of an ancestor interface type, so
  // implementing interfaceB requires implementing interfaceA, too.
  Arcadia_ObjectType_InterfaceSpecification objectAInterfaces[] = {
    { .interfaceType = interfaceA, .dispatchSize = sizeof(Arcadia_InterfaceDispatch), .initializeDispatch = NULL },
    { .interfaceType = interfaceB, .dispatchSize = sizeof(Arcadia_InterfaceDispatch), .initializeDispatch = NULL }
  };
  Arcadia_TypeValue objectA =
    Arcadia_registerObjectTypeWithInterfaces
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.ObjectA", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.ObjectA") - 1),
        0,
        _Arcadia_Object_getType(thread),
        sizeof(Arcadia_ObjectDispatch),
        &_objectA_typeOperations,
        objectAInterfaces,
        2,
        &Helper_typeDestructing
      );
  g_objectA_type = objectA;

  // An object type inherits the interface types implemented by its ancestor object types.
  Arcadia_TypeValue objectB =
    Arcadia_registerObjectType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.ObjectB", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.ObjectB") - 1),
        0,
        objectA,
        sizeof(Arcadia_ObjectDispatch),
        &_objectB_typeOperations,
        &Helper_typeDestructing
      );
  g_objectB_type = objectB;

  // An object type which implements no interface type.
  Arcadia_TypeValue objectC =
    Arcadia_registerObjectType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.ObjectC", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.ObjectC") - 1),
        0,
        _Arcadia_Object_getType(thread),
        sizeof(Arcadia_ObjectDispatch),
        &_objectC_typeOperations,
        &Helper_typeDestructing
      );
  g_objectC_type = objectC;

  // An object type without a parent object type and without virtual functions.
  Arcadia_TypeValue objectD =
    Arcadia_registerObjectType
      (
        thread,
        Arcadia_Names_getOrCreateName(thread, u8"Arcadia.Tests.InterfaceTypeTests.ObjectD", sizeof(u8"Arcadia.Tests.InterfaceTypeTests.ObjectD") - 1),
        0,
        NULL,
        0,
        &_objectD_typeOperations,
        &Helper_typeDestructing
      );
  g_objectD_type = objectD;

  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectA, interfaceB));
  // Transitively, because interfaceB extends interfaceA.
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectA, interfaceA));
  // Inherited from the ancestor object type objectA.
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectB, interfaceB));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectB, interfaceA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, objectC, interfaceB));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, objectC, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectD, objectD));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, objectD, interfaceA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, objectD, _Arcadia_Object_getType(thread)));

  // An interface type never is a descendant of an object type.
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, interfaceA, objectA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, interfaceB, objectA));

  // Object type inheritance is unaffected.
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectA, objectA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectA, _Arcadia_Object_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectB, objectA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_isDescendantType(thread, objectA, objectB));

  // An interface type which is implemented is a parent type, so it is destructed later.
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_hasChildren(thread, interfaceB));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_hasChildren(thread, interfaceA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_hasChildren(thread, objectA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_hasChildren(thread, objectC));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Type_hasChildren(thread, objectD));

  // Dispatch lookup.
  Arcadia_InterfaceDispatch* objectADispatchOfB =
    (Arcadia_InterfaceDispatch*)Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectA, (Arcadia_InterfaceType*)interfaceB);
  Arcadia_Tests_assertTrue(thread, NULL != objectADispatchOfB);
  if (objectADispatchOfB) {
    Arcadia_Tests_assertTrue(thread, objectADispatchOfB->objectType == objectA);
    Arcadia_Tests_assertTrue(thread, objectADispatchOfB->interfaceType == interfaceB);
  }
  // interfaceA is an ancestor of the implemented interfaceB, but the dispatch of interfaceB is not a
  // dispatch of interfaceA, so the implementation of interfaceA is used for it.
  Arcadia_InterfaceDispatch* objectADispatchOfA =
    (Arcadia_InterfaceDispatch*)Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectA, (Arcadia_InterfaceType*)interfaceA);
  Arcadia_Tests_assertTrue(thread, NULL != objectADispatchOfA);
  if (objectADispatchOfA) {
    Arcadia_Tests_assertTrue(thread, objectADispatchOfA->objectType == objectA);
    Arcadia_Tests_assertTrue(thread, objectADispatchOfA->interfaceType == interfaceA);
  }
  // The implementations of interfaceA and interfaceB are distinct dispatches.
  if (objectADispatchOfB && objectADispatchOfA) {
    Arcadia_Tests_assertTrue(thread, objectADispatchOfB != objectADispatchOfA);
  }
  // A derived interface type is not implemented by implementing one of its ancestors, either.
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectA, (Arcadia_InterfaceType*)interfaceC));
  // objectB implements interfaceB through its ancestor objectA, so it gets the ancestor's dispatch.
  Arcadia_InterfaceDispatch* objectBDispatchOfB =
    (Arcadia_InterfaceDispatch*)Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectB, (Arcadia_InterfaceType*)interfaceB);
  Arcadia_Tests_assertTrue(thread, NULL != objectBDispatchOfB);
  if (objectBDispatchOfB) {
    Arcadia_Tests_assertTrue(thread, objectBDispatchOfB->objectType == objectA);
    Arcadia_Tests_assertTrue(thread, objectBDispatchOfB->interfaceType == interfaceB);
  }
  // interfaceC extends interfaceB, but interfaceC is not implemented, and no ancestor of it is.
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectA, (Arcadia_InterfaceType*)interfaceC));
  // An object type which implements no interface type has no dispatch for any interface type.
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectC, (Arcadia_InterfaceType*)interfaceA));
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectD, (Arcadia_InterfaceType*)interfaceA));
  // A type which is not an interface type has no dispatch.
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)objectA, (Arcadia_InterfaceType*)objectA));


  // An interface type is implemented by an object, and an interface-typed reference in C code is
  // that object. The interface type is therefore not erased, and is never stored in an
  // "Arcadia.Value" of an object reference: an object reference value of an object which implements
  // an interface type is an instance of that interface type, and a checked extraction of an object
  // reference value accepts that interface type.
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Object* objectAInstance = (Arcadia_Object*)_Arcadia_EndCreate0(thread, objectA, oldValueStackSize);
  Arcadia_Tests_assertTrue(thread, NULL != objectAInstance);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_getType(thread, objectAInstance) == objectA);

  oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Object* objectCInstance = (Arcadia_Object*)_Arcadia_EndCreate0(thread, objectC, oldValueStackSize);
  Arcadia_Tests_assertTrue(thread, NULL != objectCInstance);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_getType(thread, objectCInstance) == objectC);

  // The object reference value of an object which implements an interface type.
  Arcadia_Value valueOfObjectA =
    Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)objectAInstance);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&valueOfObjectA));
  // The type of an object reference value is the type of the referenced object.
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getType(thread, &valueOfObjectA) == objectA);
  // It is an instance of the interface types the object implements, and of their ancestor interface
  // types, as well as of the types of the object.
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInstanceOf(thread, &valueOfObjectA, objectA));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInstanceOf(thread, &valueOfObjectA, interfaceB));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInstanceOf(thread, &valueOfObjectA, interfaceA));
  // It is not an instance of an interface type which the object does not implement.
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isInstanceOf(thread, &valueOfObjectA, interfaceC));

  // The object reference value of an object which implements no interface type.
  Arcadia_Value valueOfObjectC =
    Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)objectCInstance);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getType(thread, &valueOfObjectC) == objectC);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isInstanceOf(thread, &valueOfObjectC, interfaceA));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isInstanceOf(thread, &valueOfObjectC, interfaceB));

  // Equality of two object reference values of the same object, and of different objects.
  Arcadia_Value valueOfObjectA2 =
    Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)objectAInstance);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &valueOfObjectA, &valueOfObjectA2));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getHash(thread, &valueOfObjectA) == Arcadia_Value_getHash(thread, &valueOfObjectA2));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isEqualTo(thread, &valueOfObjectA, &valueOfObjectC));

  // A checked extraction of an object reference value accepts an interface type the object
  // implements, and that interface type's ancestor interface types.
  Arcadia_Object* extracted =
    Arcadia_Value_getObjectReferenceValueChecked(thread, valueOfObjectA, interfaceB);
  Arcadia_Tests_assertTrue(thread, extracted == objectAInstance);
  extracted = Arcadia_Value_getObjectReferenceValueChecked(thread, valueOfObjectA, interfaceA);
  Arcadia_Tests_assertTrue(thread, extracted == objectAInstance);
}
