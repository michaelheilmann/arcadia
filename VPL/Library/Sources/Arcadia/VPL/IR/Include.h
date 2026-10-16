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

#if !defined(ARCADIA_VPL_IR_INCLUDE_H_INCLUDED)
#define ARCADIA_VPL_IR_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif

#include "Arcadia/VPL/IR/ArrayType.h"
#include "Arcadia/VPL/IR/ArrayUniform.h"
#include "Arcadia/VPL/IR/BinaryInstruction.h"
#include "Arcadia/VPL/IR/Block.h"
#include "Arcadia/VPL/IR/BranchTerminator.h"
#include "Arcadia/VPL/IR/CallInstruction.h"
#include "Arcadia/VPL/IR/CopyInstruction.h"
#include "Arcadia/VPL/IR/ConditionalBranchTerminator.h"
#include "Arcadia/VPL/IR/ConstantValue.h"
#include "Arcadia/VPL/IR/ConstructInstruction.h"
#include "Arcadia/VPL/IR/DeclareLocalInstruction.h"
#include "Arcadia/VPL/IR/DiscardInstruction.h"
#include "Arcadia/VPL/IR/DiscardTerminator.h"
#include "Arcadia/VPL/IR/Field.h"
#include "Arcadia/VPL/IR/Function.h"
#include "Arcadia/VPL/IR/IndexInstruction.h"
#include "Arcadia/VPL/IR/Instruction.h"
#include "Arcadia/VPL/IR/LoadInstruction.h"
#include "Arcadia/VPL/IR/MatrixType.h"
#include "Arcadia/VPL/IR/MemberInstruction.h"
#include "Arcadia/VPL/IR/Module.h"
#include "Arcadia/VPL/IR/Verifier.h"
#include "Arcadia/VPL/IR/RecordType.h"
#include "Arcadia/VPL/IR/RecordUniform.h"
#include "Arcadia/VPL/IR/ReferenceValue.h"
#include "Arcadia/VPL/IR/MemberReferenceValue.h"
#include "Arcadia/VPL/IR/IndexReferenceValue.h"
#include "Arcadia/VPL/IR/RegisterValue.h"
#include "Arcadia/VPL/IR/ReturnTerminator.h"
#include "Arcadia/VPL/IR/SamplerType.h"
#include "Arcadia/VPL/IR/ScalarType.h"
#include "Arcadia/VPL/IR/ScalarUniform.h"
#include "Arcadia/VPL/IR/Stage.h"
#include "Arcadia/VPL/IR/StageIO.h"
#include "Arcadia/VPL/IR/StoreInstruction.h"
#include "Arcadia/VPL/IR/SwizzleInstruction.h"
#include "Arcadia/VPL/IR/Terminator.h"
#include "Arcadia/VPL/IR/Type.h"
#include "Arcadia/VPL/IR/Uniform.h"
#include "Arcadia/VPL/IR/Value.h"
#include "Arcadia/VPL/IR/VectorType.h"
#include "Arcadia/VPL/IR/VoidType.h"

#endif // ARCADIA_VPL_IR_INCLUDE_H_INCLUDED
