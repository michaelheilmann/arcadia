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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/MemberEnterPhase/EnumerationCompleter.h"

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/TypeResolutionPhase.h"
#include "Arcadia/MILC/Environment.h"
#include <assert.h>

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleterDispatch* self
  );

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_visitImpl,

};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.MemberEnterPhase.EnumerationCompleter", Arcadia_MILC_MemberEnterPhase_EnumerationCompleter,
                         u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                         &_typeOperations);

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_MemberEnterPhase_EnumerationCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_MemberEnterPhase_EnumerationCompleter);
}

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleterDispatch* self
  )
{
  ((Arcadia_MILC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_MILC_Completer*, Arcadia_MILC_Context*, Arcadia_MILC_Symbol*)) & completeImpl;
}

static void
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self
  )
{/*Intentionally empty.*/}

static Arcadia_String*
makeFullQualifiedName
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* temporary,
    Arcadia_String* prefix,
    Arcadia_String* suffix
  )
{ 
  Arcadia_StringBuilder_clear(thread, temporary);
  Arcadia_StringBuilder_insertBackString(thread, temporary, prefix);
  Arcadia_StringBuilder_insertBackCxxString(thread, temporary, u8".");
  Arcadia_StringBuilder_insertBackString(thread, temporary, suffix);
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(temporary));
}
static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{ 
  assert(symbol->completer == (Arcadia_MILC_Completer*)self);
  Arcadia_MILC_AST_EnumerationDefinitionNode* node = ((Arcadia_MILC_EnumerationSymbol*)symbol)->ast;
  if (!node) {
    symbol->completer = NULL;
    return;
  }
  Arcadia_MILC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(symbol)),
        _Arcadia_MILC_Environment_getType(thread)
      );
  /* Enter the enumeration members. */
  Arcadia_StringBuilder* temporary = Arcadia_StringBuilder_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->enumerationBody); i < n; ++i) {
    Arcadia_MILC_AST_EnumerationConstantDefinitionNode* childNode = (Arcadia_MILC_AST_EnumerationConstantDefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->enumerationBody, i, _Arcadia_MILC_AST_EnumerationConstantDefinitionNode_getType(thread));
    Arcadia_MILC_EnumerationConstantSymbol* childSymbol = Arcadia_MILC_EnumerationConstantSymbol_create(thread, makeFullQualifiedName(thread, temporary, symbol->name, childNode->name));
    ((Arcadia_MILC_Symbol*)childSymbol)->enclosing = (Arcadia_MILC_Symbol*)symbol;
    assert(((Arcadia_MILC_EnumerationSymbol*)symbol)->scope);
    childSymbol->ast = childNode;
    Arcadia_List_insertBack(thread, ((Arcadia_MILC_EnumerationSymbol*)symbol)->members, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)childSymbol));
    if (Arcadia_Languages_Scope_contains(thread, ((Arcadia_MILC_EnumerationSymbol*)symbol)->scope, ((Arcadia_MILC_Symbol*)childSymbol)->name, Arcadia_BooleanValue_True)) {
      Arcadia_Languages_Diagnostics_add
        (
          thread, context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
              Arcadia_SizeValue_Literal(0),
              ((Arcadia_MILC_Symbol*)childSymbol)->name
            )
        );
    } else {
      Arcadia_Languages_Scope_enter(thread, ((Arcadia_MILC_EnumerationSymbol*)symbol)->scope, ((Arcadia_MILC_Symbol*)childSymbol)->name, (Arcadia_Object*)childSymbol);
    }
    // We add a diagnostic if the child node is missing an initializer. 
    if (!childNode->initializer) {
      Arcadia_Languages_Diagnostics_add
        (
          thread, context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_MILC_Diagnostics_MissingInitializerDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
              Arcadia_SizeValue_Literal(0),
              childSymbol
            )
        );
    }
  }
  symbol->completer = Arcadia_MILC_TypeResolutionPhase_getInstance(thread, context)->enumerationCompleter;
}

Arcadia_MILC_MemberEnterPhase_EnumerationCompleter*
Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_MemberEnterPhase_EnumerationCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_MemberEnterPhase_EnumerationCompleter);
}
