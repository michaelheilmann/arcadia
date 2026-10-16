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

#include "Arcadia/PDLC/EnterPhase.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/MemberEnterPhase/ClassCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/EnumerationCompleter.h"
#include "Arcadia/PDLC/Symbols/Symbols.h"
#include "Arcadia/PDLC/Environment.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhaseDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  );

static Arcadia_String*
onIdentifierNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_IdentifierNode* identifierNode
  );

static void
onClassDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ClassDefinitionNode* classDefinitionNode
  );

static void
onInterfaceDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_InterfaceDefinitionNode* interfaceDefinitionNode
  );

static void
onModuleDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleDefinitionNode* moduleDefinitionNode
  );

static void
onModuleDefinitionNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleDefinitionNode* moduleDefinitionNode,
    Arcadia_BooleanValue isModuleFile
  );

static void
onEnumerationDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_EnumerationDefinitionNode* enumerationDefinitionNode
  );

static void
onProcedureDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ProcedureDefinitionNode* procedureDefinitionNode
  );

static void
onCompilationUnitNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode,
    Arcadia_BooleanValue isModuleFile
  );

static void
onModuleNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleNode* moduleNode
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.EnterPhase", Arcadia_PDLC_EnterPhase,
                         u8"Arcadia.PDLC.AST.Visitor", Arcadia_PDLC_AST_Visitor,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_EnterPhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->lastSymbol = NULL;
  self->deque = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);
  self->context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Context_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_EnterPhase);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhaseDispatch* self
  )
{
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitClassDefinitionNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ClassDefinitionNode*)) & onClassDefinitionNode;
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitEnumerationDefinitionNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_EnumerationDefinitionNode*)) & onEnumerationDefinitionNode;
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitInterfaceDefinitionNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_InterfaceDefinitionNode*)) & onInterfaceDefinitionNode;
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitModuleDefinitionNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ModuleDefinitionNode*)) & onModuleDefinitionNode;
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitModuleNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ModuleNode*)) & onModuleNode;
  ((Arcadia_PDLC_AST_VisitorDispatch*)self)->visitProcedureDefinitionNode = (void(*)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ProcedureDefinitionNode*)) & onProcedureDefinitionNode;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  )
{
  if (self->deque) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->deque);
  }
  if (self->lastSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->lastSymbol);
  }
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
}

Arcadia_PDLC_EnterPhase*
Arcadia_PDLC_EnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_EnterPhase);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_EnterPhase);
}

Arcadia_PDLC_EnterPhase*
Arcadia_PDLC_EnterPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_PDLC_EnterPhase_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_EnterPhase_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_PDLC_EnterPhase*)Arcadia_Value_getObjectReferenceValue(&v);
}

static Arcadia_String*
onIdentifierNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_IdentifierNode* identifierNode
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_String* name = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)identifierNode->names, 0, _Arcadia_String_getType(thread));
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, name);
  for (Arcadia_SizeValue i = 1, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)identifierNode->names); i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '.');
    name = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)identifierNode->names, i, _Arcadia_String_getType(thread));
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, name);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

static void
enterTopLevelSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode,
    Arcadia_PDLC_ModuleSymbol* moduleSymbol,
    Arcadia_String* symbolName,
    Arcadia_PDLC_Symbol* symbol
  )
{
  if (Arcadia_Languages_Scope_contains(thread, moduleSymbol->scope, symbolName, Arcadia_BooleanValue_False) ||
      Arcadia_Languages_Scope_contains(thread, self->context->scope, symbolName, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread, self->context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, self->context->inputFileManager, Arcadia_FilePath_toNative(thread, compilationUnitNode->filePath, Arcadia_BooleanValue_False), compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            symbolName
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, moduleSymbol->scope, symbolName, (Arcadia_Object*)symbol);
    Arcadia_Languages_Scope_enter(thread, self->context->scope, symbolName, (Arcadia_Object*)symbol);
  }
}

static void
onClassDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ClassDefinitionNode* classDefinitionNode
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_PDLC_AST_DefinitionNode*)classDefinitionNode)->compilationUnitNode;
  assert(NULL != compilationUnitNode);
  Arcadia_PDLC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;
  assert(NULL != moduleNode);
  Arcadia_PDLC_ModuleSymbol* moduleSymbol = moduleNode->moduleSymbol;
  assert(NULL != moduleSymbol);

  Arcadia_String* symbolName = onIdentifierNode(thread, self, classDefinitionNode->className);
  Arcadia_PDLC_ClassSymbol* symbol = Arcadia_PDLC_ClassSymbol_create(thread, symbolName);
  ((Arcadia_PDLC_Symbol*)symbol)->enclosing = (Arcadia_PDLC_Symbol*)moduleSymbol;
  symbol->ast = classDefinitionNode;
  symbol->isNative = classDefinitionNode->isNative;
  symbol->isNativeImplementation = classDefinitionNode->isNativeImplementation;
  symbol->isAbstract = classDefinitionNode->isAbstract;
  Arcadia_PDLC_Environment* environment = Arcadia_PDLC_Environment_create(thread, (Arcadia_PDLC_Symbol*)symbol, compilationUnitNode);
  Arcadia_Map_set(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
                                                       Arcadia_Value_makeObjectReferenceValue(environment), NULL, NULL);
  if (!moduleSymbol->scope) {
    moduleSymbol->scope = Arcadia_Languages_Scope_create(thread, self->context->scope);
  }
  if (!symbol->scope) {
    symbol->scope = Arcadia_Languages_Scope_create(thread, moduleSymbol->scope);
  }
  enterTopLevelSymbol(thread, self, compilationUnitNode, moduleSymbol, symbolName, (Arcadia_PDLC_Symbol*)symbol);
}

static void
onInterfaceDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_InterfaceDefinitionNode* interfaceDefinitionNode
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_PDLC_AST_DefinitionNode*)interfaceDefinitionNode)->compilationUnitNode;
  assert(NULL != compilationUnitNode);
  Arcadia_PDLC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;
  assert(NULL != moduleNode);
  Arcadia_PDLC_ModuleSymbol* moduleSymbol = moduleNode->moduleSymbol;
  assert(NULL != moduleSymbol);

  Arcadia_String* symbolName = onIdentifierNode(thread, self, interfaceDefinitionNode->interfaceName);
  Arcadia_PDLC_InterfaceSymbol* symbol = Arcadia_PDLC_InterfaceSymbol_create(thread, symbolName);
  ((Arcadia_PDLC_Symbol*)symbol)->enclosing = (Arcadia_PDLC_Symbol*)moduleSymbol;
  symbol->ast = interfaceDefinitionNode;
  Arcadia_PDLC_Environment* environment = Arcadia_PDLC_Environment_create(thread, (Arcadia_PDLC_Symbol*)symbol, compilationUnitNode);
  Arcadia_Map_set(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
                                                       Arcadia_Value_makeObjectReferenceValue(environment), NULL, NULL);
  if (!moduleSymbol->scope) {
    moduleSymbol->scope = Arcadia_Languages_Scope_create(thread, self->context->scope);
  }
  if (!symbol->scope) {
    symbol->scope = Arcadia_Languages_Scope_create(thread, moduleSymbol->scope);
  }
  enterTopLevelSymbol(thread, self, compilationUnitNode, moduleSymbol, symbolName, (Arcadia_PDLC_Symbol*)symbol);
}

static void
onModuleDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleDefinitionNode* moduleDefinitionNode
  )
{
  onModuleDefinitionNodeImpl(thread, self, moduleDefinitionNode, Arcadia_BooleanValue_False);
}

static void
onModuleDefinitionNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleDefinitionNode* moduleDefinitionNode,
    Arcadia_BooleanValue isModuleFile
  )
{
  Arcadia_String* moduleSymbolName = onIdentifierNode(thread, self, moduleDefinitionNode->moduleName);
  // Get the compilation unit node.
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_PDLC_AST_DefinitionNode*)moduleDefinitionNode)->compilationUnitNode;
  assert(NULL != compilationUnitNode);
  // Get the module node.
  Arcadia_PDLC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;
  assert(NULL != moduleNode);
  if (!isModuleFile || self->lastSymbol) {
    // LANGUAGE DEFINITION: Assert not more than one module definition exists in Module.pdl. A module
    // definition outside Module.pdl also violates this rule because Module.pdl is the only module
    // definition file.
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        self->context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_MultipleModuleDefinitionsDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_toNative(thread, compilationUnitNode->filePath, Arcadia_BooleanValue_False), compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0)
          )
      );
    return;
  }
  Arcadia_PDLC_ModuleSymbol* moduleSymbol = Arcadia_PDLC_ModuleSymbol_create(thread, moduleSymbolName);
  moduleNode->moduleSymbol = moduleSymbol;
  moduleSymbol->scope = Arcadia_Languages_Scope_create(thread, self->context->scope);
  moduleSymbol->moduleDefinitionNode = moduleDefinitionNode;
  Arcadia_PDLC_Environment* environment = Arcadia_PDLC_Environment_create(thread, (Arcadia_PDLC_Symbol*)moduleSymbol, compilationUnitNode);
  Arcadia_Map_set(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(moduleSymbol),
                                                       Arcadia_Value_makeObjectReferenceValue(environment), NULL, NULL);
  // Module names are unique independently of type/procedure names.
  if (Arcadia_Languages_Scope_contains(thread, self->context->moduleScope, moduleSymbolName, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        self->context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, self->context->inputFileManager,
              Arcadia_FilePath_toNative(thread, compilationUnitNode->filePath, Arcadia_BooleanValue_False), compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            moduleSymbolName
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, self->context->moduleScope, moduleSymbolName, (Arcadia_Object*)moduleSymbol);
  }
  self->lastSymbol = moduleSymbol;
}

static void
onEnumerationDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_EnumerationDefinitionNode* enumerationDefinitionNode
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_PDLC_AST_DefinitionNode*)enumerationDefinitionNode)->compilationUnitNode;
  assert(NULL != compilationUnitNode);
  Arcadia_PDLC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;
  assert(NULL != moduleNode);
  Arcadia_PDLC_ModuleSymbol* moduleSymbol = moduleNode->moduleSymbol;
  assert(NULL != moduleSymbol);

  Arcadia_String* symbolName = onIdentifierNode(thread, self, enumerationDefinitionNode->enumerationName);
  Arcadia_PDLC_EnumerationSymbol* symbol = Arcadia_PDLC_EnumerationSymbol_create(thread, symbolName);
  ((Arcadia_PDLC_Symbol*)symbol)->enclosing = (Arcadia_PDLC_Symbol*)moduleSymbol;
  symbol->ast = enumerationDefinitionNode;
  symbol->isNative = enumerationDefinitionNode->isNative;
  Arcadia_PDLC_Environment* environment = Arcadia_PDLC_Environment_create(thread, (Arcadia_PDLC_Symbol*)symbol, compilationUnitNode);
  Arcadia_Map_set(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
                                                       Arcadia_Value_makeObjectReferenceValue(environment), NULL, NULL);

  if (!moduleSymbol->scope) {
    moduleSymbol->scope = Arcadia_Languages_Scope_create(thread, self->context->scope);
  }
  if (!symbol->scope) {
    symbol->scope = Arcadia_Languages_Scope_create(thread, moduleSymbol->scope);
  }
  enterTopLevelSymbol(thread, self, compilationUnitNode, moduleSymbol, symbolName, (Arcadia_PDLC_Symbol*)symbol);
}

static void
onProcedureDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ProcedureDefinitionNode* procedureDefinitionNode
  )
{
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_PDLC_AST_DefinitionNode*)procedureDefinitionNode)->compilationUnitNode;
  assert(NULL != compilationUnitNode);
  Arcadia_PDLC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;
  assert(NULL != moduleNode);
  Arcadia_PDLC_ModuleSymbol* moduleSymbol = moduleNode->moduleSymbol;
  assert(NULL != moduleSymbol);
  Arcadia_String* symbolName = onIdentifierNode(thread, self, procedureDefinitionNode->name);
  Arcadia_PDLC_ProcedureSymbol* symbol = Arcadia_PDLC_ProcedureSymbol_create(thread, symbolName);
  ((Arcadia_PDLC_Symbol*)symbol)->enclosing = (Arcadia_PDLC_Symbol*)moduleSymbol;
  symbol->ast = procedureDefinitionNode;
  Arcadia_PDLC_Environment* environment = Arcadia_PDLC_Environment_create(thread, (Arcadia_PDLC_Symbol*)symbol, compilationUnitNode);
  Arcadia_Map_set(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
                                                       Arcadia_Value_makeObjectReferenceValue(environment), NULL, NULL);

  if (!moduleSymbol->scope) {
    moduleSymbol->scope = Arcadia_Languages_Scope_create(thread, self->context->scope);
  }
  enterTopLevelSymbol(thread, self, compilationUnitNode, moduleSymbol, symbolName, (Arcadia_PDLC_Symbol*)symbol);
}

static void
onDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_DefinitionNode* definitionNode,
    Arcadia_BooleanValue isModuleFile
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definitionNode, _Arcadia_PDLC_AST_ModuleDefinitionNode_getType(thread))) {
    onModuleDefinitionNodeImpl(thread, self, (Arcadia_PDLC_AST_ModuleDefinitionNode*)definitionNode, isModuleFile);
  } else {
    Arcadia_Deque_insertBackObjectReferenceValue(thread, self->deque, (Arcadia_Object*)definitionNode);
  }
}

static void
onCompilationUnitNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode,
    Arcadia_BooleanValue isModuleFile
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)compilationUnitNode->definitions); i < n; ++i) {
    Arcadia_PDLC_AST_DefinitionNode* definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)Arcadia_List_getObjectReferenceValueAt(thread, compilationUnitNode->definitions, i);
    onDefinitionNode(thread, self, definitionNode, isModuleFile);
  }
}

static void
onModuleNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self,
    Arcadia_PDLC_AST_ModuleNode* moduleNode
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)moduleNode->compilationUnits); i < n; ++i) {
    Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode = (Arcadia_PDLC_AST_CompilationUnitNode*)Arcadia_List_getObjectReferenceValueAt(thread, moduleNode->compilationUnits, i);
    Arcadia_FilePath* moduleFilePath = Arcadia_FilePath_clone(thread, moduleNode->moduleDirectoryPath);
    Arcadia_FilePath_append(thread, moduleFilePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Module.pdl")));
    onCompilationUnitNode(thread, self, compilationUnitNode, Arcadia_FilePath_isEqualTo(thread, compilationUnitNode->filePath, moduleFilePath));
  }
}

void
Arcadia_PDLC_EnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_EnterPhase* self
  )
{
  Arcadia_PDLC_Context* context = self->context;
  if (!context->scope) {
    context->scope = Arcadia_Languages_Scope_create(thread, NULL);
  } else {
    Arcadia_Languages_Scope_clear(thread, context->scope);
  }
  if (!context->moduleScope) {
    context->moduleScope = Arcadia_Languages_Scope_create(thread, NULL);
  } else {
    Arcadia_Languages_Scope_clear(thread, context->moduleScope);
  }
  // Re-register built-ins after every reset, including repeated phase runs.
  Arcadia_PDLC_Symbols* symbols = Arcadia_PDLC_Symbols_getInstance(thread, context);
  Arcadia_List* builtIns = Arcadia_PDLC_Symbols_getBuiltIns(thread, symbols);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)builtIns); i < n; ++i) {
    Arcadia_PDLC_Symbol* builtIn = (Arcadia_PDLC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, builtIns, i, _Arcadia_PDLC_Symbol_getType(thread));
    Arcadia_Languages_Scope_enter(thread, context->scope, builtIn->name, (Arcadia_Object*)builtIn);
  }
  // (1)
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->context->moduleNodes); i < n; ++i) {
    self->lastSymbol = NULL;
    Arcadia_PDLC_AST_ModuleNode* moduleNode = (Arcadia_PDLC_AST_ModuleNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->context->moduleNodes, i);
    onModuleNode(thread, self, moduleNode);
    if (!self->lastSymbol) {
      // LANGUAGE DEFINITION: Assert exactly one module definition exists in Module.pdl.
      Arcadia_FilePath* moduleFilePath = Arcadia_FilePath_clone(thread, moduleNode->moduleDirectoryPath);
      Arcadia_FilePath_append(thread, moduleFilePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Module.pdl")));
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_MissingModuleDefinitionDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_toNative(thread, moduleFilePath, Arcadia_BooleanValue_False), moduleFilePath)
            )
        );
    }
    self->lastSymbol = NULL;
  }
  if (Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics)) {
    return;
  }
  // (3)
  while (!Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)self->deque)) {
    Arcadia_Value temporary = Arcadia_Deque_getFront(thread, self->deque);
    Arcadia_PDLC_AST_DefinitionNode* definitionNode = (Arcadia_PDLC_AST_DefinitionNode*)Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_PDLC_AST_DefinitionNode_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definitionNode, _Arcadia_PDLC_AST_ClassDefinitionNode_getType(thread))) {
      onClassDefinitionNode(thread, self, (Arcadia_PDLC_AST_ClassDefinitionNode*)definitionNode);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definitionNode, _Arcadia_PDLC_AST_EnumerationDefinitionNode_getType(thread))) {
      onEnumerationDefinitionNode(thread, self, (Arcadia_PDLC_AST_EnumerationDefinitionNode*)definitionNode);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definitionNode, _Arcadia_PDLC_AST_InterfaceDefinitionNode_getType(thread))) {
      onInterfaceDefinitionNode(thread, self, (Arcadia_PDLC_AST_InterfaceDefinitionNode*)definitionNode);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definitionNode, _Arcadia_PDLC_AST_ProcedureDefinitionNode_getType(thread))) {
      onProcedureDefinitionNode(thread, self, (Arcadia_PDLC_AST_ProcedureDefinitionNode*)definitionNode);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Deque_removeFront(thread, self->deque);
  }
}
