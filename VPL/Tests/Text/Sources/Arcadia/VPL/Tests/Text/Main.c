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

#include "Arcadia/Testing/Include.h"

#include "Arcadia/VPL/Include.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

static const char minimal[] =
  "(module 1\n"
  " (types (type Z void) (type V vector float 4))\n"
  " (uniforms)\n"
  " (vertex start (interface (out pos \"position\" V -1 position))\n"
  "  (functions (function start \"vertex_main\" Z (parameters) (registers (register r42 V))\n"
  "   (block entry (construct r42 (float 1.0)) (store (stage pos) r42) (return)))))\n"
  " (fragment start (interface (out color \"color\" V 0 fragment_color))\n"
  "  (functions (function start \"fragment_main\" Z (parameters) (registers (register r42 V))\n"
  "   (block entry (construct r42 (float 1.0)) (store (stage color) r42) (return))))))\n";

static const char complete[] =
  "// Forward type, function, and block references; one operation per form.\n"
  "(module 1\n"
  " (types (type A array R 2) (type R record \"Light\" (field \"value\" V))\n"
  "  (type V vector float 4) (type V3 vector float 3) (type V2 vector float 2)\n"
  "  (type M matrix 4 4) (type S sampler2D) (type I int) (type F float) (type B bool) (type Z void))\n"
  " (uniforms (array lights \"lights\" R 2) (scalar sampler \"sampler\" S) (record material \"material\" R) (scalar scalar \"scalar\" F))\n"
  " (vertex main (interface (out pos \"position\" V -1 position) (out color \"vertex_color\" V 1 color))\n"
  "  (functions (function main \"vertex\\u005fmain\" Z (parameters) (registers (register r0 V) (register r1 F))\n"
  "   (block entry (construct r0 (float 1.0)) (call r1 helper (float 2.0))\n"
  "    (store (stage pos) r0) (store (stage color) r0) (return)))\n"
  "   (function helper 'helper' F (parameters (parameter input F)) (registers (register r0 F))\n"
  "    (block entry (load r0 (parameter input)) (add r0 r0 (float 1)) (return r0)))))\n"
  " (fragment main (interface (in color \"fragment_color\" V 1 color) (out pixel \"pixel\" V 0 fragment_color))\n"
  "  (functions (function main \"fragment_main\" Z (parameters)\n"
  "   (registers (register r0 V) (register r1 I) (register r2 B) (register r3 R) (register r4 A)\n"
  "    (register r5 V) (register r6 V3) (register r7 S) (register r8 V2) (register r9 M) (register r10 F))\n"
  "   (block entry (call r1 count (int 0)) (load r4 (uniform lights)) (index r3 r4 (int 1))\n"
  "    (member r5 r3 \"value\") (swizzle r6 r5 \"xyz\") (construct r0 r6 (float 1.0))\n"
  "    (construct r3 r0) (construct r4 r3 r3)\n"
  "    (store (member_ref (index_ref (local r4) (int 0)) \"value\") r0)\n"
  "    (load r5 (member_ref (index_ref (local r4) (int 0)) \"value\"))\n"
  "    (load r7 (uniform sampler)) (construct r8 (float .0)) (texture r0 r7 r8)\n"
  "    (construct r9 (float 1e0)) (multiply r0 r9 r0) (construct r10 r1)\n"
  "    (subtract r10 r10 (float 1)) (divide r10 r10 (float 2)) (copy r1 (int 0))\n"
  "    (store (stage pixel) r0) (branch test))\n"
  "   (block test (less r2 r1 (int 2)) (branch_if r2 loop done))\n"
  "   (block loop (add r1 r1 (int 1)) (branch test))\n"
  "   (block done (equal r2 r1 (int 2)) (not_equal r2 r1 (int 0))\n"
  "    (less_equal r2 r1 (int 2)) (greater r2 r1 (int 0)) (greater_equal r2 r1 (int 0))\n"
  "    (call void sink) (branch_if (bool false) kill exit))\n"
  "   (block kill (discard)) (block exit (return)))\n"
  "   (function count \"count\" I (parameters (parameter input I)) (registers (register r0 I))\n"
  "    (block entry (load r0 (parameter input)) (return r0)))\n"
  "   (function sink \"sink\" Z (parameters) (registers) (block entry (return))))))\n";

static Arcadia_Object*
root
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  return object;
}

static Arcadia_Languages_Diagnostics*
diagnostics
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  return Arcadia_VPL_Context_getDiagnostics(thread, context);
}

static Arcadia_VPL_IR_Module*
read
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue n,
    Arcadia_Languages_Diagnostics* d
  )
{
  Arcadia_String* name = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, "fixture.vpl"));
  return (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_read(thread, bytes, n, d, name));
}

static Arcadia_String*
replaceSpan
  (
    Arcadia_Thread* thread,
    const void* sourceBytes,
    Arcadia_SizeValue sourceLength,
    const char* find,
    const void* replacement,
    Arcadia_SizeValue replacementLength
  )
{
  const char* source = sourceBytes;
  Arcadia_SizeValue findLength = strlen(find), offset = 0;
  while (offset <= sourceLength && (findLength > sourceLength - offset || memcmp(source + offset, find, findLength))) ++offset;
  Arcadia_Tests_assertTrue(thread, offset <= sourceLength);
  Arcadia_StringBuilder* builder = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
  const void* pieces[] = { source, replacement, source + offset + findLength };
  Arcadia_SizeValue lengths[] = { offset, replacementLength, sourceLength - offset - findLength };
  for (int i = 0; i < 3; ++i) {
    Arcadia_Value v = Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, pieces[i], lengths[i]));
    Arcadia_ValueStack_pushValue(thread, &v);
    Arcadia_StringBuilder_insertBack(thread, builder, v);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  return (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder)));
}

static Arcadia_String*
replaceBytes
  (
    Arcadia_Thread* thread,
    const char* source,
    const char* find,
    const void* replacement,
    Arcadia_SizeValue replacementLength
  )
{
  return replaceSpan(thread, source, strlen(source), find, replacement, replacementLength);
}

static Arcadia_String*
replaceSource
  (
    Arcadia_Thread* thread,
    Arcadia_String* source,
    const char* find,
    const char* replacement
  )
{
  return replaceSpan(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), find, replacement, strlen(replacement));
}

static Arcadia_String*
replace
  (
    Arcadia_Thread* thread,
    const char* source,
    const char* find,
    const char* replacement
  )
{
  return replaceBytes(thread, source, find, replacement, strlen(replacement));
}

static void
testRoundTrips
  (
    Arcadia_Thread* thread
  )
{
  const char* inputs[] = { minimal, complete };
  for (int i = 0; i < 2; ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_Languages_Diagnostics* d = diagnostics(thread);
    Arcadia_VPL_IR_Module* module = read(thread, inputs[i], strlen(inputs[i]), d);
    Arcadia_String* first = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_write(thread, module, d));
    Arcadia_VPL_IR_Module* copy = read(thread, Arcadia_String_getBytes(thread, first), Arcadia_String_getNumberOfBytes(thread, first), d);
    Arcadia_String* second = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_write(thread, copy, d));
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, first, Arcadia_String_getBytes(thread, second), Arcadia_String_getNumberOfBytes(thread, second)));
    Arcadia_Tests_assertTrue(thread, module != copy && module->vertex->entryFunction != copy->vertex->entryFunction);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, d));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
expectReadFailureAt
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue n,
    Arcadia_Status expected,
    Arcadia_SizeValue expectedOffset
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_Languages_Diagnostics* d = diagnostics(thread);
  Arcadia_String* name = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, "bad.vpl"));
  Arcadia_SizeValue before = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) Arcadia_VPL_Text_read(thread, bytes, n, d, name);
  else raised = Arcadia_BooleanValue_True;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_Tests_assertTrue(thread, raised && status == expected && before == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, d));
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, d->diagnostics, 0, _Arcadia_VPL_Diagnostics_VerificationDiagnostic_getType(thread));
  Arcadia_Tests_assertTrue(thread, diagnostic->sourceName == name && diagnostic->line >= 1 && diagnostic->column >= 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isSizeValue(&diagnostic->_parent.inputPosition));
  if (expectedOffset != (Arcadia_SizeValue)-1) Arcadia_Tests_assertTrue(thread, Arcadia_Value_getSizeValue(&diagnostic->_parent.inputPosition) == expectedOffset);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
expectReadFailure
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue n,
    Arcadia_Status expected
  )
{
  expectReadFailureAt(thread, bytes, n, expected, (Arcadia_SizeValue)-1);
}

static void
testFailures
  (
    Arcadia_Thread* thread
  )
{
  const char* malformed[] = { "", ")", "(module", "/* unterminated", "(module 1e)", "(module +)", "(module _)", "(module \"x\n\")", "(module \"\\uD800\")", "(module \"\\q\")", "(module \"\\uZZZZ\")" };
  for (int i = 0; i < 11; ++i) expectReadFailure(thread, malformed[i], strlen(malformed[i]), i == 0 || i == 2 ? Arcadia_Status_SyntacticalError : Arcadia_Status_LexicalError);
  const unsigned char invalidUTF8[] = { '(', 0xf0, 0x80, 0x80, 0x80, ')' };
  expectReadFailure(thread, invalidUTF8, sizeof(invalidUTF8), Arcadia_Status_LexicalError);

  static const struct {
    const char* find;
    const char* replacement;
    Arcadia_Status status;
  } cases[] = {
    { "module 1", "module 2", Arcadia_Status_SemanticalError },
    { "(type Z void)", "(type Z void) (type Z int)", Arcadia_Status_SemanticalError },
    { "(type V vector float 4)", "(type V array V 2)", Arcadia_Status_SemanticalError },
    { "(type V vector float 4)", "(type V array Z 2)", Arcadia_Status_SemanticalError },
    { "(type V vector float 4)", "(type V vector float 256)", Arcadia_Status_SemanticalError },
    { "(register r42 V)", "(register r42 V) (register r042 V)", Arcadia_Status_SemanticalError },
    { "(register r42 V)", "(register x42 V)", Arcadia_Status_SyntacticalError },
    { "(construct r42", "(construct r43", Arcadia_Status_SemanticalError },
    { "(float 1.0)", "(float 1e999)", Arcadia_Status_SemanticalError },
    { "(float 1.0)", "(int 2147483648)", Arcadia_Status_SemanticalError },
    { "(float 1.0)", "(int -2147483649)", Arcadia_Status_SemanticalError },
    { "(float 1.0)", "(int 1e0)", Arcadia_Status_SyntacticalError },
    { "vertex start", "vertex missing", Arcadia_Status_SemanticalError },
    { "(return)", "(branch missing)", Arcadia_Status_SemanticalError },
    { "(return)", "(return) (copy r42 r42)", Arcadia_Status_SyntacticalError },
    { "(store (stage pos) r42)", "", Arcadia_Status_SemanticalError },
    { "(store (stage pos) r42)", "(load r42 (stage pos)) (store (stage pos) r42)", Arcadia_Status_SemanticalError },
    { "(construct r42 (float 1.0))", "(copy r42 r42)", Arcadia_Status_SemanticalError },
    { "(store (stage pos) r42)", "(store (stage missing) r42)", Arcadia_Status_SemanticalError },
    { "(construct r42", "(unknown r42", Arcadia_Status_SyntacticalError },
    { "(return)", "(return r42)", Arcadia_Status_SemanticalError },
    { "(uniforms)", "(uniforms (array u \"bad\" V 0))", Arcadia_Status_SemanticalError },
  };

  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_String* source = replace(thread, minimal, cases[i].find, cases[i].replacement);
    expectReadFailure(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), cases[i].status);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
  const char* divisions[] = { "(divide r1 (int 1) (int 0))", "(divide r1 (int -2147483648) (int -1))" };
  for (int i = 0; i < 2; ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_String* source = replace(thread, complete, "(copy r1 (int 0))", divisions[i]);
    expectReadFailure(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), Arcadia_Status_SemanticalError);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testLocatedDiagnostics
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_Languages_Diagnostics* d = diagnostics(thread);
  Arcadia_String* name = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, "line-test.vpl"));
  const char source[] = "// prefix\r\n  (module 2)";
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_VPL_Text_read(thread, source, sizeof(source) - 1, d, name);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)Arcadia_List_getObjectReferenceValueAt(thread, d->diagnostics, 0);
  // The incomplete root form is diagnosed at its opening parenthesis.
  Arcadia_Tests_assertTrue(thread, diagnostic->line == 2 && diagnostic->column == 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getSizeValue(&diagnostic->_parent.inputPosition) == 13);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testNumericPublicationRejection
  (
    Arcadia_Thread* thread
  )
{
  for (int i = 0; i < 2; ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_Languages_Diagnostics* d = diagnostics(thread);
    Arcadia_VPL_IR_Module* module = read(thread, minimal, sizeof(minimal) - 1, d);
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, module->vertex->entryFunction->blocks, 0);
    Arcadia_VPL_IR_ConstructInstruction* construct = (Arcadia_VPL_IR_ConstructInstruction*)Arcadia_List_getObjectReferenceValueAt(thread, block->instructions, 1);
    Arcadia_VPL_IR_ConstantValue* constant = (Arcadia_VPL_IR_ConstantValue*)Arcadia_List_getObjectReferenceValueAt(thread, construct->arguments, 0);
    constant->literal = Arcadia_Value_makeReal32Value(i ? NAN : INFINITY);
    Arcadia_SizeValue before = Arcadia_ValueStack_getSize(thread);
    Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) Arcadia_VPL_VerifiedModule_create(thread, module, d);
    else raised = Arcadia_BooleanValue_True;
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, raised && status == Arcadia_Status_SemanticalError && before == Arcadia_ValueStack_getSize(thread));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testPublicationAndLexicalDetails
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_Languages_Diagnostics* d = diagnostics(thread);
  Arcadia_String* source = replace(thread, minimal, "\"vertex_main\"", "'Unicode \\u0041 \\n \\u0000 \\t \\b \\\\ \\\' \\\"'");
  Arcadia_VPL_IR_Module* module = read(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), d);
  Arcadia_VPL_VerifiedModule* published = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, d));
  Arcadia_String* canonical = Arcadia_VPL_VerifiedModule_getText(thread, published);
  Arcadia_VPL_IR_Function* originalEntry = module->vertex->entryFunction;
  module->vertex->entryFunction = NULL;
  Arcadia_VPL_IR_Module* first = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, published, d));
  Arcadia_Tests_assertTrue(thread, first->vertex->entryFunction && first->vertex->entryFunction != originalEntry);
  first->vertex->entryFunction = NULL;
  Arcadia_VPL_IR_Module* second = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, published, d));
  Arcadia_Tests_assertTrue(thread, second != first && second->vertex->entryFunction != NULL);
  Arcadia_String* again = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_write(thread, second, d));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, canonical, Arcadia_String_getBytes(thread, again), Arcadia_String_getNumberOfBytes(thread, again)));
  Arcadia_String* boundaries = replace(thread, minimal, "(float 1.0)", "(int -2147483648)");
  read(thread, Arcadia_String_getBytes(thread, boundaries), Arcadia_String_getNumberOfBytes(thread, boundaries), d);
  Arcadia_String* negativeZero = replace(thread, minimal, "(float 1.0)", "(float -0.0)");
  Arcadia_VPL_IR_Module* zeros = read(thread, Arcadia_String_getBytes(thread, negativeZero), Arcadia_String_getNumberOfBytes(thread, negativeZero), d);
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, zeros->vertex->entryFunction->blocks, 0);
  Arcadia_VPL_IR_ConstructInstruction* construct = (Arcadia_VPL_IR_ConstructInstruction*)Arcadia_List_getObjectReferenceValueAt(thread, entry->instructions, 1);
  Arcadia_VPL_IR_ConstantValue* zero = (Arcadia_VPL_IR_ConstantValue*)Arcadia_List_getObjectReferenceValueAt(thread, construct->arguments, 0);
  Arcadia_Tests_assertTrue(thread, signbit(Arcadia_Value_getReal32Value(&zero->literal)));
  const char* trivia = "/* Unicode: \xe2\x98\x83 */ \\\r\n";
  Arcadia_StringBuilder* joined = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
  Arcadia_String* prefix = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, trivia));
  Arcadia_StringBuilder_insertBack(thread, joined, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)prefix));
  Arcadia_String* body = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, minimal));
  Arcadia_StringBuilder_insertBack(thread, joined, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)body));
  Arcadia_String* joinedText = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)joined)));
  read(thread, Arcadia_String_getBytes(thread, joinedText), Arcadia_String_getNumberOfBytes(thread, joinedText), d);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testPublishedExample
  (
    Arcadia_Thread* thread
  )
{
  char bytes[4096];
  FILE* file = fopen(VPL_TEXT_EXAMPLE_PATH, "rb");
  Arcadia_Tests_assertTrue(thread, file != NULL);
  size_t n = fread(bytes, 1, sizeof(bytes), file);
  int failed = ferror(file);
  int extra = fgetc(file);
  fclose(file);
  Arcadia_Tests_assertTrue(thread, !failed && extra == EOF);
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_Languages_Diagnostics* d = diagnostics(thread);
  Arcadia_VPL_IR_Module* module = read(thread, bytes, n, d);
  Arcadia_VPL_VerifiedModule* publication = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, d));
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_VerifiedModule_getText(thread, publication) != NULL);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testBinary32RoundTrips
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_Languages_Diagnostics* d = diagnostics(thread);
  Arcadia_VPL_IR_Module* module = read(thread, minimal, sizeof(minimal) - 1, d);
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, module->vertex->entryFunction->blocks, 0);
  Arcadia_VPL_IR_ConstructInstruction* construct = (Arcadia_VPL_IR_ConstructInstruction*)Arcadia_List_getObjectReferenceValueAt(thread, entry->instructions, 1);
  Arcadia_VPL_IR_ConstantValue* literal = (Arcadia_VPL_IR_ConstantValue*)Arcadia_List_getObjectReferenceValueAt(thread, construct->arguments, 0);
  const Arcadia_Natural32Value values[] = { 0, 0x80000000u, 1, 0x80000001u, 0x007fffffu, 0x00800000u, 0x3dcccccdu, 0x3f800001u, 0x7f7fffffu, 0xff7fffffu, 0x12345678u, 0xabcdefffu };
  Arcadia_SizeValue rootedSize = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    Arcadia_Real32Value value;
    memcpy(&value, &values[i], sizeof(value));
    literal->literal = Arcadia_Value_makeReal32Value(value);
    Arcadia_String* text = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_write(thread, module, d));
    Arcadia_VPL_IR_Module* copy = read(thread, Arcadia_String_getBytes(thread, text), Arcadia_String_getNumberOfBytes(thread, text), d);
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, copy->vertex->entryFunction->blocks, 0);
    Arcadia_VPL_IR_ConstructInstruction* instruction = (Arcadia_VPL_IR_ConstructInstruction*)Arcadia_List_getObjectReferenceValueAt(thread, block->instructions, 1);
    Arcadia_VPL_IR_ConstantValue* constant = (Arcadia_VPL_IR_ConstantValue*)Arcadia_List_getObjectReferenceValueAt(thread, instruction->arguments, 0);
    Arcadia_Real32Value actual = Arcadia_Value_getReal32Value(&constant->literal);
    Arcadia_Natural32Value bits;
    memcpy(&bits, &actual, sizeof(bits));
    Arcadia_Tests_assertTrue(thread, bits == values[i]);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - rootedSize);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testGrammarAndBindingConformance
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* source;
    const char* find;
    const char* replacement;
    Arcadia_Status status; // Zero means valid.
  } cases[] = {
    { minimal, "start \"vertex_main\" Z", "start\"vertex_main\"Z", 0 },
    { minimal, "(type Z void)", "(type Z void) (type function int) (type start bool) (type entry int)", 0 },
    { minimal, "(type Z void)", "(type Z void) (type Empty record \"Empty\")", 0 },
    { minimal, "(type Z void)", "(type Z void) (type Empty record \"\")", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type R record \"R\" (field \"\" V))", Arcadia_Status_SemanticalError },
    { minimal, "(float 1.0)", "(float +.5E+1)", 0 },
    { minimal, "module 1", "module 0001", 0 },
    { minimal, "(module", "(module/**/", 0 },
    { minimal, "(module", "(module\\\n", 0 },
    { minimal, "vertex start", "Vertex start", Arcadia_Status_SyntacticalError },
    { minimal, "module", "mod\\\nule", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void) (type true int)", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void) (type false int)", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void) (type void int)", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void extra)", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad missing)", Arcadia_Status_SyntacticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad array Missing 1)", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad array V 0)", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad vector float 1)", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad matrix 5 2)", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad record \"R\" (field \"x\" Z))", Arcadia_Status_SemanticalError },
    { minimal, "(type Z void)", "(type Z void) (type Bad record \"R\" (field \"x\" V) (field \"x\" V))", Arcadia_Status_SemanticalError },
    { minimal, "(uniforms)", "(uniforms (scalar u \"u\" V) (scalar u \"other\" V))", Arcadia_Status_SemanticalError },
    { minimal, "(uniforms)", "(uniforms (scalar u \"same\" V) (scalar other \"same\" V))", Arcadia_Status_SemanticalError },
    { minimal, "(uniforms)", "(uniforms (record u \"u\" V))", Arcadia_Status_SemanticalError },
    { minimal, "(uniforms)", "(uniforms (scalar u \"\" V))", Arcadia_Status_SemanticalError },
    { minimal, "\"vertex_main\"", "\"\"", Arcadia_Status_SemanticalError },
    { minimal, "\"position\"", "\"\"", Arcadia_Status_SemanticalError },
    { minimal, "(uniforms)", "(uniforms (scalar u \"u\" Missing))", Arcadia_Status_SemanticalError },
    { minimal, "(interface (out pos \"position\" V -1 position))", "(interface (out pos \"position\" V -1 position) (out pos \"other\" V 0 none))", Arcadia_Status_SemanticalError },
    { minimal, "(register r42 V)", "(register r V)", Arcadia_Status_SyntacticalError },
    { minimal, "(register r42 V)", "(register r42 Missing)", Arcadia_Status_SemanticalError },
    { minimal, "(register r42 V)", "(register \"r42\" V)", Arcadia_Status_SyntacticalError },
    { minimal, "(uniforms)", "(uniforms (array u \"u\" V \"2\"))", Arcadia_Status_SyntacticalError },
    { minimal, "(return)", "(return) (return)", Arcadia_Status_SyntacticalError },
    { minimal, "(store (stage pos) r42)", "(store (uniform pos) r42)", Arcadia_Status_SemanticalError },
    { minimal, "(store (stage pos) r42)", "(store (parameter pos) r42)", Arcadia_Status_SemanticalError },
    { minimal, "(store (stage pos) r42)", "(store (member_ref (local r42) \"x\") r42)", Arcadia_Status_SemanticalError },
    { minimal, "(store (stage pos) r42)", "(store (index_ref (local r42) (int 0)) r42)", Arcadia_Status_SemanticalError },
    { minimal, "(return)", "(branch_if (bool true) entry Missing)", Arcadia_Status_SemanticalError },
    { complete, "(parameter input F)", "(parameter input F) (parameter input F)", Arcadia_Status_SemanticalError },
    { complete, "(function helper 'helper' F", "(function main 'helper' F", Arcadia_Status_SemanticalError },
    { complete, "(function helper 'helper' F", "(function helper 'vertex_main' F", Arcadia_Status_SemanticalError },
    { complete, "(return r0)", "(return r0)) (block entry (return r0)", Arcadia_Status_SemanticalError },
    { complete, "(call r1 helper (float 2.0))", "(call r1 count (int 0))", Arcadia_Status_SemanticalError },
    { complete, "(texture r0 r7 r8)", "(texture void r7 r8)", Arcadia_Status_SyntacticalError },
    { complete, "(texture r0 r7 r8)", "(texture r0 r7 r8 r8)", Arcadia_Status_SyntacticalError },
    { complete, "(call void sink)", "(call void Missing)", Arcadia_Status_SemanticalError },
    { complete, "(call void sink)", "(call void sink (int 0))", Arcadia_Status_SemanticalError },
    { complete, "(swizzle r6 r5 \"xyz\")", "(swizzle r6 r5 \"\")", Arcadia_Status_SemanticalError },
    { complete, "(block kill (discard))", "(block kill (discard) (return))", Arcadia_Status_SyntacticalError }
  };

  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_String* source = replace(thread, cases[i].source, cases[i].find, cases[i].replacement);
    if (cases[i].status) expectReadFailure(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), cases[i].status);
    else {
      Arcadia_Languages_Diagnostics* d = diagnostics(thread);
      Arcadia_VPL_IR_Module* module = read(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), d);
      Arcadia_VPL_VerifiedModule* snapshot = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, d));
      Arcadia_Tests_assertTrue(thread, snapshot != NULL && !Arcadia_Languages_Diagnostics_hasErrors(thread, d));
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
  char trailing[sizeof(minimal) + 2];
  memcpy(trailing, minimal, sizeof(minimal) - 1);
  trailing[sizeof(minimal) - 1] = ')';
  trailing[sizeof(minimal)] = 0;
  expectReadFailureAt(thread, trailing, sizeof(minimal), Arcadia_Status_SyntacticalError, sizeof(minimal) - 1);
}

static void
appendText
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* builder,
    const char* text
  )
{
  Arcadia_Value value = Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, text, strlen(text)));
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_StringBuilder_insertBack(thread, builder, value);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
appendNumber
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* builder,
    Arcadia_SizeValue number
  )
{
  Arcadia_String* value = Arcadia_String_createFromSize(thread, number);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)value);
  Arcadia_StringBuilder_insertBack(thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)value));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
testTypeDepthIndependentOfDeclarationOrder
  (
    Arcadia_Thread* thread
  )
{
  for (int reverse = 0; reverse < 2; ++reverse) {
    for (Arcadia_SizeValue depth = 128; depth <= 129; ++depth) {
      Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
      Arcadia_StringBuilder* definitions = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
      appendText(thread, definitions, "(type Z void) ");
      for (Arcadia_SizeValue j = 0; j <= depth; ++j) {
        Arcadia_SizeValue i = reverse ? depth - j : j;
        appendText(thread, definitions, "(type T"); appendNumber(thread, definitions, i);
        if (!i) appendText(thread, definitions, " int) ");
        else {
          appendText(thread, definitions, " array T"); appendNumber(thread, definitions, i - 1); appendText(thread, definitions, " 1) ");
        }
      }
      Arcadia_String* source = replaceBytes(thread, minimal, "(type Z void)", Arcadia_StringBuilder_getBytes(thread, definitions), Arcadia_StringBuilder_getNumberOfBytes(thread, definitions));
      if (depth == 129) expectReadFailure(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), Arcadia_Status_SemanticalError);
      else {
        Arcadia_Languages_Diagnostics* d = diagnostics(thread);
        read(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), d);
        Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, d));
      }
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    }
  }
}

static void
testConstructorConformance
  (
    Arcadia_Thread* thread
  )
{
  static const struct {
    const char* type;
    const char* arguments;
    Arcadia_BooleanValue valid;
  } cases[] = {
    { "F", "(int 1)", 1 }, { "I", "(float 1.0)", 0 },
    { "B", "(bool true)", 1 }, { "B", "(int 0)", 0 },
    { "F", "", 0 }, { "F", "(float 1) (float 2)", 0 },
    { "V", "(float 1)", 1 }, { "V", "(int 1)", 1 }, { "V", "(bool false)", 0 },
    { "V", "r61 (float 1)", 1 }, { "V", "r61", 0 },
    { "V3", "r61", 1 }, { "V2", "r61", 0 },
    { "IV2", "(int 1)", 1 }, { "IV2", "(float 1)", 0 },
    { "BV2", "(bool true)", 1 }, { "BV2", "(int 1)", 0 },
    { "V2", "r62", 0 }, { "BV2", "r62", 0 },
    { "M", "(float 1)", 1 }, { "M", "(int 1)", 1 }, { "M", "r61 r61", 1 },
    { "M", "(bool false)", 0 }, { "M", "(float 1) (float 2) (float 3) (float 4) (float 5)", 0 },
    { "A", "(float 1) (float 2)", 1 }, { "A", "(int 1) (float 2)", 0 },
    { "R", "(float 1) (int 2)", 1 }, { "R", "(int 1) (int 2)", 0 },
    { "R", "(int 1) (float 2)", 0 }, { "R", "(float 1)", 0 },
    { "E", "", 1 }, { "S", "", 0 }, { "Z", "", 0 }
  };

  for (Arcadia_SizeValue i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_String* source = replace(thread, minimal, "(type Z void)",
      "(type Z void) (type F float) (type I int) (type B bool) (type V2 vector float 2)"
      " (type V3 vector float 3) (type IV2 vector int 2) (type BV2 vector bool 2)"
      " (type M matrix 2 3) (type A array F 2) (type R record \"Pair\" (field \"x\" F) (field \"y\" I))"
      " (type E record \"Empty\") (type S sampler2D)");
    Arcadia_StringBuilder* registers = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
    appendText(thread, registers, "(register r42 V) (register r61 V3) (register r62 IV2) (register r60 ");
    appendText(thread, registers, cases[i].type); appendText(thread, registers, ")");
    source = replaceSpan(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), "(register r42 V)", Arcadia_StringBuilder_getBytes(thread, registers), Arcadia_StringBuilder_getNumberOfBytes(thread, registers));
    Arcadia_StringBuilder* operations = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
    appendText(thread, operations, "(construct r42 (float 1.0)) (construct r61 (float 1)) (construct r62 (int 1)) (construct r60 ");
    appendText(thread, operations, cases[i].arguments); appendText(thread, operations, ")");
    source = replaceSpan(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), "(construct r42 (float 1.0))", Arcadia_StringBuilder_getBytes(thread, operations), Arcadia_StringBuilder_getNumberOfBytes(thread, operations));
    if (!cases[i].valid) expectReadFailure(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), Arcadia_Status_SemanticalError);
    else {
      Arcadia_Languages_Diagnostics* d = diagnostics(thread);
      Arcadia_VPL_IR_Module* module = read(thread, Arcadia_String_getBytes(thread, source), Arcadia_String_getNumberOfBytes(thread, source), d);
      root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, d));
      Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, d));
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testHandwrittenExamples
  (
    Arcadia_Thread* thread
  )
{
  const char* files[] = { VPL_TEXT_SPEC_DIRECTORY "/control-flow.vpl", VPL_TEXT_SPEC_DIRECTORY "/aggregate-storage.vpl" };
  for (int i = 0; i < 2; ++i) {
    char bytes[8192];
    FILE* file = fopen(files[i], "rb");
    Arcadia_Tests_assertTrue(thread, file != NULL);
    size_t n = fread(bytes, 1, sizeof(bytes), file);
    int failed = ferror(file), extra = fgetc(file);
    fclose(file);
    Arcadia_Tests_assertTrue(thread, !failed && extra == EOF);
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_Languages_Diagnostics* d = diagnostics(thread);
    Arcadia_VPL_IR_Module* module = read(thread, bytes, n, d);
    Arcadia_VPL_VerifiedModule* snapshot = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, d));
    Arcadia_VPL_IR_Module* copy = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, snapshot, d));
    Arcadia_String* canonical = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_write(thread, copy, d));
    Arcadia_String* original = Arcadia_VPL_VerifiedModule_getText(thread, snapshot);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, original, Arcadia_String_getBytes(thread, canonical), Arcadia_String_getNumberOfBytes(thread, canonical)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static int
runRoundTrips
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRoundTrips) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFailures
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testFailures) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runPublicationAndLexicalDetails
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testPublicationAndLexicalDetails) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runLocatedDiagnostics
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testLocatedDiagnostics) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNumericPublicationRejection
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNumericPublicationRejection) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runPublishedExample
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testPublishedExample) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runBinary32RoundTrips
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testBinary32RoundTrips) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGrammarAndBindingConformance
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testGrammarAndBindingConformance) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runTypeDepthIndependentOfDeclarationOrder
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testTypeDepthIndependentOfDeclarationOrder) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runConstructorConformance
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testConstructorConformance) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runHandwrittenExamples
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testHandwrittenExamples) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "RoundTrips", &runRoundTrips },
    { "Failures", &runFailures },
    { "PublicationAndLexicalDetails", &runPublicationAndLexicalDetails },
    { "LocatedDiagnostics", &runLocatedDiagnostics },
    { "NumericPublicationRejection", &runNumericPublicationRejection },
    { "PublishedExample", &runPublishedExample },
    { "Binary32RoundTrips", &runBinary32RoundTrips },
    { "GrammarAndBindingConformance", &runGrammarAndBindingConformance },
    { "TypeDepthIndependentOfDeclarationOrder", &runTypeDepthIndependentOfDeclarationOrder },
    { "ConstructorConformance", &runConstructorConformance },
    { "HandwrittenExamples", &runHandwrittenExamples },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
