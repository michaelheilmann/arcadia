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

#include "RunModels.h"
#include "Arcadia/Engine/Text/Include.h"
#include "Arcadia/Collections/Include.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct MarkupRunABI {
  Arcadia_Object _parent;
  Arcadia_String* text;
  Arcadia_String* glyphSource;
  Arcadia_String* dynamicToken;
  Arcadia_Natural32Value pixelSize;
  Arcadia_String* colorName;
  Arcadia_Engine_Text_TextAlignment alignment;
  Arcadia_BooleanValue hasRectangle;
  Arcadia_Integer32Value rectangleLeft;
  Arcadia_Integer32Value rectangleBottom;
  Arcadia_Integer32Value rectangleWidth;
  Arcadia_Integer32Value rectangleHeight;
  Arcadia_BooleanValue lineBreakBefore;
  Arcadia_BooleanValue fixedPlacement;
};

typedef Arcadia_Engine_Text_MarkupTextRun*
CreateRun
  (
    Arcadia_Thread* thread,
    Arcadia_String* text,
    Arcadia_String* glyphSource,
    Arcadia_String* dynamicToken,
    Arcadia_Natural32Value pixelSize,
    Arcadia_String* colorName,
    Arcadia_Engine_Text_TextAlignment alignment,
    Arcadia_BooleanValue fixedPlacement
  );

_Static_assert(sizeof(Arcadia_Engine_Text_MarkupTextRun) == sizeof(struct MarkupRunABI), "run size");
_Static_assert(_Alignof(Arcadia_Engine_Text_MarkupTextRun) == _Alignof(struct MarkupRunABI), "run alignment");
_Static_assert(sizeof(Arcadia_Engine_Text_MarkupTextRunDispatch) == sizeof(Arcadia_ObjectDispatch), "run has no own slots");
_Static_assert(_Generic(&Arcadia_Engine_Text_MarkupTextRun_create, CreateRun*: 1, default: 0), "factory signature");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Engine_Text_MarkupTextRun, Name) == offsetof(struct MarkupRunABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Engine_Text_MarkupTextRun*)0)->Name, Type: 1, default: 0), #Name " type");
FIELD(text, Arcadia_String*)
FIELD(glyphSource, Arcadia_String*)
FIELD(dynamicToken, Arcadia_String*)
FIELD(pixelSize, Arcadia_Natural32Value)
FIELD(colorName, Arcadia_String*)
FIELD(alignment, Arcadia_Engine_Text_TextAlignment)
FIELD(hasRectangle, Arcadia_BooleanValue)
FIELD(rectangleLeft, Arcadia_Integer32Value)
FIELD(rectangleBottom, Arcadia_Integer32Value)
FIELD(rectangleWidth, Arcadia_Integer32Value)
FIELD(rectangleHeight, Arcadia_Integer32Value)
FIELD(lineBreakBefore, Arcadia_BooleanValue)
FIELD(fixedPlacement, Arcadia_BooleanValue)
#undef FIELD
_Static_assert(Arcadia_Engine_Text_TextAlignment_Left == 0, "Left value");
_Static_assert(Arcadia_Engine_Text_TextAlignment_Center == 1, "Center value");
_Static_assert(Arcadia_Engine_Text_TextAlignment_Right == 2, "Right value");
_Static_assert(sizeof(Arcadia_Engine_Text_TextAlignment) == sizeof(Arcadia_Integer32Value), "alignment ABI");

typedef struct MarkupRunProbe MarkupRunProbe;
typedef struct MarkupRunProbeDispatch MarkupRunProbeDispatch;

struct MarkupRunProbe {
  Arcadia_Engine_Text_MarkupTextRun _parent;
  Arcadia_String* extra;
};

struct MarkupRunProbeDispatch {
  Arcadia_Engine_Text_MarkupTextRunDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.MarkupRunProbe", MarkupRunProbe,
                          u8"Arcadia.Engine.Text.MarkupTextRun");

static void
constructProbe
  (
    Arcadia_Thread* thread,
    MarkupRunProbe* self
  )
{
  Arcadia_EnterConstructor(MarkupRunProbe);
  if (_numberOfArguments != 7) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value arguments[7];
  for (unsigned i = 0; i < 7; ++i) arguments[i] = Arcadia_ValueStack_getValue(thread, 7 - i);
  for (unsigned i = 0; i < 7; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->extra = Arcadia_String_createFromCxxString(thread, "derived payload");
  Arcadia_LeaveConstructor(MarkupRunProbe);
}

static void
visitProbe
  (
    Arcadia_Thread* thread,
    MarkupRunProbe* self
  )
{
  if (self->extra) Arcadia_Object_visit(thread, (void*)self->extra);
}

static const Arcadia_ObjectType_Operations probeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitProbe,
};

static const Arcadia_Type_Operations probeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &probeObjects,
};

Arcadia_defineObjectType(u8"Tests.MarkupRunProbe", MarkupRunProbe,
                         u8"Arcadia.Engine.Text.MarkupTextRun", Arcadia_Engine_Text_MarkupTextRun,
                         &probeOperations);

static Arcadia_Engine_Text_MarkupTextRun*
createWithArguments
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value const* arguments,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return (void*)_Arcadia_EndCreate0(thread, type, base);
}

static void
testAlignment
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = _Arcadia_Engine_Text_TextAlignment_getType(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  const char expected[] = "Arcadia.Engine.Text.TextAlignment";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == sizeof(expected) - 1);
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expected, sizeof(expected) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_Integer32Value value = 0; value < 3; ++value) {
    Arcadia_Value boxed = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, value));
    Arcadia_ValueStack_pushValue(thread, &boxed);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Value boxed = Arcadia_ValueStack_getValue(thread, 2 - i);
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&boxed);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == (Arcadia_Integer32Value)i);
  }
  Arcadia_Tests_assertTrue(thread, type == _Arcadia_Engine_Text_TextAlignment_getType(thread));
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testFactory
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_String* strings[4];
  const char* names[] = { "text", "glyphs", "token", "Colors.White" };
  for (unsigned i = 0; i < 4; ++i) {
    strings[i] = Arcadia_String_createFromCxxString(thread, names[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, strings[i]);
  }
  Arcadia_Integer32Value alignments[] = { 0, 1, 2, INT32_MIN, INT32_MAX };
  for (unsigned a = 0; a < sizeof(alignments) / sizeof(alignments[0]); ++a) {
    for (unsigned flags = 0; flags < 8; ++flags) {
      Arcadia_String* token = flags & 1 ? strings[2] : NULL;
      Arcadia_Natural32Value pixelSize = flags & 2 ? UINT32_MAX : 0;
      Arcadia_BooleanValue fixedPlacement = (flags & 4) != 0;
      Arcadia_Engine_Text_MarkupTextRun* run = Arcadia_Engine_Text_MarkupTextRun_create(thread,
        strings[0], strings[1], token, pixelSize, strings[3], (Arcadia_Engine_Text_TextAlignment)alignments[a], fixedPlacement);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, run);
      Arcadia_Tests_assertTrue(thread, run->text == strings[0] && run->glyphSource == strings[1] && run->dynamicToken == token && run->colorName == strings[3]);
      Arcadia_Tests_assertTrue(thread, run->pixelSize == pixelSize && (Arcadia_Integer32Value)run->alignment == alignments[a] && run->fixedPlacement == fixedPlacement);
      Arcadia_Tests_assertTrue(thread, !run->hasRectangle && !run->lineBreakBefore);
      Arcadia_Tests_assertTrue(thread, !run->rectangleLeft && !run->rectangleBottom && !run->rectangleWidth && !run->rectangleHeight);
      run->hasRectangle = true;
      run->rectangleLeft = INT32_MIN; run->rectangleBottom = INT32_MAX;
      run->rectangleWidth = -23; run->rectangleHeight = 17;
      run->lineBreakBefore = true;
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, run->rectangleLeft == INT32_MIN && run->rectangleBottom == INT32_MAX && run->rectangleWidth == -23 && run->rectangleHeight == 17 && run->hasRectangle && run->lineBreakBefore);
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 304);
    }
  }
  // Required strings may be empty and may alias each other or the token.
  Arcadia_String* empty = Arcadia_String_createFromCxxString(thread, "");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, empty);
  Arcadia_Engine_Text_MarkupTextRun* run = Arcadia_Engine_Text_MarkupTextRun_create(thread, empty, empty, empty, 0, empty, Arcadia_Engine_Text_TextAlignment_Left, false);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, run);
  Arcadia_Tests_assertTrue(thread, run->text == empty && run->glyphSource == empty && run->dynamicToken == empty && run->colorName == empty);
  Arcadia_ValueStack_popValues(thread, 306);
}

static Arcadia_SizeValue destroyed[6];

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{
  (*(Arcadia_SizeValue*)observer)++;
}

static void
testLifetime
  (
    Arcadia_Thread* thread
  )
{
  for (unsigned i = 0; i < 6; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_String* strings[4];
  const char* names[] = { "text", "glyphs", "token", "Colors.White" };
  for (unsigned i = 0; i < 4; ++i) {
    strings[i] = Arcadia_String_createFromCxxString(thread, names[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, strings[i]);
  }
  Arcadia_Value arguments[] = {
    Arcadia_Value_makeObjectReferenceValue(strings[0]), Arcadia_Value_makeObjectReferenceValue(strings[1]),
    Arcadia_Value_makeObjectReferenceValue(strings[2]), Arcadia_Value_makeNatural32Value(31),
    Arcadia_Value_makeObjectReferenceValue(strings[3]), Arcadia_Value_makeInteger32Value(2), Arcadia_Value_makeBooleanValue(true),
  };
  MarkupRunProbe* probe = (void*)createWithArguments(thread, _MarkupRunProbe_getType(thread), arguments, 7);
  Arcadia_Engine_Text_MarkupTextRun* run = (void*)probe;
  Arcadia_Object* objects[] = { (void*)run, (void*)strings[0], (void*)strings[1], (void*)strings[2], (void*)strings[3], (void*)probe->extra };
  for (unsigned i = 0; i < 6; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(run);
  Arcadia_ValueStack_setValue(thread, 3, &value);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_String* retained[] = { run->text, run->glyphSource, run->dynamicToken, run->colorName };
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, retained[i], names[i], strlen(names[i])));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, probe->extra, "derived payload", sizeof("derived payload") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_String* text = Arcadia_String_createFromCxxString(thread, "text");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, text);
  Arcadia_ArrayList* wrong = Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  Arcadia_Value arguments[] = {
    Arcadia_Value_makeObjectReferenceValue(text), Arcadia_Value_makeObjectReferenceValue(text),
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeNatural32Value(17),
    Arcadia_Value_makeObjectReferenceValue(text), Arcadia_Value_makeInteger32Value(1), Arcadia_Value_makeBooleanValue(false),
  };
  Arcadia_Value invalid[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeInteger32Value(0),
    Arcadia_Value_makeNatural32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal64Value(0.),
    Arcadia_Value_makeObjectReferenceValue(wrong),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Text_MarkupWordType_getType(thread), 1)),
  };
  for (unsigned parameter = 0; parameter < 7; ++parameter) {
    Arcadia_Value saved = arguments[parameter];
    for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
      if ((parameter == 2 && Arcadia_Value_isVoidValue(&invalid[i])) ||
          (parameter == 3 && Arcadia_Value_isNatural32Value(&invalid[i])) ||
          (parameter == 5 && Arcadia_Value_isInteger32Value(&invalid[i])) ||
          (parameter == 6 && Arcadia_Value_isBooleanValue(&invalid[i]))) continue;
      arguments[parameter] = invalid[i];
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        createWithArguments(thread, _Arcadia_Engine_Text_MarkupTextRun_getType(thread), arguments, 7);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
    arguments[parameter] = saved;
  }
  for (unsigned count = 0; count <= 255; ++count) {
    if (count == 7) continue;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Engine_Text_MarkupTextRun_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 302);
}

static void
testTypedEnumCallbacks
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_String* text = Arcadia_String_createFromCxxString(thread, "text");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, text);
  Arcadia_TypeValue type = _Arcadia_Engine_Text_TextAlignment_getType(thread);
  Arcadia_Integer32Value alignments[] = { 0, 1, 2, INT32_MIN, INT32_MAX };
  for (unsigned derived = 0; derived < 2; ++derived) {
    for (unsigned representation = 0; representation < 2; ++representation) {
      for (unsigned i = 0; i < sizeof(alignments) / sizeof(alignments[0]); ++i) {
        Arcadia_Value arguments[] = {
          Arcadia_Value_makeObjectReferenceValue(text), Arcadia_Value_makeObjectReferenceValue(text),
          Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeNatural32Value(UINT32_MAX),
          Arcadia_Value_makeObjectReferenceValue(text),
          representation ? Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, alignments[i])) : Arcadia_Value_makeInteger32Value(alignments[i]),
          Arcadia_Value_makeBooleanValue(true),
        };
        Arcadia_Engine_Text_MarkupTextRun* run = createWithArguments(thread,
          derived ? _MarkupRunProbe_getType(thread) : _Arcadia_Engine_Text_MarkupTextRun_getType(thread), arguments, 7);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, run);
        Arcadia_Tests_assertTrue(thread, (Arcadia_Integer32Value)run->alignment == alignments[i] && run->pixelSize == UINT32_MAX && run->fixedPlacement && !run->dynamicToken);
        Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
        Arcadia_ValueStack_popValues(thread, 1);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
      }
    }
  }
  // C NULLs at the three required string parameters must remain rejected.
  for (unsigned parameter = 0; parameter < 3; ++parameter) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_Text_MarkupTextRun_create(thread, parameter == 0 ? NULL : text,
        parameter == 1 ? NULL : text, NULL, 0, parameter == 2 ? NULL : text,
        Arcadia_Engine_Text_TextAlignment_Left, false);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 301);
}

int
Arcadia_Engine_Text_Tests_runModels
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testAlignment)) { fprintf(stderr, "TextAlignment model failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testFactory)) { fprintf(stderr, "MarkupTextRun factory failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testLifetime)) { fprintf(stderr, "MarkupTextRun lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testErrors)) { fprintf(stderr, "MarkupTextRun errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testTypedEnumCallbacks)) { fprintf(stderr, "MarkupTextRun typed enum callbacks failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
