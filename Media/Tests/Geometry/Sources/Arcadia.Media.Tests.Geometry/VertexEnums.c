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

#include "VertexEnums.h"
#include "Arcadia/Media/Include.h"
#include <string.h>

#if defined(ARCADIA_MEDIA_PRIVATE)
  #error("Media's public aggregate must not leak its private include switch")
#endif

_Static_assert(Arcadia_Media_VertexElementSemantics_PositionXYZ == 1, "PositionXYZ value");
_Static_assert(Arcadia_Media_VertexElementSemantics_AmbientRGBA == 2, "AmbientRGBA value");
_Static_assert(Arcadia_Media_VertexElementSemantics_AmbientUV == 3, "AmbientUV value");
_Static_assert(Arcadia_Media_VertexElementSyntactics_Real32 == 1, "Real32 value");
_Static_assert(Arcadia_Media_VertexElementSyntactics_Real32Real32 == 2, "Real32Real32 value");
_Static_assert(Arcadia_Media_VertexElementSyntactics_Real32Real32Real32 == 3, "Real32Real32Real32 value");
_Static_assert(Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32 == 4, "Real32Real32Real32Real32 value");
_Static_assert(sizeof(Arcadia_Media_VertexElementSemantics) == sizeof(Arcadia_Integer32Value), "semantics size");
_Static_assert(sizeof(Arcadia_Media_VertexElementSyntactics) == sizeof(Arcadia_Integer32Value), "syntactics size");
_Static_assert(_Alignof(Arcadia_Media_VertexElementSemantics) == _Alignof(Arcadia_Integer32Value), "semantics alignment");
_Static_assert(_Alignof(Arcadia_Media_VertexElementSyntactics) == _Alignof(Arcadia_Integer32Value), "syntactics alignment");
_Static_assert(_Generic(&_Arcadia_Media_VertexElementSemantics_getType,
  Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "semantics descriptor signature");
_Static_assert(_Generic(&_Arcadia_Media_VertexElementSyntactics_getType,
  Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "syntactics descriptor signature");
_Static_assert(_Generic(&Arcadia_Media_VertexElementDescriptor_create,
  Arcadia_Media_VertexElementDescriptor* (*)(Arcadia_Thread*, Arcadia_Natural8Value,
    Arcadia_Media_VertexElementSemantics, Arcadia_Media_VertexElementSyntactics): 1, default: 0), "native element factory signature");
_Static_assert(_Generic(&Arcadia_Media_VertexElementDescriptor_getSemantics,
  Arcadia_Media_VertexElementSemantics (*)(Arcadia_Thread*, Arcadia_Media_VertexElementDescriptor*): 1, default: 0), "native semantics getter signature");
_Static_assert(_Generic(&Arcadia_Media_VertexElementDescriptor_getSyntactics,
  Arcadia_Media_VertexElementSyntactics (*)(Arcadia_Thread*, Arcadia_Media_VertexElementDescriptor*): 1, default: 0), "native syntactics getter signature");

static const Arcadia_Media_VertexElementSemantics semantics[] = {
  Arcadia_Media_VertexElementSemantics_PositionXYZ,
  Arcadia_Media_VertexElementSemantics_AmbientRGBA,
  Arcadia_Media_VertexElementSemantics_AmbientUV,
};

static const Arcadia_Media_VertexElementSyntactics syntactics[] = {
  Arcadia_Media_VertexElementSyntactics_Real32,
  Arcadia_Media_VertexElementSyntactics_Real32Real32,
  Arcadia_Media_VertexElementSyntactics_Real32Real32Real32,
  Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32,
};

static void
checkType
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    const char* expectedName
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == strlen(expectedName));
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expectedName, strlen(expectedName)));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
}

static void
checkElement
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* element,
    Arcadia_Natural32Value offset,
    Arcadia_SizeValue semanticsIndex,
    Arcadia_SizeValue syntacticsIndex
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexElementDescriptor_getSource(thread, element) == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexElementDescriptor_getOffset(thread, element) == offset);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexElementDescriptor_getSemantics(thread, element) == semantics[semanticsIndex]);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexElementDescriptor_getSyntactics(thread, element) == syntactics[syntacticsIndex]);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexElementDescriptor_getSize(thread, element) == (syntacticsIndex + 1) * sizeof(Arcadia_Real32Value));
}

static void
testConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_Media_VertexDescriptorBuilder* builder = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)builder);
  Arcadia_SizeValue expectedStride = 0;
  for (Arcadia_SizeValue s = 0; s < 3; ++s) {
    for (Arcadia_SizeValue y = 0; y < 4; ++y) {
      Arcadia_Natural8Value offset = (Arcadia_Natural8Value)(16 * s + 4 * y);
      Arcadia_Media_VertexElementDescriptor* element = Arcadia_Media_VertexElementDescriptor_create(thread, offset, semantics[s], syntactics[y]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)element);
      checkElement(thread, element, offset, s, y);
      Arcadia_List_insertBackObjectReferenceValue(thread, builder->vertexElementDescriptors, (Arcadia_Object*)element);
      Arcadia_ValueStack_popValues(thread, 1);

      // Keep the native constructor's Integer32 enum transport and wider offset.
      Arcadia_Natural32Value runtimeOffset = (Arcadia_Natural32Value)(256 + 64 * s + 16 * y);
      Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushNatural32Value(thread, runtimeOffset);
      Arcadia_ValueStack_pushInteger32Value(thread, semantics[s]);
      Arcadia_ValueStack_pushInteger32Value(thread, syntactics[y]);
      Arcadia_ValueStack_pushNatural8Value(thread, 3);
      element = (Arcadia_Media_VertexElementDescriptor*)_Arcadia_EndCreate0(thread, _Arcadia_Media_VertexElementDescriptor_getType(thread), argumentsBase);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)element);
      checkElement(thread, element, runtimeOffset, s, y);
      Arcadia_List_insertBackObjectReferenceValue(thread, builder->vertexElementDescriptors, (Arcadia_Object*)element);
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_SizeValue end = runtimeOffset + (y + 1) * sizeof(Arcadia_Real32Value);
      if (end > expectedStride) expectedStride = end;
    }
  }
  Arcadia_Media_VertexDescriptor* descriptor = Arcadia_Media_VertexDescriptorBuilder_build(thread, builder);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)descriptor);
  // Retain the elements solely through the built descriptor's immutable snapshot.
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)builder->vertexElementDescriptors);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexDescriptor_getStride(thread, descriptor) == expectedStride);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexDescriptor_getVertexSize(thread, descriptor) == expectedStride);
  Arcadia_List* elements = (Arcadia_List*)Arcadia_Media_VertexDescriptor_getVertexElementDescriptors(thread, descriptor);
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements) == 24);
  for (Arcadia_SizeValue s = 0; s < 3; ++s) {
    for (Arcadia_SizeValue y = 0; y < 4; ++y) {
      Arcadia_SizeValue index = 2 * (4 * s + y);
      Arcadia_Media_VertexElementDescriptor* element = (Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, elements, index, _Arcadia_Media_VertexElementDescriptor_getType(thread));
      checkElement(thread, element, (Arcadia_Natural32Value)(16 * s + 4 * y), s, y);
      element = (Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, elements, index + 1, _Arcadia_Media_VertexElementDescriptor_getType(thread));
      checkElement(thread, element, (Arcadia_Natural32Value)(256 + 64 * s + 16 * y), s, y);
    }
  }
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_TypeValue semanticsType = _Arcadia_Media_VertexElementSemantics_getType(thread);
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(semanticsType, semantics[i]));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_TypeValue syntacticsType = _Arcadia_Media_VertexElementSyntactics_getType(thread);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(syntacticsType, syntactics[i]));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, semanticsType != syntacticsType);
  checkType(thread, semanticsType, "Arcadia.Media.VertexElementSemantics");
  checkType(thread, syntacticsType, "Arcadia.Media.VertexElementSyntactics");
  for (Arcadia_SizeValue repetition = 0; repetition < 3; ++repetition) {
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, _Arcadia_Media_VertexElementSemantics_getType(thread) == semanticsType);
    Arcadia_Tests_assertTrue(thread, _Arcadia_Media_VertexElementSyntactics_getType(thread) == syntacticsType);
    checkType(thread, semanticsType, "Arcadia.Media.VertexElementSemantics");
    checkType(thread, syntacticsType, "Arcadia.Media.VertexElementSyntactics");
    for (Arcadia_SizeValue i = 0; i < 7; ++i) {
      Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 6 - i);
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
      Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
      Arcadia_Tests_assertTrue(thread, enumeration.type == (i < 3 ? semanticsType : syntacticsType));
      Arcadia_Tests_assertTrue(thread, enumeration.value == (i < 3 ? semantics[i] : syntactics[i - 3]));
    }
  }
  testConsumers(thread);
  Arcadia_ValueStack_popValues(thread, 7);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, 299 - i) == i);
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

int
Arcadia_Media_Tests_Geometry_vertexEnums
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
