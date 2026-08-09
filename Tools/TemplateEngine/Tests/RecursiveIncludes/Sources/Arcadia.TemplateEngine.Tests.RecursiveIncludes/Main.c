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

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Ring2/Include.h"
#include "Arcadia.TemplateEngine/Context.h"
#include "Arcadia.TemplateEngine/DependenciesContext.h"
#include "Arcadia.TemplateEngine/Environment.h"
#include "Arcadia.TemplateEngine/FileContext.h"

static void
test_1_1
  (
    Arcadia_Thread* thread
  )
{
  Context* context = Context_create(thread);

  context->sourceFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-1/1.t"));
  context->targetFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-1/1.txt"));
  context->dependenciesFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-1/1.txt.dependencies"));
  context->environmentFilePath = NULL;

  context->stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);
  context->targetBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->target = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Context_onRun(thread, context);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

static void
test_1_2_1
  (
    Arcadia_Thread* thread
  )
{
  Context* context = Context_create(thread);

  context->sourceFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-1/1.t"));
  context->targetFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-1/1.txt"));
  context->dependenciesFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-1/1.txt.dependencies"));
  context->environmentFilePath = NULL;

  context->stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);
  context->targetBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->target = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Context_onRun(thread, context);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

static void
test_1_2_3_1
(
  Arcadia_Thread* thread
) {
  Context* context = Context_create(thread);

  context->sourceFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-1/1.t"));
  context->targetFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-1/1.txt"));
  context->dependenciesFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-1/1.txt.dependencies"));
  context->environmentFilePath = NULL;

  context->stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);
  context->targetBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->target = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Context_onRun(thread, context);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

static void
test_1_2_3_2
  (
    Arcadia_Thread* thread
  )
{
  Context* context = Context_create(thread);

  context->sourceFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-2/1.t"));
  context->targetFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-2/1.txt"));
  context->dependenciesFilePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"1-2-3-2/1.txt.dependencies"));
  context->environmentFilePath = NULL;

  context->stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);
  context->targetBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->target = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Context_onRun(thread, context);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test_1_1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test_1_2_1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test_1_2_3_1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test_1_2_3_2)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
