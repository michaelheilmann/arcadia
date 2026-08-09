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

#if !defined(ARCADIA_MILC_WORDTYPE_H_INCLUDED)
#define ARCADIA_MILC_WORDTYPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType("Arcadia.MILC.WordType", Arcadia_MILC_WordType);

enum Arcadia_MILC_WordType {

  // Start of input.
  Arcadia_MILC_WordType_StartOfInput,
  // End of input.
  Arcadia_MILC_WordType_EndOfInput,

  // LF, CR or CR LF
  Arcadia_MILC_WordType_LineTerminator,
  // Any non-empty sequence of the symbols tabulator and whitespace.
  Arcadia_MILC_WordType_WhiteSpaces,

  // The `Equals Sign` / `=`  / U+003D.
  Arcadia_MILC_WordType_EqualsSign,
  // The `Colon` / `:` / U+003A.
  Arcadia_MILC_WordType_Colon,
  // The `Comma` / `,` / U+002C.
  Arcadia_MILC_WordType_Comma,
  // The `Left Curly Bracket` / `{` / U+007B.
  Arcadia_MILC_WordType_LeftCurlyBracket,
  // The `Right Curly Bracket`/ `}` / U+007D.
  Arcadia_MILC_WordType_RightCurlyBracket,
  // The `Left Parenthesis` / `(` / U+0028.
  Arcadia_MILC_WordType_LeftParenthesis,
  // The `Right Parenthesis` / `)` / U+0029.
  Arcadia_MILC_WordType_RightParenthesis,
  // The `Period` / `.` / U+002E.
  Arcadia_MILC_WordType_Period,


  // A name.
  Arcadia_MILC_WordType_Name,
  // A register.
  Arcadia_MILC_WordType_Register,

  // A boolean literal.
  // "true" or "false".
  Arcadia_MILC_WordType_BooleanLiteral,
  // A real number literal.
  Arcadia_MILC_WordType_RealLiteral,
  // An integer literal.
  Arcadia_MILC_WordType_IntegerLiteral,
  // A string literal.
  Arcadia_MILC_WordType_StringLiteral,
  // A void literal.
  // "void".
  Arcadia_MILC_WordType_VoidLiteral,

  // "class"
  Arcadia_MILC_WordType_Class,
  // "constructor"
  Arcadia_MILC_WordType_Constructor,
  // "enumeration"
  Arcadia_MILC_WordType_Enumeration,
  // "extends"
  Arcadia_MILC_WordType_Extends,
  // "implements"
  Arcadia_MILC_WordType_Implements,
  // "procedure"
  Arcadia_MILC_WordType_Procedure,
  // "method"
  Arcadia_MILC_WordType_Method,
  // "module",
  Arcadia_MILC_WordType_Module,

  // "invoke"
  Arcadia_MILC_WordType_Invoke,

  // "native"
  Arcadia_MILC_WordType_Native,
  // "entry"
  Arcadia_MILC_WordType_Entry,
  // "return"
  Arcadia_MILC_WordType_Return,
  // "variable"
  Arcadia_MILC_WordType_Variable,

  // "jump"
  Arcadia_MILC_WordType_Jump,
  // "jumpIfTrue"
  Arcadia_MILC_WordType_JumpIfTrue,
  // "jumpIfFalse"
  Arcadia_MILC_WordType_JumpIfFalse,

  // "raise"
  Arcadia_MILC_WordType_Raise,

  // "add" (arithmetic operations)
  Arcadia_MILC_WordType_Add,
  // "subtract" (arithmetic operations)
  Arcadia_MILC_WordType_Subtract,
  // "multiply" (arithmetic operations)
  Arcadia_MILC_WordType_Multiply,
  // "divide" (arithmetic operations)
  Arcadia_MILC_WordType_Divide,

  // "isEqualTo" (relational operation)
  Arcadia_MILC_WordType_IsEqualTo,
  // "isNotEqualTo" (relational operation),
  Arcadia_MILC_WordType_IsNotEqualTo,
  // "isLowerThan" (relational operation)
  Arcadia_MILC_WordType_IsLowerThan,
  // "isLowerThanOrEqualTo" (relational operation)
  Arcadia_MILC_WordType_IsLowerThanOrEqualTo,
  // "isGreaterThan" (relational operation)
  Arcadia_MILC_WordType_IsGreaterThan,
  // "isGreaterThanOrEqualTo" (relational operation)
  Arcadia_MILC_WordType_IsGreaterThanOrEqualTo,

  // "negate" (arithmetic operations/logical operations)
  Arcadia_MILC_WordType_Negate,

  // "not" (boolean operations)
  Arcadia_MILC_WordType_Not,
  // "and" (boolean operations)
  Arcadia_MILC_WordType_And,
  // "or" (boolean operations)
  Arcadia_MILC_WordType_Or,

  // "concatenate" (list operations)
  Arcadia_MILC_WordType_Concatenate,

  // "set"
  Arcadia_MILC_WordType_Set,

  /// A single line comment.
  Arcadia_MILC_WordType_SingleLineComment,
  /// A multi line comment.
  Arcadia_MILC_WordType_MultiLineComment,

};

#endif // ARCADIA_MILC_WORDTYPE_H_INCLUDED
