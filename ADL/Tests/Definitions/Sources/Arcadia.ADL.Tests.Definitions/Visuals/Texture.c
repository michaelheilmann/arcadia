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

#include "Arcadia.ADL.Tests.Definitions/Visuals/Texture.h"

#include "Arcadia/ADL/Include.h"

#include <stdio.h>
#include <string.h>

#if defined(ARCADIA_ADL_PRIVATE)
  #error("ADL's public aggregate must not leak its private include switch")
#endif

_Static_assert(Arcadia_ADL_TextureFilter_None == 10, "None filter changed");
_Static_assert(Arcadia_ADL_TextureFilter_Linear == 0, "Linear filter changed");
_Static_assert(Arcadia_ADL_TextureFilter_Nearest == 1, "Nearest filter changed");
_Static_assert(Arcadia_ADL_TextureAddressMode_None == 10, "None address mode changed");
_Static_assert(Arcadia_ADL_TextureAddressMode_ClampToBorder == 0, "ClampToBorder changed");
_Static_assert(Arcadia_ADL_TextureAddressMode_ClampToEdge == 1, "ClampToEdge changed");
_Static_assert(Arcadia_ADL_TextureAddressMode_MirroredRepeat == 2, "MirroredRepeat changed");
_Static_assert(Arcadia_ADL_TextureAddressMode_Repeat == 3, "Repeat changed");
_Static_assert(sizeof(Arcadia_ADL_TextureFilter) == sizeof(Arcadia_Integer32Value), "Filter ABI changed");
_Static_assert(sizeof(Arcadia_ADL_TextureAddressMode) == sizeof(Arcadia_Integer32Value), "Address mode ABI changed");
_Static_assert(_Generic(&_Arcadia_ADL_TextureFilter_getType,
  Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "Filter descriptor signature changed");
_Static_assert(_Generic(&_Arcadia_ADL_TextureAddressMode_getType,
  Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "Address mode descriptor signature changed");

static void
checkEnumeration
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    const char* expectedName,
    const Arcadia_Integer32Value* values,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == strlen(expectedName));
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expectedName, strlen(expectedName)));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, values[i]));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, count - 1 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == values[i]);
  }
  Arcadia_ValueStack_popValues(thread, count);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

void
Arcadia_ADL_Tests_Visuals_TextureEnums
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) {
    Arcadia_ValueStack_pushNatural64Value(thread, i);
  }
  const Arcadia_Integer32Value filters[] = { 10, 0, 1 };
  const Arcadia_Integer32Value modes[] = { 10, 0, 1, 2, 3 };
  const char* filterEntries[] = { "minificationFilter: \"Linear\",",
    "magnificationFilter: \"Linear\", minificationFilter: \"Nearest\",",
    "magnificationFilter: \"Nearest\"," };
  const char* modeEntries[] = { "addressModeV: \"ClampToBorder\",",
    "addressModeU: \"ClampToBorder\", addressModeV: \"ClampToEdge\",",
    "addressModeU: \"ClampToEdge\", addressModeV: \"MirroredRepeat\",",
    "addressModeU: \"MirroredRepeat\", addressModeV: \"Repeat\",",
    "addressModeU: \"Repeat\"," };
  checkEnumeration(thread, _Arcadia_ADL_TextureFilter_getType(thread), "Arcadia.ADL.TextureFilter", filters, 3);
  checkEnumeration(thread, _Arcadia_ADL_TextureAddressMode_getType(thread), "Arcadia.ADL.TextureAddressMode", modes, 5);
  Arcadia_Tests_assertTrue(thread, _Arcadia_ADL_TextureFilter_getType(thread) != _Arcadia_ADL_TextureAddressMode_getType(thread));

  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  for (Arcadia_SizeValue f = 0; f < 3; ++f) {
    for (Arcadia_SizeValue m = 0; m < 5; ++m) {
      Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
      char input[512];
      int length = snprintf(input, sizeof(input), "{type: \"Texture\", name: \"Test.Texture\", pixelBuffer: \"Unresolved\", %s %s}", filterEntries[f], modeEntries[m]);
      Arcadia_Tests_assertTrue(thread, length > 0 && (size_t)length < sizeof(input));
      // Deliberately do not link: enum parsing must not require the pixel buffer.
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, input), Arcadia_BooleanValue_False);
      Arcadia_ADL_TextureDefinition* texture = (Arcadia_ADL_TextureDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "Test.Texture"));
      Arcadia_Tests_assertTrue(thread, texture != NULL);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, texture->magnificationFilter == filters[f] && texture->minificationFilter == filters[(f + 1) % 3]);
      Arcadia_Tests_assertTrue(thread, texture->addressModeU == modes[m] && texture->addressModeV == modes[(m + 1) % 5]);
      Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  const char* properties[] = { "magnificationFilter", "minificationFilter", "addressModeU", "addressModeV" };
  const char* invalidValues[] = { "Invalid", "None" };
  // None is a sentinel for an omitted property, not an accepted ADL string.
  for (Arcadia_SizeValue p = 0; p < 4; ++p) {
    for (Arcadia_SizeValue v = 0; v < 2; ++v) {
      Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      char input[512];
      int length = snprintf(input, sizeof(input), "{type: \"Texture\", name: \"Test.Texture\", pixelBuffer: \"Unresolved\", %s: \"%s\"}", properties[p], invalidValues[v]);
      Arcadia_Tests_assertTrue(thread, length > 0 && (size_t)length < sizeof(input));
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, input), Arcadia_BooleanValue_False);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Status status = Arcadia_Thread_getStatus(thread);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
        Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_SemanticalError);
      }
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) >= depth);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - depth + 1);
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, 299 - i) == i);
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == entry);
}

void
Arcadia_ADL_Tests_Visuals_TextureDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
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
  Arcadia_ADL_Definitions_link(thread, definitions);
}

void
Arcadia_ADL_Tests_Visuals_TextureDefinitionTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
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
      "  magnificationFilter : \"Nearest\",\n"
      "  minificationFilter : \"Nearest\",\n"
      "  addressModeU : \"ClampToEdge\",\n"
      "  addressModeV : \"MirroredRepeat\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"MyGame.Textures.MyTexture"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_TextureDefinition* textureDefinition = (Arcadia_ADL_TextureDefinition*)definition;
  if (Arcadia_ADL_TextureFilter_Nearest != textureDefinition->magnificationFilter) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureFilter_Nearest != textureDefinition->minificationFilter) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureAddressMode_ClampToEdge != textureDefinition->addressModeU) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureAddressMode_MirroredRepeat != textureDefinition->addressModeV) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_TextureDefinitionTest3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
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
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"MyGame.Textures.MyTexture"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_TextureDefinition* textureDefinition = (Arcadia_ADL_TextureDefinition*)definition;
  if (Arcadia_ADL_TextureFilter_None != textureDefinition->magnificationFilter) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureFilter_None != textureDefinition->minificationFilter) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureAddressMode_None != textureDefinition->addressModeU) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ADL_TextureAddressMode_None != textureDefinition->addressModeV) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_TextureDefinitionTest4
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
        "  magnificationFilter : \"InvalidFilter\",\n"
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
