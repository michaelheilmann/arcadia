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

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Tests/AST/Bases.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(ARCADIA_PDLC_PRIVATE)
  #error("the public PDLC master must not leak an implementation-private switch")
#endif

struct BaseModel { Arcadia_PDLC_AST_Node _parent; Arcadia_SizeValue startOffset; };
_Static_assert(sizeof(Arcadia_PDLC_AST_OperandNode) == sizeof(struct BaseModel), "operand size");
_Static_assert(_Alignof(Arcadia_PDLC_AST_OperandNode) == _Alignof(struct BaseModel), "operand alignment");
_Static_assert(offsetof(Arcadia_PDLC_AST_OperandNode, _parent) == 0, "operand parent offset");
_Static_assert(offsetof(Arcadia_PDLC_AST_OperandNode, startOffset) == offsetof(struct BaseModel, startOffset), "operand field offset");
_Static_assert(sizeof(Arcadia_PDLC_AST_StatementNode) == sizeof(struct BaseModel), "statement size");
_Static_assert(_Alignof(Arcadia_PDLC_AST_StatementNode) == _Alignof(struct BaseModel), "statement alignment");
_Static_assert(offsetof(Arcadia_PDLC_AST_StatementNode, _parent) == 0, "statement parent offset");
_Static_assert(offsetof(Arcadia_PDLC_AST_StatementNode, startOffset) == offsetof(struct BaseModel, startOffset), "statement field offset");
_Static_assert(sizeof(Arcadia_PDLC_AST_OperandNodeDispatch) == sizeof(Arcadia_PDLC_AST_NodeDispatch), "operand dispatch");
_Static_assert(sizeof(Arcadia_PDLC_AST_StatementNodeDispatch) == sizeof(Arcadia_PDLC_AST_NodeDispatch), "statement dispatch");
_Static_assert(_Generic(((Arcadia_PDLC_AST_OperandNode*)0)->startOffset, Arcadia_SizeValue: 1, default: 0), "operand Size domain");
_Static_assert(_Generic(((Arcadia_PDLC_AST_StatementNode*)0)->startOffset, Arcadia_SizeValue: 1, default: 0), "statement Size domain");

typedef struct OperandBaseProbe OperandBaseProbe;
typedef struct OperandBaseProbeDispatch OperandBaseProbeDispatch;
typedef struct StatementBaseProbe StatementBaseProbe;
typedef struct StatementBaseProbeDispatch StatementBaseProbeDispatch;

struct OperandBaseProbe { Arcadia_PDLC_AST_OperandNode _parent; Arcadia_String* payload; };
struct OperandBaseProbeDispatch { Arcadia_PDLC_AST_OperandNodeDispatch _parent; };
struct StatementBaseProbe { Arcadia_PDLC_AST_StatementNode _parent; Arcadia_String* payload; };
struct StatementBaseProbeDispatch { Arcadia_PDLC_AST_StatementNodeDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.OperandBaseProbe", OperandBaseProbe,
                          u8"Arcadia.PDLC.AST.OperandNode");
Arcadia_declareObjectType(u8"Tests.StatementBaseProbe", StatementBaseProbe,
                          u8"Arcadia.PDLC.AST.StatementNode");

static unsigned destroyedOperands, destroyedStatements;

static void
constructOperandProbe
  (
    Arcadia_Thread* thread,
    OperandBaseProbe* self
  )
{
  Arcadia_EnterConstructor(OperandBaseProbe);
  Arcadia_Value offset = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_ValueStack_pushValue(thread, &offset);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->payload = Arcadia_String_createFromCxxString(thread, "operand payload");
  Arcadia_LeaveConstructor(OperandBaseProbe);
}

static void
constructStatementProbe
  (
    Arcadia_Thread* thread,
    StatementBaseProbe* self
  )
{
  Arcadia_EnterConstructor(StatementBaseProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_PDLC_AST_StatementNode*)self)->startOffset == 0);
  self->payload = Arcadia_String_createFromCxxString(thread, "statement payload");
  Arcadia_LeaveConstructor(StatementBaseProbe);
}

static void
visitOperandProbe
  (
    Arcadia_Thread* thread,
    OperandBaseProbe* self
  )
{ if (self->payload) Arcadia_Object_visit(thread, (Arcadia_Object*)self->payload); }

static void
visitStatementProbe
  (
    Arcadia_Thread* thread,
    StatementBaseProbe* self
  )
{ if (self->payload) Arcadia_Object_visit(thread, (Arcadia_Object*)self->payload); }

static void
destructOperandProbe
  (
    Arcadia_Thread* thread,
    OperandBaseProbe* self
  )
{ destroyedOperands++; }

static void
destructStatementProbe
  (
    Arcadia_Thread* thread,
    StatementBaseProbe* self
  )
{ destroyedStatements++; }

static const Arcadia_ObjectType_Operations operandObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructOperandProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitOperandProbe,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructOperandProbe,
};
static const Arcadia_Type_Operations operandOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &operandObjects };
static const Arcadia_ObjectType_Operations statementObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructStatementProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitStatementProbe,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructStatementProbe,
};
static const Arcadia_Type_Operations statementOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &statementObjects };

Arcadia_defineObjectType(u8"Tests.OperandBaseProbe", OperandBaseProbe,
                         u8"Arcadia.PDLC.AST.OperandNode", Arcadia_PDLC_AST_OperandNode,
                         &operandOperations);
Arcadia_defineObjectType(u8"Tests.StatementBaseProbe", StatementBaseProbe,
                         u8"Arcadia.PDLC.AST.StatementNode", Arcadia_PDLC_AST_StatementNode,
                         &statementOperations);

static Arcadia_Object*
create
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
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value const* arguments,
    Arcadia_Natural8Value count,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    create(thread, type, arguments, count);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  }
}

static void
testConstructors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_TypeValue operandType = _Arcadia_PDLC_AST_OperandNode_getType(thread);
  Arcadia_TypeValue statementType = _Arcadia_PDLC_AST_StatementNode_getType(thread);
  Arcadia_Name* operandName = Arcadia_Type_getName(thread, operandType);
  Arcadia_Name* statementName = Arcadia_Type_getName(thread, statementType);
  const char operandText[] = "Arcadia.PDLC.AST.OperandNode";
  const char statementText[] = "Arcadia.PDLC.AST.StatementNode";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, operandName) == sizeof(operandText) - 1 &&
    !memcmp(Arcadia_Name_getBytes(thread, operandName), operandText, sizeof(operandText) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, statementName) == sizeof(statementText) - 1 &&
    !memcmp(Arcadia_Name_getBytes(thread, statementName), statementText, sizeof(statementText) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, operandName) == operandType && Arcadia_getType(thread, statementName) == statementType);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, operandType) == Arcadia_TypeKind_Object && Arcadia_Type_getKind(thread, statementType) == Arcadia_TypeKind_Object);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, operandType, _Arcadia_PDLC_AST_Node_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, statementType, _Arcadia_PDLC_AST_Node_getType(thread)));
  Arcadia_SizeValue offsets[] = { 0, 1, 255, 256, Arcadia_SizeValue_Maximum };
  for (unsigned i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
    Arcadia_Value argument = Arcadia_Value_makeSizeValue(offsets[i]);
    Arcadia_PDLC_AST_OperandNode* operand = (void*)create(thread, operandType, &argument, 1);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, operand);
    Arcadia_Tests_assertTrue(thread, operand->startOffset == offsets[i] && Arcadia_Object_getType(thread, (Arcadia_Object*)operand) == operandType);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_PDLC_AST_StatementNode* statement = (void*)create(thread, statementType, NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, statement);
  Arcadia_Tests_assertTrue(thread, statement->startOffset == 0 && Arcadia_Object_getType(thread, (Arcadia_Object*)statement) == statementType);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Value arguments[255];
  for (unsigned i = 0; i < 255; ++i) arguments[i] = Arcadia_Value_makeSizeValue(17);
  for (unsigned count = 0; count < 256; ++count) {
    if (count != 1) expectFailure(thread, operandType, arguments, (Arcadia_Natural8Value)count, Arcadia_Status_NumberOfArgumentsInvalid);
    if (count != 0) expectFailure(thread, statementType, arguments, (Arcadia_Natural8Value)count, Arcadia_Status_NumberOfArgumentsInvalid);
  }
  Arcadia_String* string = Arcadia_String_createFromCxxString(thread, "wrong offset");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, string);
  Arcadia_Value invalid[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeNatural16Value(0),
    Arcadia_Value_makeNatural32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeReal32Value(0), Arcadia_Value_makeReal64Value(0),
    Arcadia_Value_makeTypeValue(operandType), Arcadia_Value_makeObjectReferenceValue(string),
  };
  for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) expectFailure(thread, operandType, &invalid[i], 1, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  for (unsigned i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, i) == 77);
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testNativeDescendants
  (
    Arcadia_Thread* thread
  )
{
  destroyedOperands = destroyedStatements = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Value argument = Arcadia_Value_makeSizeValue(Arcadia_SizeValue_Maximum);
  OperandBaseProbe* operand = (void*)create(thread, _OperandBaseProbe_getType(thread), &argument, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, operand);
  StatementBaseProbe* statement = (void*)create(thread, _StatementBaseProbe_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, statement);
  ((Arcadia_PDLC_AST_StatementNode*)statement)->startOffset = 12345;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !destroyedOperands && !destroyedStatements);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_PDLC_AST_OperandNode*)operand)->startOffset == Arcadia_SizeValue_Maximum && ((Arcadia_PDLC_AST_StatementNode*)statement)->startOffset == 12345);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, operand->payload, "operand payload", sizeof("operand payload") - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, statement->payload, "statement payload", sizeof("statement payload") - 1));
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyedOperands == 1 && destroyedStatements == 1 && Arcadia_ValueStack_getSize(thread) == base);
}

static void
testCompilerConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_PDLC_AST_VariableOperandNode* operand = Arcadia_PDLC_AST_VariableOperandNode_create(thread, Arcadia_SizeValue_Maximum,
    Arcadia_String_createFromCxxString(thread, "compiler consumer"));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, operand);
  Arcadia_PDLC_AST_ReturnInstructionNode* instruction = Arcadia_PDLC_AST_ReturnInstructionNode_create(thread, 37, (void*)operand);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, instruction);
  // Instruction factories currently leave the statement base's offset at zero.
  // Migrating these bases must not silently change that separate legacy policy.
  Arcadia_Tests_assertTrue(thread, ((Arcadia_PDLC_AST_StatementNode*)instruction)->startOffset == 0);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, instruction);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, instruction->operand == (Arcadia_PDLC_AST_OperandNode*)operand && ((Arcadia_PDLC_AST_OperandNode*)operand)->startOffset == Arcadia_SizeValue_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, operand->value, "compiler consumer", sizeof("compiler consumer") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_PDLC_Tests_AST_bases
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testConstructors)) { fprintf(stderr, "AST base constructors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testNativeDescendants)) { fprintf(stderr, "AST base descendants failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testCompilerConsumers)) { fprintf(stderr, "AST base consumers failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
