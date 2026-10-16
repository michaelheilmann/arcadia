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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/TextInternal.h"

static void
nodeConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Node* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Text_Node);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (_numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->kind = 0;
  self->text = NULL;
  self->items = NULL;
  self->binding = NULL;
  self->state = 0;
  self->offset = 0;
  self->ordinal = 0;
  Arcadia_LeaveConstructor(Arcadia_VPL_Text_Node);
}

static void
nodeVisit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Node* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->items) Arcadia_Object_visit(thread, (Arcadia_Object*)self->items);
  if (self->binding) Arcadia_Object_visit(thread, self->binding);
}

static const Arcadia_ObjectType_Operations nodeObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&nodeConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&nodeVisit,
};

static const Arcadia_Type_Operations nodeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &nodeObjectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Text.Node", Arcadia_VPL_Text_Node,
                         u8"Arcadia.Object", Arcadia_Object, &nodeOperations);

static void
contextConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Text_Context);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (_numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->source = NULL;
  self->sourceName = NULL;
  self->diagnostics = NULL;
  self->roots = NULL;
  self->locations = NULL;
  self->types = NULL;
  self->uniforms = NULL;
  self->functions = NULL;
  self->registers = NULL;
  self->parameters = NULL;
  self->interface = NULL;
  self->blocks = NULL;
  self->output = NULL;
  self->typeOrder = NULL;
  self->position = 0;
  Arcadia_LeaveConstructor(Arcadia_VPL_Text_Context);
}

static void
contextVisit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* self
  )
{
  if (self->source) Arcadia_RuntimeByteArray_visit(thread, self->source);
  if (self->sourceName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sourceName);
  if (self->diagnostics) Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  if (self->roots) Arcadia_Object_visit(thread, (Arcadia_Object*)self->roots);
  if (self->locations) Arcadia_Object_visit(thread, (Arcadia_Object*)self->locations);
  if (self->types) Arcadia_Object_visit(thread, (Arcadia_Object*)self->types);
  if (self->uniforms) Arcadia_Object_visit(thread, (Arcadia_Object*)self->uniforms);
  if (self->functions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->functions);
  if (self->registers) Arcadia_Object_visit(thread, (Arcadia_Object*)self->registers);
  if (self->parameters) Arcadia_Object_visit(thread, (Arcadia_Object*)self->parameters);
  if (self->interface) Arcadia_Object_visit(thread, (Arcadia_Object*)self->interface);
  if (self->blocks) Arcadia_Object_visit(thread, (Arcadia_Object*)self->blocks);
  if (self->output) Arcadia_Object_visit(thread, (Arcadia_Object*)self->output);
  if (self->typeOrder) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeOrder);
}

static const Arcadia_ObjectType_Operations contextObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&contextConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&contextVisit,
};

static const Arcadia_Type_Operations contextOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &contextObjectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Text.Context", Arcadia_VPL_Text_Context,
                         u8"Arcadia.Object", Arcadia_Object, &contextOperations);

static Arcadia_VPL_Text_Context*
newContext
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Text_Context);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_Text_Context);
}

Arcadia_VPL_Text_Context*
Arcadia_VPL_Text_createContext
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  Arcadia_VPL_Text_Context* self = newContext(thread);
  // Kept on the caller's value stack until its success/failure cleanup.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  self->diagnostics = diagnostics;
  self->roots = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->locations = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->types = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->uniforms = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->output = Arcadia_StringBuilder_create(thread);
  return self;
}

Arcadia_Object*
Arcadia_VPL_Text_own
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  Arcadia_List_insertBackObjectReferenceValue(thread, context->roots, object);
  Arcadia_ValueStack_popValues(thread, 1);
  return object;
}

void
Arcadia_VPL_Text_append
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* builder,
    const void* bytes,
    Arcadia_SizeValue count
  )
{
  Arcadia_Value value = Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, bytes, count));
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_StringBuilder_insertBack(thread, builder, value);
  Arcadia_ValueStack_popValues(thread, 1);
}

void
Arcadia_VPL_Text_locate
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic,
    Arcadia_SizeValue offset
  )
{
  const unsigned char* bytes = Arcadia_RuntimeByteArray_getBytes(thread, context->source);
  Arcadia_SizeValue n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, context->source);
  if (offset > n) offset = n;
  diagnostic->sourceName = context->sourceName;
  diagnostic->line = 1;
  diagnostic->column = 1;
  ((Arcadia_Languages_Diagnostic*)diagnostic)->inputPosition = Arcadia_Value_makeSizeValue(offset);
  for (Arcadia_SizeValue i = 0; i < offset;) {
    if (bytes[i] == '\r' || bytes[i] == '\n') {
      unsigned char other = bytes[i++] == '\r' ? '\n' : '\r';
      while (i < offset && bytes[i] == other) ++i;
      ++diagnostic->line;
      diagnostic->column = 1;
    } else {
      if ((bytes[i] & 0xc0) != 0x80) ++diagnostic->column;
      ++i;
    }
  }
}

void
Arcadia_VPL_Text_fail
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_SizeValue offset,
    Arcadia_Status status,
    const char* message
  )
{
  Arcadia_String* text = (Arcadia_String*)Arcadia_VPL_Text_own(thread, context, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, message));
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)Arcadia_VPL_Text_own(thread, context,
    (Arcadia_Object*)Arcadia_VPL_Diagnostics_VerificationDiagnostic_create(thread, text, NULL));
  Arcadia_VPL_Text_locate(thread, context, diagnostic, offset);
  Arcadia_Languages_Diagnostics_add(thread, context->diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_Thread_setStatus(thread, status);
  Arcadia_Thread_jump(thread);
}
