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

#include "Arcadia.TemplateEngine/FileContext.h"

#include <stdio.h> // @todo Remove references to `stdio.h`.

#include "Arcadia.TemplateEngine/Parser/Parser.h"
#include "Arcadia.TemplateEngine/Parser/Tree.h"
#include "Arcadia.TemplateEngine/Context.h"
#include "Arcadia.TemplateEngine/Environment.h"

static void
evalInvoke
  (
    Arcadia_Thread* thread,
    FileContext* context,
    Directives_Tree* ast
  );

static void
eval
  (
    Arcadia_Thread* thread,
    FileContext* context,
    Directives_Tree* ast
  );

static void
evalInvoke
  (
    Arcadia_Thread* thread,
    FileContext* context,
    Directives_Tree* ast
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)ast->invokeExpr.target);
  Arcadia_Value v = Environment_get(thread, context->environment, k, Arcadia_BooleanValue_True);
  if (Arcadia_Value_isVoidValue(&v)) {
    // Error.
    Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"variable `");
    Arcadia_StringBuilder_insertBack(thread, sb, k);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"` not defined\n");

    Arcadia_Log_error(thread, context->context->consoleLog, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Value_isForeignProcedureValue(&v)) {
    // Error.
    Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"variable `");
    Arcadia_StringBuilder_insertBack(thread, sb, k);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"` is not of procedure string\n");

    Arcadia_Log_error(thread, context->context->consoleLog, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb)));

    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ForeignProcedure* p = Arcadia_Value_getForeignProcedureValue(&v);
  Arcadia_Value targetValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);

  Arcadia_SizeValue oldStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_SizeValue numberOfValues = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)ast->invokeExpr.arguments);
  if (numberOfValues > Arcadia_Natural8Value_Maximum - 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = numberOfValues; i < n; ++i) {
    Arcadia_Value temporary = Arcadia_List_getAt(thread, ast->invokeExpr.arguments, i);
    Arcadia_ValueStack_pushValue(thread, &temporary);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, numberOfValues);
  (*p)(thread);
  if (oldStackSize > Arcadia_ValueStack_getSize(thread) || (Arcadia_ValueStack_getSize(thread) - oldStackSize) != 1) {
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - oldStackSize);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  targetValue = Arcadia_ValueStack_getValue(thread, 0);
  Arcadia_ValueStack_popValues(thread, 1);


  Arcadia_String* string = NULL;
  if (Arcadia_Value_isInteger16Value(&targetValue)) {
    string = Arcadia_String_createFromInteger16(thread, Arcadia_Value_getInteger16Value(&targetValue));
  } else if (Arcadia_Value_isInteger32Value(&targetValue)) {
    string = Arcadia_String_createFromInteger32(thread, Arcadia_Value_getInteger32Value(&targetValue));
  } else if (Arcadia_Value_isInteger64Value(&targetValue)) {
    string = Arcadia_String_createFromInteger64(thread, Arcadia_Value_getInteger64Value(&targetValue));
  } else if (Arcadia_Value_isInteger8Value(&targetValue)) {
    string = Arcadia_String_createFromInteger8(thread, Arcadia_Value_getInteger8Value(&targetValue));
  } else  if (Arcadia_Value_isObjectReferenceValue(&targetValue)) {
    Arcadia_Object* object = Arcadia_Value_getObjectReferenceValue(&targetValue);
    if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, object), _Arcadia_String_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    string = (Arcadia_String*)object;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, context->context->targetBuffer, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));

}

static void
eval
  (
    Arcadia_Thread* thread,
    FileContext* context,
    Directives_Tree* ast
  )
{
  switch (ast->type) {
    case Arcadia_TemplateEngine_TreeType_Text: {
      Arcadia_Unicode_Encoder_encodeString(thread, context->context->target, ast->text.text, context->context->targetBuffer);
    } break;
    case Arcadia_TemplateEngine_TreeType_AtLiteralExpr: {
      Arcadia_Natural32Value targetCodePoint = '@';
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, context->context->target, &targetCodePoint, 1, context->context->targetBuffer);
    } break;
    case Arcadia_TemplateEngine_TreeType_NameExpr: {
      Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)ast->nameExpr.name);
      Arcadia_Value v = Environment_get(thread, context->environment, k, Arcadia_BooleanValue_True);
      if (Arcadia_Value_isVoidValue(&v)) {
        // Error.
        Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"variable `");
        Arcadia_StringBuilder_insertBack(thread, sb, k);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"` not defined\n");
        fwrite(Arcadia_StringBuilder_getBytes(thread, sb), 1, Arcadia_StringBuilder_getNumberOfBytes(thread, sb), stderr);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_jump(thread);
      } else  if (!Arcadia_Value_isObjectReferenceValue(&v)) {
        // Error.
        Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"variable `");
        Arcadia_StringBuilder_insertBack(thread, sb, k);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"` is not of type string\n");
        fwrite(Arcadia_StringBuilder_getBytes(thread, sb), 1, Arcadia_StringBuilder_getNumberOfBytes(thread, sb), stderr);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Object* object = Arcadia_Value_getObjectReferenceValue(&v);
      if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, object), _Arcadia_String_getType(thread))) {
        // Error.
        Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"variable `");
        Arcadia_StringBuilder_insertBack(thread, sb, k);
        Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"` is not of type string\n");
        fwrite(Arcadia_StringBuilder_getBytes(thread, sb), 1, Arcadia_StringBuilder_getNumberOfBytes(thread, sb), stderr);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, context->context->targetBuffer, Arcadia_String_getBytes(thread, (Arcadia_String*)object), Arcadia_String_getNumberOfBytes(thread, (Arcadia_String*)object));
    } break;
    case Arcadia_TemplateEngine_TreeType_InvokeExpr: {
      if (Arcadia_String_isEqualTo_pn(thread, ast->invokeExpr.target, u8"include", sizeof(u8"include") - 1)) {
        if (1 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)ast->invokeExpr.arguments)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        }
        Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, ast->invokeExpr.arguments, 0);
        if (!Arcadia_Object_isInstanceOf(thread, object, _Arcadia_String_getType(thread))) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
          Arcadia_Thread_jump(thread);
        }
        Arcadia_String* string = (Arcadia_String*)object;
        Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, string);
        Arcadia_Value value;
        Arcadia_Value_setObjectReferenceValue(&value, filePath);
        Arcadia_Stack_push(thread, context->context->stack, value);
        Context_onRunInner(thread, context->context, context->includedFilePath);
      } else {
        evalInvoke(thread, context, ast);
      }
    } break;
    case Arcadia_TemplateEngine_TreeType_StringLiteralExpr: {
      Arcadia_Unicode_Encoder_encodeString(thread, context->context->target, ast->stringLiteralExpr.string, context->context->targetBuffer);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

static Directives_Tree*
parseDirective
  (
    Arcadia_Thread* thread,
    FileContext* context
  )
{
  Arcadia_Value treeValue = Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)context->parser);
  return Arcadia_Value_getObjectReferenceValueChecked(thread, treeValue, _Directives_Tree_getType(thread));
}

static void
FileContext_destruct
  (
    Arcadia_Thread* thread,
    FileContext* self
  );

static void
FileContext_visit
  (
    Arcadia_Thread* thread,
    FileContext* self
  );

static void
FileContext_constructImpl
  (
    Arcadia_Thread* thread,
    FileContext* self
  );

static void
FileContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    FileContextDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&FileContext_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&FileContext_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&FileContext_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&FileContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.FileContext", FileContext,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
FileContext_destruct
  (
    Arcadia_Thread* thread,
    FileContext* self
  )
{/*Intentionally empty.*/}

static void
FileContext_visit
  (
    Arcadia_Thread* thread,
    FileContext* self
  )
{
  if (self->fileBytes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fileBytes);
  }
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->includedFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->includedFilePath);
  }
  if (self->reader) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->reader);
  }
  if (self->environment) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->environment);
  }
  if (self->includingFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->includingFilePath);
  }
  if (self->parser) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->parser);
  }
  if (self->scanner) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scanner);
  }
}

static void
FileContext_constructImpl
  (
    Arcadia_Thread* thread,
    FileContext* self
  )
{
  Arcadia_EnterConstructor(FileContext);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->context = (Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Context_getType(thread));
  self->environment = (Environment*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Environment_getType(thread));

  self->includingFilePath = (Arcadia_FilePath*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_FilePath_getType(thread));;
  self->includedFilePath = (Arcadia_FilePath*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_FilePath_getType(thread));

  self->fileBytes = NULL;
  self->reader = NULL;

  self->parser = NULL;
  self->scanner = NULL;

  Arcadia_LeaveConstructor(FileContext);
}

static void
FileContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    FileContextDispatch* self
  )
{ }

FileContext*
FileContext_create
  (
    Arcadia_Thread* thread,
    Context* context,
    Environment* environment,
    Arcadia_FilePath* includingFilePath,
    Arcadia_FilePath* includedFilePath
  )
{
  _Arcadia_BeginCreate(FileContext);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)environment);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)includingFilePath);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)includedFilePath);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(FileContext);
}

void
FileContext_execute
  (
    Arcadia_Thread* thread,
    FileContext* context
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ByteArrayBuilder* sourceByteBuffer = NULL;

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    sourceByteBuffer = Arcadia_FileSystem_getFileContents(thread, fileSystem, context->includedFilePath);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);

    Arcadia_String* ps = Arcadia_FilePath_toGeneric(thread, context->includedFilePath);
    Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
    Arcadia_Value v = Arcadia_Value_makeObjectReferenceValue((Arcadia_ObjectReferenceValue)ps);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"failed to read file `");
    Arcadia_StringBuilder_insertBack(thread, sb, v);
    Arcadia_StringBuilder_insertBackCxxString(thread, sb, u8"`\n");

    Arcadia_Log_error(thread, context->context->consoleLog, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb)));

    Arcadia_Thread_jump(thread);
  }
  context->fileBytes = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, sourceByteBuffer), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, sourceByteBuffer)));
  context->reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, context->fileBytes));
  if (!context->parser) {
    context->parser = Arcadia_TemplateEngine_Parser_create(thread, context->context->diagnostics, Arcadia_FilePath_toGeneric(thread, context->includedFilePath), 0, context->reader);
  }
  if (!context->scanner) {
    context->scanner = (Arcadia_TemplateEngine_Scanner*)context->parser->scanner;
  }
  Arcadia_TemplateEngine_Parser_setPosition(thread, context->parser, Arcadia_FilePath_toGeneric(thread, context->includedFilePath), 1);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)context->parser, context->reader);
  Directives_Tree* fileAST = (Directives_Tree*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)context->parser), _Directives_Tree_getType(thread));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fileAST->file.children); i < n; ++i) {
    Directives_Tree* ast = (Directives_Tree*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, fileAST->file.children, i, _Directives_Tree_getType(thread));
    eval(thread, context, ast);
  }
}
