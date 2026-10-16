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

#include "Arcadia.ADL.Tests.Definitions/Visuals/Material.h"

#include "Arcadia/ADL/Include.h"

#include <stdio.h>
#include <string.h>

_Static_assert(Arcadia_ADL_MaterialType_BlinnPhong == 0, "BlinnPhong value");
_Static_assert(Arcadia_ADL_AmbientColorSource_Mesh == 0, "Mesh value");
_Static_assert(Arcadia_ADL_AmbientColorSource_Vertex == 1, "Vertex value");
_Static_assert(Arcadia_ADL_AmbientColorSource_Texture == 2, "Texture value");
_Static_assert(Arcadia_ADL_AmbientColorSource_TextureVertex == 3, "TextureVertex value");
_Static_assert(Arcadia_ADL_BlendFunction_None == 10, "None value");
_Static_assert(Arcadia_ADL_BlendFunction_Zero == 0, "Zero value");
_Static_assert(Arcadia_ADL_BlendFunction_One == 1, "One value");
_Static_assert(Arcadia_ADL_BlendFunction_SourceColor == 2, "SourceColor value");
_Static_assert(Arcadia_ADL_BlendFunction_OneMinusSourceColor == 3, "OneMinusSourceColor value");
_Static_assert(Arcadia_ADL_BlendFunction_DestinationColor == 4, "DestinationColor value");
_Static_assert(Arcadia_ADL_BlendFunction_OneMinusDestinationColor == 5, "OneMinusDestinationColor value");
_Static_assert(Arcadia_ADL_BlendFunction_SourceAlpha == 6, "SourceAlpha value");
_Static_assert(Arcadia_ADL_BlendFunction_OneMinusSourceAlpha == 7, "OneMinusSourceAlpha value");
_Static_assert(Arcadia_ADL_BlendFunction_DestinationAlpha == 8, "DestinationAlpha value");
_Static_assert(Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha == 9, "OneMinusDestinationAlpha value");

#define CHECK_ENUM_ABI(Type) \
  _Static_assert(sizeof(Type) == sizeof(Arcadia_Integer32Value), #Type " size"); \
  _Static_assert(_Alignof(Type) == _Alignof(Arcadia_Integer32Value), #Type " alignment"); \
  _Static_assert(_Generic(&_##Type##_getType, Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), #Type " descriptor signature");
CHECK_ENUM_ABI(Arcadia_ADL_MaterialType)
CHECK_ENUM_ABI(Arcadia_ADL_AmbientColorSource)
CHECK_ENUM_ABI(Arcadia_ADL_BlendFunction)
#undef CHECK_ENUM_ABI

_Static_assert(_Generic(&Arcadia_ADL_MaterialDefinition_create,
  Arcadia_ADL_MaterialDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*,
    Arcadia_ADL_MaterialType, Arcadia_ADL_AmbientColorSource, Arcadia_String*,
    Arcadia_ADL_BlendFunction, Arcadia_ADL_BlendFunction): 1, default: 0), "native material factory signature");

static const Arcadia_ADL_BlendFunction blendValues[] = {
  Arcadia_ADL_BlendFunction_None, Arcadia_ADL_BlendFunction_Zero,
  Arcadia_ADL_BlendFunction_One, Arcadia_ADL_BlendFunction_SourceColor,
  Arcadia_ADL_BlendFunction_OneMinusSourceColor, Arcadia_ADL_BlendFunction_DestinationColor,
  Arcadia_ADL_BlendFunction_OneMinusDestinationColor, Arcadia_ADL_BlendFunction_SourceAlpha,
  Arcadia_ADL_BlendFunction_OneMinusSourceAlpha, Arcadia_ADL_BlendFunction_DestinationAlpha,
  Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha,
};

static const Arcadia_ADL_AmbientColorSource ambientValues[] = {
  Arcadia_ADL_AmbientColorSource_Mesh, Arcadia_ADL_AmbientColorSource_Vertex,
  Arcadia_ADL_AmbientColorSource_Texture, Arcadia_ADL_AmbientColorSource_TextureVertex,
};

static void
checkMaterialEnum
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue (*getType)(Arcadia_Thread*),
    const char* expectedName,
    const Arcadia_Integer32Value* values,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = getType(thread);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == strlen(expectedName));
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expectedName, strlen(expectedName)));
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, values[i]));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, getType(thread) == type && Arcadia_getType(thread, name) == type);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, count - 1 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == values[i]);
  }
  Arcadia_ValueStack_popValues(thread, count);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

static Arcadia_ADL_MaterialDefinition*
readMaterialEnums
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    const char* materialType,
    const char* ambientSource,
    const char* blendSource,
    const char* blendDestination
  )
{
  char source[128] = "", destination[128] = "", input[768];
  if (blendSource) snprintf(source, sizeof(source), "blendSourceFunction: \"%s\",", blendSource);
  if (blendDestination) snprintf(destination, sizeof(destination), "blendDestinationFunction: \"%s\",", blendDestination);
  int length = snprintf(input, sizeof(input), "{type: \"Material\", name: \"Test.Material\", materialType: \"%s\", ambientColorSource: \"%s\", ambientColorTexture: \"Unresolved.Texture\", %s %s}", materialType, ambientSource, source, destination);
  Arcadia_Tests_assertTrue(thread, length > 0 && (size_t)length < sizeof(input));
  // Enum parsing is independent of reference resolution; deliberately do not link.
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, input), Arcadia_BooleanValue_False);
  Arcadia_ADL_Definition* result = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "Test.Material"));
  Arcadia_Tests_assertTrue(thread, result && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)result, _Arcadia_ADL_MaterialDefinition_getType(thread)));
  return (Arcadia_ADL_MaterialDefinition*)result;
}

static void
checkMaterialFields
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MaterialDefinition* material,
    Arcadia_SizeValue ambient,
    Arcadia_SizeValue source,
    Arcadia_SizeValue destination
  )
{
  Arcadia_Tests_assertTrue(thread, material->materialType == Arcadia_ADL_MaterialType_BlinnPhong);
  Arcadia_Tests_assertTrue(thread, material->ambientColorSource == ambientValues[ambient]);
  Arcadia_Tests_assertTrue(thread, material->blendSourceFunction == blendValues[source]);
  Arcadia_Tests_assertTrue(thread, material->blendDestinationFunction == blendValues[destination]);
}

static void
testMaterialEnumConsumers
  (
    Arcadia_Thread* thread
  )
{
  const char* const ambientNames[] = { "Mesh", "Vertex", "Texture", "TextureVertex" };
  const char* const blendNames[] = { NULL, "Zero", "One", "SourceColor", "OneMinusSourceColor",
    "DestinationColor", "OneMinusDestinationColor", "SourceAlpha", "OneMinusSourceAlpha",
    "DestinationAlpha", "OneMinusDestinationAlpha" };
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Test.DirectMaterial");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, "Unresolved.Texture");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)textureName);
  for (Arcadia_SizeValue a = 0; a < 4; ++a) {
    for (Arcadia_SizeValue b = 0; b < 11; ++b) {
      Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
      Arcadia_SizeValue destination = (b + 1) % 11;
      Arcadia_ADL_MaterialDefinition* parsed = readMaterialEnums(thread, context, definitions, "Blinn-Phong", ambientNames[a], blendNames[b], blendNames[destination]);
      checkMaterialFields(thread, parsed, a, b, destination);
      Arcadia_ADL_MaterialDefinition* direct = Arcadia_ADL_MaterialDefinition_create(thread, definitions, name, Arcadia_ADL_MaterialType_BlinnPhong, ambientValues[a], textureName, blendValues[b], blendValues[destination]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)direct);
      checkMaterialFields(thread, direct, a, b, destination);
      Arcadia_ValueStack_popValues(thread, 2);
    }
  }
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  checkMaterialFields(thread, readMaterialEnums(thread, context, definitions, "Blinn-Phong", "Mesh", NULL, NULL), 0, 0, 0);
  const char* const invalid[][4] = {
    { "Invalid", "Mesh", NULL, NULL }, { "BlinnPhong", "Mesh", NULL, NULL },
    { "Blinn-Phong", "Invalid", NULL, NULL }, { "Blinn-Phong", "texturevertex", NULL, NULL },
    { "Blinn-Phong", "Mesh", "Invalid", NULL }, { "Blinn-Phong", "Mesh", NULL, "Invalid" },
    { "Blinn-Phong", "Mesh", "None", NULL }, { "Blinn-Phong", "Mesh", NULL, "None" },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      readMaterialEnums(thread, context, definitions, invalid[i][0], invalid[i][1], invalid[i][2], invalid[i][3]);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_SemanticalError);
    }
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) >= depth);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - depth);
  }
  const Arcadia_TypeValue types[] = { _Arcadia_ADL_MaterialType_getType(thread), _Arcadia_ADL_AmbientColorSource_getType(thread), _Arcadia_ADL_BlendFunction_getType(thread), _Arcadia_ADL_BlendFunction_getType(thread) };
  for (Arcadia_SizeValue bad = 0; bad < 8; ++bad) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value enums[4];
    for (Arcadia_SizeValue i = 0; i < 4; ++i) enums[i] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(types[i], 0));
    // Numeric zero is valid in every enum; descriptor identity must still matter.
    enums[bad % 4] = bad < 4 ? Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(types[bad % 4] == types[0] ? types[1] : types[0], 0)) : Arcadia_Value_makeInteger32Value(0);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
      Arcadia_ValueStack_pushValue(thread, &enums[0]);
      Arcadia_ValueStack_pushValue(thread, &enums[1]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)textureName);
      Arcadia_ValueStack_pushValue(thread, &enums[2]);
      Arcadia_ValueStack_pushValue(thread, &enums[3]);
      Arcadia_ValueStack_pushNatural8Value(thread, 7);
      _Arcadia_EndCreate0(thread, _Arcadia_ADL_MaterialDefinition_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_ValueStack_popValues(thread, 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

void
Arcadia_ADL_Tests_Visuals_MaterialEnums
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  const Arcadia_Integer32Value material[] = { 0 }, ambient[] = { 0, 1, 2, 3 }, blend[] = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  checkMaterialEnum(thread, _Arcadia_ADL_MaterialType_getType, "Arcadia.ADL.MaterialType", material, 1);
  checkMaterialEnum(thread, _Arcadia_ADL_AmbientColorSource_getType, "Arcadia.ADL.AmbientColorSource", ambient, 4);
  checkMaterialEnum(thread, _Arcadia_ADL_BlendFunction_getType, "Arcadia.ADL.BlendFunction", blend, 11);
  Arcadia_Tests_assertTrue(thread, _Arcadia_ADL_MaterialType_getType(thread) != _Arcadia_ADL_AmbientColorSource_getType(thread));
  Arcadia_Tests_assertTrue(thread, _Arcadia_ADL_MaterialType_getType(thread) != _Arcadia_ADL_BlendFunction_getType(thread));
  Arcadia_Tests_assertTrue(thread, _Arcadia_ADL_AmbientColorSource_getType(thread) != _Arcadia_ADL_BlendFunction_getType(thread));
  // These scalar/reader checks are separate from the material lifetime regression.
  testMaterialEnumConsumers(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, 299 - i) == i);
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

static void
makeTexture
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions
  )
{
  {
    static const char* DDL =
      "{\n"
      "  type : \"Color\",\n"
      "  name : \"MyGame.Colors.MyBlack\",\n"
      "  red : 0,\n"
      "  green : 0,\n"
      "  blue : 0,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Color\",\n"
      "  name : \"MyGame.Colors.MyWhite\",\n"
      "  red : 255,\n"
      "  green : 255,\n"
      "  blue : 255,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"PixelBufferOperations.CheckerboardFillOperation\",\n"
      "  name  : \"MyGame.PixelBufferOperations.MyPixelBufferFillOperation\",\n"
      "  checkerWidth : 32,\n"
      "  checkerHeight : 32,\n"
      "  firstCheckerColor : \"MyGame.Colors.MyWhite\",\n"
      "  secondCheckerColor : \"MyGame.Colors.MyBlack\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"PixelBuffer\",\n"
      "  name  : \"MyGame.PixelBuffers.MyPixelBuffer\",\n"
      "  width: 256,\n"
      "  height: 256,\n"
      "  operations : [\n"
      "    \"MyGame.PixelBufferOperations.MyPixelBufferFillOperation\",\n"
      "  ],\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"Texture\",\n"
      "  name  : \"MyGame.Textures.MyTexture\",\n"
      "  pixelBuffer : \"MyGame.PixelBuffers.MyPixelBuffer\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  {
    makeTexture(thread, context, definitions);
  }
  {
    static const char *DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Mesh\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  {
    makeTexture(thread, context, definitions);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"MyGame.Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Vertex\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_MaterialDefinition* material = (Arcadia_ADL_MaterialDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(
    thread, definitions, Arcadia_String_createFromCxxString(thread, "MyGame.Materials.MyMaterial"));
  Arcadia_Tests_assertTrue(thread, material != NULL);
  // Drop fixture roots: only the material should retain its reference and graph.
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)material);
  Arcadia_WeakReference* reference = Arcadia_WeakReference_create(thread,
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)material->ambientColorTexture));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)reference);
  for (unsigned i = 0; i < 3; ++i) {
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Value retained = Arcadia_WeakReference_getValue(thread, reference);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retained));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&retained) == (Arcadia_Object*)material->ambientColorTexture);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread,
      Arcadia_ADL_Reference_getDefinitionName(thread, material->ambientColorTexture),
      u8"MyGame.Textures.MyTexture", sizeof(u8"MyGame.Textures.MyTexture") - 1));
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)material);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread,
      (Arcadia_Object*)material->ambientColorTexture->definition, _Arcadia_ADL_TextureDefinition_getType(thread)));
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  // Keeping the weak observer alone must not retain the material/reference cycle.
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)reference);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Value released = Arcadia_WeakReference_getValue(thread, reference);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&released));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  {
    makeTexture(thread, context, definitions);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"MyGame.Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Texture\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest4
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  {
    makeTexture(thread, context, definitions);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"MyGame.Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Texture\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "  blendSourceFunction : \"SourceAlpha\",\n"
      "  blendDestinationFunction : \"OneMinusSourceAlpha\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"MyGame.Materials.MyMaterial"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_MaterialDefinition* materialDefinition = (Arcadia_ADL_MaterialDefinition*)definition;
  if (Arcadia_ADL_BlendFunction_SourceAlpha != materialDefinition->blendSourceFunction) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_BlendFunction_OneMinusSourceAlpha != materialDefinition->blendDestinationFunction) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest5
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  {
    makeTexture(thread, context, definitions);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"MyGame.Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Texture\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"MyGame.Materials.MyMaterial"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_MaterialDefinition* materialDefinition = (Arcadia_ADL_MaterialDefinition*)definition;
  if (Arcadia_ADL_BlendFunction_None != materialDefinition->blendSourceFunction) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_BlendFunction_None != materialDefinition->blendDestinationFunction) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest6
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    {
      makeTexture(thread, context, definitions);
    }
    {
      static const char* DDL =
        "{\n"
        "  type : \"Material\",\n"
        "  name : \"MyGame.Materials.MyMaterial\",\n"
        "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
        "  ambientColorSource : \"Texture\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
        "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
        "  blendSourceFunction : \"InvalidFunction\",\n"
        "}\n"
        ;
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}
