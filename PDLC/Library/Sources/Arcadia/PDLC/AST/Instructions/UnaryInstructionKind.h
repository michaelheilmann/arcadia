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

#if !defined(ARCADIA_PDLC_AST_UNARYINSTRUCTIONKIND_H_INCLUDED)
#define ARCADIA_PDLC_AST_UNARYINSTRUCTIONKIND_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.PDLC.AST.UnaryInstructionKind", Arcadia_PDLC_AST_UnaryInstructionKind);

enum Arcadia_PDLC_AST_UnaryInstructionKind {

  /// @code
  /// negateInstruction : 'negate' target ',' operand
  /// target : register
  /// operand : operand
  /// @endcode
  Arcadia_PDLC_AST_UnaryInstructionKind_Negate = 1,

  /// @code
  /// notInstruction : 'not' target ',' operand
  /// target : register
  /// operand : operand
  /// @endcode
  Arcadia_PDLC_AST_UnaryInstructionKind_Not = 2,

  /// @code
  /// setInstruction : 'set' target ',' operand
  /// target : register
  /// operand : operand
  /// @endcode
  Arcadia_PDLC_AST_UnaryInstructionKind_Set = 3,

};

#endif // ARCADIA_PDLC_AST_UNARYINSTRUCTIONKIND_H_INCLUDED
