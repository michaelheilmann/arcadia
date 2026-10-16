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
#include "Arcadia/VPL/IR/Include.h"
#include "Arcadia/VPL/IR/OutputVerification.h"
#include <string.h>

typedef struct FunctionEffects {
  unsigned char* requires;
  unsigned char* ensures;
  unsigned char state;
  Arcadia_BooleanValue normalReturn;
} FunctionEffects;

typedef struct OutputAnalysis {
  Arcadia_Thread* thread;
  Arcadia_VPL_IR_Verifier* verifier;
  Arcadia_VPL_IR_Stage* stage;
  Arcadia_SizeValue leaves;
  Arcadia_SizeValue functions;
  Arcadia_SizeValue* offsets;
  FunctionEffects* effects;
  unsigned char* summaries;
  unsigned char* flow;
} OutputAnalysis;

static Arcadia_SizeValue
length
  (
    OutputAnalysis* a,
    Arcadia_List* list
  )
{
  return Arcadia_Collection_getSize(a->thread, (Arcadia_Collection*)list);
}

static Arcadia_Object*
at
  (
    OutputAnalysis* a,
    Arcadia_List* list,
    Arcadia_SizeValue i
  )
{
  return Arcadia_List_getObjectReferenceValueAt(a->thread, list, i);
}

static void
error
  (
    OutputAnalysis* a,
    Arcadia_Object* subject,
    const char* message
  )
{
  _Arcadia_VPL_IR_raiseVerificationError(a->thread, a->verifier, subject, message);
}

static Arcadia_SizeValue
addSize
  (
    OutputAnalysis* a,
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  )
{
  if (x > ((Arcadia_SizeValue)-1) - y) error(a, (Arcadia_Object*)a->stage, "verification error: output analysis is too large\n");
  return x + y;
}

static Arcadia_SizeValue
multiplySize
  (
    OutputAnalysis* a,
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  )
{
  if (y && x > ((Arcadia_SizeValue)-1) / y) error(a, (Arcadia_Object*)a->stage, "verification error: output analysis is too large\n");
  return x * y;
}

static Arcadia_SizeValue
leafCount
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_Type* type
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) return multiplySize(a, ((Arcadia_VPL_IR_ArrayType*)type)->count, leafCount(a, ((Arcadia_VPL_IR_ArrayType*)type)->element));
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_SizeValue count = 0;
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = length(a, fields); i < n; ++i) count = addSize(a, count, leafCount(a, ((Arcadia_VPL_IR_Field*)at(a, fields, i))->type));
    return count;
  }
  return 1;
}

static Arcadia_VPL_IR_ReferenceValue*
parentReference
  (
    Arcadia_VPL_IR_ReferenceValue* ref
  )
{
  if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Member) return ((Arcadia_VPL_IR_MemberReferenceValue*)ref)->base;
  if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Index) return ((Arcadia_VPL_IR_IndexReferenceValue*)ref)->base;
  return NULL;
}

static void
selectLeaves
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_Type* type,
    Arcadia_SizeValue offset,
    Arcadia_VPL_IR_ReferenceValue* ref,
    Arcadia_SizeValue depth,
    unsigned char* mask,
    Arcadia_BooleanValue* dynamic
  )
{
  if (!leafCount(a, type)) return;
  if (!depth) {
    memset(mask + offset, 1, leafCount(a, type));
    return;
  }
  Arcadia_VPL_IR_ReferenceValue* selector = ref;
  for (Arcadia_SizeValue i = 1; i < depth; ++i) selector = parentReference(selector);
  if (selector->storageKind == Arcadia_VPL_IR_StorageKind_Member) {
    Arcadia_String* name = ((Arcadia_VPL_IR_MemberReferenceValue*)selector)->fieldName;
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = length(a, fields); i < n; ++i) {
      Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)at(a, fields, i);
      if (Arcadia_String_isEqualTo_pn(a->thread, name, Arcadia_String_getBytes(a->thread, field->name), Arcadia_String_getNumberOfBytes(a->thread, field->name))) {
        selectLeaves(a, field->type, offset, ref, depth - 1, mask, dynamic);
        return;
      }
      offset += leafCount(a, field->type);
    }
  } else {
    Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
    Arcadia_VPL_IR_Value* index = ((Arcadia_VPL_IR_IndexReferenceValue*)selector)->index;
    Arcadia_SizeValue width = leafCount(a, array->element);
    if (index->kind == Arcadia_VPL_IR_ValueKind_Constant) {
      Arcadia_SizeValue i = (Arcadia_SizeValue)Arcadia_Value_getInteger32Value(&((Arcadia_VPL_IR_ConstantValue*)index)->literal);
      selectLeaves(a, array->element, offset + i * width, ref, depth - 1, mask, dynamic);
    } else {
      *dynamic = Arcadia_BooleanValue_True;
      for (Arcadia_SizeValue i = 0; i < array->count; ++i) selectLeaves(a, array->element, offset + i * width, ref, depth - 1, mask, dynamic);
    }
  }
}

static Arcadia_BooleanValue
outputMask
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_ReferenceValue* ref,
    unsigned char* mask,
    Arcadia_BooleanValue* dynamic
  )
{
  memset(mask, 0, a->leaves);
  *dynamic = Arcadia_BooleanValue_False;
  Arcadia_SizeValue depth = 0;
  Arcadia_VPL_IR_ReferenceValue* root = ref;
  while (parentReference(root)) { root = parentReference(root); ++depth; }
  if (root->storageKind != Arcadia_VPL_IR_StorageKind_StageIO) return Arcadia_BooleanValue_False;
  Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(a, a->stage->interface, root->slot);
  if (io->direction != Arcadia_VPL_IR_StageIO_Direction_Out) return Arcadia_BooleanValue_False;
  selectLeaves(a, io->type, a->offsets[root->slot], ref, depth, mask, dynamic);
  return Arcadia_BooleanValue_True;
}

static Arcadia_SizeValue
functionIndex
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_Function* function
  )
{
  for (Arcadia_SizeValue i = 0; i < a->functions; ++i) if (at(a, a->stage->functions, i) == (Arcadia_Object*)function) return i;
  error(a, (Arcadia_Object*)function, "verification error: output analysis found a foreign callee\n");
  return 0;
}

static Arcadia_BooleanValue
effects
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_Instruction* instruction,
    unsigned char* assigned,
    unsigned char* required,
    unsigned char* mask
  )
{
  Arcadia_BooleanValue dynamic;
  if (instruction->kind == Arcadia_VPL_IR_InstructionKind_Load) {
    if (required && outputMask(a, ((Arcadia_VPL_IR_LoadInstruction*)instruction)->source, mask, &dynamic)) {
      for (Arcadia_SizeValue i = 0; i < a->leaves; ++i) if (mask[i] && !assigned[i]) required[i] = 1;
    }
  } else if (instruction->kind == Arcadia_VPL_IR_InstructionKind_Store) {
    if (outputMask(a, ((Arcadia_VPL_IR_StoreInstruction*)instruction)->target, mask, &dynamic) && !dynamic) {
      for (Arcadia_SizeValue i = 0; i < a->leaves; ++i) assigned[i] |= mask[i];
    }
  } else if (instruction->kind == Arcadia_VPL_IR_InstructionKind_Call && ((Arcadia_VPL_IR_CallInstruction*)instruction)->calleeFunction) {
    FunctionEffects* callee = &a->effects[functionIndex(a, ((Arcadia_VPL_IR_CallInstruction*)instruction)->calleeFunction)];
    for (Arcadia_SizeValue i = 0; i < a->leaves; ++i) {
      if (required && callee->requires[i] && !assigned[i]) required[i] = 1;
      if (callee->normalReturn) assigned[i] |= callee->ensures[i];
    }
    return callee->normalReturn;
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
blockEffects
  (
    OutputAnalysis* a,
    Arcadia_VPL_IR_Block* block,
    unsigned char* assigned,
    unsigned char* required,
    unsigned char* mask
  )
{
  for (Arcadia_SizeValue i = 0, n = length(a, block->instructions); i < n; ++i) {
    if (!effects(a, (Arcadia_VPL_IR_Instruction*)at(a, block->instructions, i), assigned, required, mask)) return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
edge
  (
    Arcadia_VPL_IR_Block* from,
    Arcadia_VPL_IR_Block* to
  )
{
  Arcadia_VPL_IR_Terminator* term = from->terminator;
  if (term->kind == Arcadia_VPL_IR_TerminatorKind_Branch) return ((Arcadia_VPL_IR_BranchTerminator*)term)->target == to;
  if (term->kind == Arcadia_VPL_IR_TerminatorKind_ConditionalBranch) return ((Arcadia_VPL_IR_ConditionalBranchTerminator*)term)->trueTarget == to || ((Arcadia_VPL_IR_ConditionalBranchTerminator*)term)->falseTarget == to;
  return Arcadia_BooleanValue_False;
}

static void
analyzeFunction
  (
    OutputAnalysis* a,
    Arcadia_SizeValue index
  )
{
  FunctionEffects* summary = &a->effects[index];
  Arcadia_VPL_IR_Function* function = (Arcadia_VPL_IR_Function*)at(a, a->stage->functions, index);
  if (summary->state == 2) return;
  if (summary->state == 1) error(a, (Arcadia_Object*)function, "verification error: recursive calls are unsupported\n");
  summary->state = 1;
  Arcadia_SizeValue b = length(a, function->blocks), l = a->leaves;
  // Resolve callees before allocating this function's scratch buffer.
  for (Arcadia_SizeValue q = 0; q < b; ++q) {
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)at(a, function->blocks, q);
    for (Arcadia_SizeValue i = 0, n = length(a, block->instructions); i < n; ++i) {
      Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(a, block->instructions, i);
      if (base->kind == Arcadia_VPL_IR_InstructionKind_Call && ((Arcadia_VPL_IR_CallInstruction*)base)->calleeFunction) analyzeFunction(a, functionIndex(a, ((Arcadia_VPL_IR_CallInstruction*)base)->calleeFunction));
    }
  }
  Arcadia_SizeValue bytes = addSize(a, multiplySize(a, b, addSize(a, multiplySize(a, l, 2), 2)), multiplySize(a, l, 2));
  a->flow = (unsigned char*)Arcadia_Memory_allocateUnmanaged(a->thread, bytes);
  unsigned char* in = a->flow;
  unsigned char* out = in + b * l;
  unsigned char* reachable = out + b * l;
  unsigned char* end = reachable + b;
  unsigned char* work = end + b;
  unsigned char* mask = work + l;
  memset(reachable, 0, b);
  for (Arcadia_SizeValue q = 0; q < b; ++q) {
    memset(work, 0, l);
    end[q] = blockEffects(a, (Arcadia_VPL_IR_Block*)at(a, function->blocks, q), work, NULL, mask);
  }
  reachable[0] = 1;
  Arcadia_BooleanValue changed;
  do {
    changed = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue p = 0; p < b; ++p) {
      if (!reachable[p] || !end[p]) continue;
      for (Arcadia_SizeValue q = 0; q < b; ++q) {
        if (!reachable[q] && edge((Arcadia_VPL_IR_Block*)at(a, function->blocks, p), (Arcadia_VPL_IR_Block*)at(a, function->blocks, q))) { reachable[q] = 1; changed = Arcadia_BooleanValue_True; }
      }
    }
  } while (changed);
  memset(in, 1, b * l);
  memset(out, 1, b * l);
  memset(in, 0, l);
  memset(out, 0, l);
  blockEffects(a, (Arcadia_VPL_IR_Block*)at(a, function->blocks, 0), out, NULL, mask);
  do {
    changed = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue q = 0; q < b; ++q) {
      if (!reachable[q]) continue;
      memset(work, q ? 1 : 0, l);
      if (q) {
        for (Arcadia_SizeValue p = 0; p < b; ++p) {
          if (reachable[p] && end[p] && edge((Arcadia_VPL_IR_Block*)at(a, function->blocks, p), (Arcadia_VPL_IR_Block*)at(a, function->blocks, q))) {
            for (Arcadia_SizeValue j = 0; j < l; ++j) work[j] &= out[p * l + j];
          }
        }
      }
      if (memcmp(in + q * l, work, l)) { memcpy(in + q * l, work, l); changed = Arcadia_BooleanValue_True; }
      blockEffects(a, (Arcadia_VPL_IR_Block*)at(a, function->blocks, q), work, NULL, mask);
      if (memcmp(out + q * l, work, l)) { memcpy(out + q * l, work, l); changed = Arcadia_BooleanValue_True; }
    }
  } while (changed);
  memset(summary->requires, 0, l);
  memset(summary->ensures, 1, l);
  summary->normalReturn = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue q = 0; q < b; ++q) {
    if (!reachable[q]) continue;
    memcpy(work, in + q * l, l);
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)at(a, function->blocks, q);
    Arcadia_BooleanValue continuation = blockEffects(a, block, work, summary->requires, mask);
    if (continuation && block->terminator->kind == Arcadia_VPL_IR_TerminatorKind_Return) {
      summary->normalReturn = Arcadia_BooleanValue_True;
      for (Arcadia_SizeValue j = 0; j < l; ++j) summary->ensures[j] &= work[j];
    }
  }
  if (!summary->normalReturn) memset(summary->ensures, 0, l);
  summary->state = 2;
  Arcadia_Map_set(a->thread, a->verifier->normalReturns, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)function), Arcadia_Value_makeBooleanValue(summary->normalReturn), NULL, NULL);
  Arcadia_Memory_deallocateUnmanaged(a->thread, a->flow);
  a->flow = NULL;
}

static void
dispose
  (
    OutputAnalysis* a
  )
{
  if (a->flow) Arcadia_Memory_deallocateUnmanaged(a->thread, a->flow);
  if (a->summaries) Arcadia_Memory_deallocateUnmanaged(a->thread, a->summaries);
  if (a->effects) Arcadia_Memory_deallocateUnmanaged(a->thread, a->effects);
  if (a->offsets) Arcadia_Memory_deallocateUnmanaged(a->thread, a->offsets);
  Arcadia_Memory_deallocateUnmanaged(a->thread, a);
}

static void
analyzeStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Stage* stage
  )
{
  OutputAnalysis* a = (OutputAnalysis*)Arcadia_Memory_allocateUnmanaged(thread, sizeof(OutputAnalysis));
  memset(a, 0, sizeof(OutputAnalysis));
  a->thread = thread;
  a->verifier = self;
  a->stage = stage;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_SizeValue n = length(a, stage->interface);
    a->offsets = (Arcadia_SizeValue*)Arcadia_Memory_allocateUnmanaged(thread, multiplySize(a, addSize(a, n, 1), sizeof(Arcadia_SizeValue)));
    Arcadia_BooleanValue primary = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue i = 0; i < n; ++i) {
      a->offsets[i] = a->leaves;
      Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(a, stage->interface, i);
      if (io->direction != Arcadia_VPL_IR_StageIO_Direction_Out) continue;
      if (io->semantics == (stage->kind == Arcadia_VPL_IR_Stage_Vertex ? Arcadia_VPL_IR_StageIO_Semantics_Position : Arcadia_VPL_IR_StageIO_Semantics_FragmentColor)) primary = Arcadia_BooleanValue_True;
      a->leaves = addSize(a, a->leaves, leafCount(a, io->type));
    }
    a->offsets[n] = a->leaves;
    if (!primary) error(a, (Arcadia_Object*)stage, "verification error: stage lacks its required Position or FragmentColor output\n");
    a->functions = length(a, stage->functions);
    a->effects = (FunctionEffects*)Arcadia_Memory_allocateUnmanaged(thread, multiplySize(a, a->functions, sizeof(FunctionEffects)));
    memset(a->effects, 0, a->functions * sizeof(FunctionEffects));
    a->summaries = (unsigned char*)Arcadia_Memory_allocateUnmanaged(thread, multiplySize(a, multiplySize(a, a->functions, a->leaves), 2));
    for (Arcadia_SizeValue i = 0; i < a->functions; ++i) {
      a->effects[i].requires = a->summaries + i * 2 * a->leaves;
      a->effects[i].ensures = a->effects[i].requires + a->leaves;
    }
    for (Arcadia_SizeValue i = 0; i < a->functions; ++i) analyzeFunction(a, i);
    FunctionEffects* entry = &a->effects[functionIndex(a, stage->entryFunction)];
    for (Arcadia_SizeValue i = 0; i < a->leaves; ++i) {
      if (entry->requires[i]) error(a, (Arcadia_Object*)stage->entryFunction, "verification error: stage entry reads an output before definite assignment\n");
      if (entry->normalReturn && !entry->ensures[i]) error(a, (Arcadia_Object*)stage->entryFunction, "verification error: stage entry does not assign every output on normal return\n");
    }
    self->stage = stage;
    for (Arcadia_SizeValue i = 0; i < a->functions; ++i) _Arcadia_VPL_IR_verifyFunctionFlow(thread, self, (Arcadia_VPL_IR_Function*)at(a, stage->functions, i));
    dispose(a);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    dispose(a);
    Arcadia_Thread_jump(thread);
  }
}

void
_Arcadia_VPL_IR_verifyOutputs
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  analyzeStage(thread, self, self->module->vertex);
  analyzeStage(thread, self, self->module->fragment);
}
