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

#include "Arcadia/Engine/Tests/VisualResources/Cases.h"
#include "Arcadia/Engine/Include.h"
#include <string.h>

_Static_assert(Arcadia_Engine_Visuals_ColorSource_Mesh == 0, "Mesh C API value changed");
_Static_assert(Arcadia_Engine_Visuals_ColorSource_Vertex == 1, "Vertex C API value changed");
_Static_assert(Arcadia_Engine_Visuals_ColorSource_Texture == 2, "Texture C API value changed");
_Static_assert(sizeof(Arcadia_Engine_Visuals_ColorSource) == sizeof(Arcadia_Integer32Value), "ColorSource C ABI changed");
_Static_assert(_Generic(&_Arcadia_Engine_Visuals_ColorSource_getType,
  Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "ColorSource descriptor signature changed");

typedef Arcadia_Engine_Visuals_MaterialResource*
CreateMaterialFunction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_BooleanValue blendEnabled,
    Arcadia_Engine_Visuals_BlendFunction source,
    Arcadia_Engine_Visuals_BlendFunction destination,
    Arcadia_Engine_Visuals_ColorSource colorSource,
    Arcadia_Engine_Visuals_TextureResource* texture,
    Arcadia_Engine_Visuals_ProgramResource* program
  );

_Static_assert(_Generic(&Arcadia_Engine_Visuals_BackendContext_createMaterialResource,
  CreateMaterialFunction*: 1, default: 0), "Material API must use ColorSource");
_Static_assert(_Generic(((Arcadia_Engine_Visuals_BackendContextDispatch*)0)->createMaterialResource,
  CreateMaterialFunction*: 1, default: 0), "Material dispatch must use ColorSource");

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Engine_Visuals_ColorSource values[] = {
    Arcadia_Engine_Visuals_ColorSource_Mesh,
    Arcadia_Engine_Visuals_ColorSource_Vertex,
    Arcadia_Engine_Visuals_ColorSource_Texture,
  };
  const Arcadia_SizeValue count = sizeof(values) / sizeof(values[0]);
  Arcadia_TypeValue type = _Arcadia_Engine_Visuals_ColorSource_getType(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  const char expected[] = "Arcadia.Engine.Visuals.ColorSource";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == sizeof(expected) - 1);
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expected, sizeof(expected) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, values[i]));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, _Arcadia_Engine_Visuals_ColorSource_getType(thread) == type && Arcadia_Type_getName(thread, type) == name);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, count - 1 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == values[i]);
  }
  Arcadia_ValueStack_popValues(thread, count);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_VisualResources_colorSource
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
