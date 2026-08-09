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

#include "Arcadia.TemplateEngine/Context.h"

#include "Arcadia.TemplateEngine/Environment.h"
#include "Arcadia.TemplateEngine/FileContext.h"
#include "Arcadia.TemplateEngine/DependenciesContext.h"

#include "Arcadia.TemplateEngine/Library.h"

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Logging/Include.h"
#include "Arcadia/DDL/Include.h"

static void
Context_constructImpl
  (
    Arcadia_Thread* thread,
    Context* self
  );

static void
Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    ContextDispatch* self
  );

static void
Context_destruct
  (
    Arcadia_Thread* thread,
    Context* self
  );

static void
Context_visit
  (
    Arcadia_Thread* thread,
    Context* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Context_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Context_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Context_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Context_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Context", Context,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

void
Context_constructImpl
  (
    Arcadia_Thread* thread,
    Context* self
  )
{
  Arcadia_EnterConstructor(Context);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  static const uint8_t sourceBytes[] =
    u8"{\n"
    u8"  generatorName : \"Michael Heilmann's Arcadia Template Engine\",\n"
    u8"  generatorHome : \"https://michaelheilmann.com\",\n"
    u8"}\n"
    ;
  Arcadia_String* sourceString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, sourceBytes, sizeof(sourceBytes) - 1)));
  self->environment = Environment_loadString(thread, sourceString);
  Arcadia_TemplateEngine_registerTimeLibrary(thread, self->environment);

  self->sourceFilePath = NULL;
  self->targetFilePath = NULL;
  self->dependenciesFilePath = NULL;
  self->environmentFilePath = NULL;
  self->logFilePath = NULL;

  self->files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->dependenciesContext = DependenciesContext_create(thread);
  self->consoleLog = (Arcadia_Log*)Arcadia_ConsoleLog_create(thread);

  self->stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);

  self->targetBuffer = Arcadia_ByteArrayBuilder_create(thread);
  self->target = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  self->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  self->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

  self->diagnostics = Arcadia_Languages_Diagnostics_create(thread, self->consoleLog);

  Arcadia_LeaveConstructor(Context);
}

static void
Context_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    ContextDispatch* self
  )
{/*Intentionally empty.*/}

static void
Context_destruct
  (
    Arcadia_Thread* thread,
    Context* self
  )
{/*Intentionally empty.*/}

static void
Context_visit
  (
    Arcadia_Thread* thread,
    Context* self
  )
{
  if (self->sourceFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sourceFilePath);
  }
  if (self->targetFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->targetFilePath);
  }
  if (self->environmentFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->environmentFilePath);
  }
  if (self->dependenciesFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dependenciesFilePath);
  }
  if (self->logFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->logFilePath);
  }

  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->consoleLog);
  }

  if (self->consoleLog) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->consoleLog);
  }

  if (self->environment) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->environment);
  }
  if (self->targetBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->targetBuffer);
  }
  if (self->target) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->target);
  }

  if (self->temporaryBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporaryBuffer);
  }
  if (self->temporary) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporary);
  }

  if (self->stack) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stack);
  }
  if (self->files) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->files);
  }

  if (self->dependenciesContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dependenciesContext);
  }
}

Context*
Context_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Context);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Context);
}

static void
onRecursionGuard
  (
    Arcadia_Thread* thread,
    Context* self,
    Arcadia_FilePath* path
  )
{
  path = Arcadia_FilePath_getFullPath(thread, path);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->files); i < n; ++i) {
    Arcadia_FilePath* p = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueAt(thread, self->files, i);
    if (Arcadia_FilePath_isEqualTo(thread, p, path)) {
      Arcadia_String* ps = Arcadia_FilePath_toGeneric(thread, p);
      Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
      Arcadia_Value v = Arcadia_Value_makeObjectReferenceValue(ps);
      Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"recursive include of file `");
      Arcadia_StringBuilder_insertBack(thread, sb, v);
      Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"`\n");

      Arcadia_Log_error(thread, self->consoleLog, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb)));

      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  Arcadia_List_insertBackObjectReferenceValue(thread, self->files, path);
}

void
Context_onRunInner
  (
    Arcadia_Thread* thread,
    Context* self,
    Arcadia_FilePath* dependingFile
  )
{
  while (!Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)self->stack)) {
    Arcadia_Value elementValue = Arcadia_Stack_peek(thread, self->stack);
    Arcadia_Stack_pop(thread, self->stack);
    Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_Value_getObjectReferenceValue(&elementValue);

    onRecursionGuard(thread, self, filePath);
    DependencyContext_onFile(thread, self->dependenciesContext, dependingFile);
    DependencyContext_onDependency(thread, self->dependenciesContext, dependingFile, filePath);

    FileContext* fileContext = FileContext_create(thread, self, self->environment, dependingFile, filePath);
    FileContext_execute(thread, fileContext);
    Arcadia_List_removeAt(thread, self->files, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->files) - 1, 1);
  }
}

void
Context_onRun
  (
    Arcadia_Thread* thread,
    Context* self
  )
{
  self->dependenciesContext->dependenciesFilePath = self->dependenciesFilePath;

#if 0
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_FilePath* workingDirectory = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_StringBuilder_insertBackCxxString(thread, message, "working directory: ");
  Arcadia_StringBuilder_insertBackString(thread, message, Arcadia_FilePath_toNative(thread, workingDirectory, Arcadia_BooleanValue_False));
  Arcadia_StringBuilder_insertBackCxxString(thread, message, "\n");
  Arcadia_Log_information(thread, self->consoleLog, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(message)));
#endif

  Arcadia_Stack_pushObjectReferenceValue(thread, self->stack, (Arcadia_Object*)self->sourceFilePath);
  Context_onRunInner(thread, self, self->targetFilePath);
  DependenciesContext_write(thread, self->dependenciesContext);
}
