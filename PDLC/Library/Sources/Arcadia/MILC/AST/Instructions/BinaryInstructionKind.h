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

#if !defined(ARCADIA_MILC_AST_BINARYINSTRUCTIONKIND_H_INCLUDED)
#define ARCADIA_MILC_AST_BINARYINSTRUCTIONKIND_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.MILC.AST.BinaryInstructionKind", Arcadia_MILC_AST_BinaryInstructionKind);

enum Arcadia_MILC_AST_BinaryInstructionKind {

  /// @code
  /// addInstruction : 'add' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Add = 1,

  /// @code
  /// andInstruction : 'and' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_And = 2,

  /// @code
  /// concatenateInstruction : 'concatenate' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Concatenate = 3,

  /// @code
  /// divideInstruction : 'divide' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Divide = 4,

  /// @code
  /// multiplyInstruction : 'multiply' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Multiply = 5,

  /// @code
  /// orInstruction : 'or' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Or = 6,

  /// @code
  /// subtractInstruction : 'subtract' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_Subtract = 7,

  /// @code
  /// isEqualToInstruction : 'isEqualTo' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsEqualTo = 8,

  /// @code
  /// isEqualToInstruction : 'isNotEqualTo' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsNotEqualTo = 9,

  /// @code
  /// isLowerThanInstruction : 'isLowerThan' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsLowerThan = 10,

  /// @code
  /// isLowerThanOrEqualToInstruction : 'isLowerThanOrEqualTo' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsLowerThanOrEqualTo = 11,

  /// @code
  /// isGreaterThanInstruction : 'isGreaterThan' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsGreaterThan = 12,

  /// @code
  /// isGreaterThanOrEqualToInstruction : 'isGreaterThanOrEqualTo' target, firstOperand, secondOperand
  /// target : register
  /// firstOperand : register
  /// secondOperand : register
  /// @endcode
  Arcadia_MILC_AST_BinaryInstructionKind_IsGreaterThanOrEqualTo = 13,

};

#endif // ARCADIA_MILC_AST_BINARYINSTRUCTIONKIND_H_INCLUDED
