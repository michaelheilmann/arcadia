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

#include "Arcadia.TemplateEngine/Parser/Tree.h"

static void
Directives_Tree_destruct
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  );

static void
Directives_Tree_visit
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  );

static void
Directives_Tree_constructImpl
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  );

static void
Directives_Tree_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Directives_TreeDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Directives_Tree_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Directives_Tree_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Directives_Tree_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Directives_Tree_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Tree", Directives_Tree,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Directives_Tree_destruct
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  )
{/*Intentionally empty.*/}

static void
Directives_Tree_visit
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  )
{
  switch (self->type) {
    case Arcadia_TemplateEngine_TreeType_AtLiteralExpr: {
      /* Intentionally empty. */
    } break;
    case Arcadia_TemplateEngine_TreeType_File: {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->file.children);
    } break;
    case Arcadia_TemplateEngine_TreeType_InvokeExpr: {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->invokeExpr.target);
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->invokeExpr.arguments);
    } break;
    case Arcadia_TemplateEngine_TreeType_NameExpr: {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->nameExpr.name);
    } break;
    case Arcadia_TemplateEngine_TreeType_StringLiteralExpr: {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringLiteralExpr.string);
    } break;
    case Arcadia_TemplateEngine_TreeType_Text: {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->text.text);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
Directives_Tree_constructImpl
  (
    Arcadia_Thread* thread,
    Directives_Tree* self
  )
{
  Arcadia_EnterConstructor(Directives_Tree);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (_numberOfArguments < 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->type = Arcadia_ValueStack_getInteger32Value(thread, _numberOfArguments);
  switch (self->type) {
    case Arcadia_TemplateEngine_TreeType_AtLiteralExpr: {
      /* Intentionally empty. */
    } break;
    case Arcadia_TemplateEngine_TreeType_File: {
      self->file.children = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    } break;
    case Arcadia_TemplateEngine_TreeType_InvokeExpr: {
      self->invokeExpr.target = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
      self->invokeExpr.arguments = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
    } break;
    case Arcadia_TemplateEngine_TreeType_NameExpr: {
      self->nameExpr.name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
    } break;
    case Arcadia_TemplateEngine_TreeType_StringLiteralExpr: {
      self->stringLiteralExpr.string = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
    } break;
    case Arcadia_TemplateEngine_TreeType_Text: {
      self->text.text = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
    } break;

    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Directives_Tree);
}

static void
Directives_Tree_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Directives_TreeDispatch* self
  )
{/*Intentionally empty.*/}

Directives_Tree*
Directives_Tree_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value type,
    Arcadia_String* name
  )
{
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, type);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createFile
  (
    Arcadia_Thread* thread
  )
{ 
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_File);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createAtLiteralExpr
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_AtLiteralExpr);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createInvokeExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_List* arguments
  )
{
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_InvokeExpr);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)arguments);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createNameExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  )
{
  _Arcadia_BeginCreate();
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_NameExpr);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createStringLiteralExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_StringLiteralExpr);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)string);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Directives_Tree);
}

Directives_Tree*
Directives_Tree_createText
  (
    Arcadia_Thread* thread,
    Arcadia_String* text
  )
{
  _Arcadia_BeginCreate(Directives_Tree);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_TemplateEngine_TreeType_Text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Directives_Tree);
}
