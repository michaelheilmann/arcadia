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

#if !defined(ARCADIA_PDLC_WORDTYPE_H_INCLUDED)
#define ARCADIA_PDLC_WORDTYPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType("Arcadia.PDLC.WordType", Arcadia_PDLC_WordType);

enum Arcadia_PDLC_WordType {

  // Start of input.
  Arcadia_PDLC_WordType_StartOfInput,
  // End of input.
  Arcadia_PDLC_WordType_EndOfInput,

  // LF, CR or CR LF
  Arcadia_PDLC_WordType_LineTerminator,
  // A reverse solidus followed by a line terminator.
  Arcadia_PDLC_WordType_LineContinuation,
  // Any non-empty sequence of the symbols tabulator and whitespace.
  Arcadia_PDLC_WordType_WhiteSpaces,

  // The `Equals Sign` / `=`  / U+003D.
  Arcadia_PDLC_WordType_EqualsSign,
  // The `Colon` / `:` / U+003A.
  Arcadia_PDLC_WordType_Colon,
  // The `Comma` / `,` / U+002C.
  Arcadia_PDLC_WordType_Comma,
  // The `Left Curly Bracket` / `{` / U+007B.
  Arcadia_PDLC_WordType_LeftCurlyBracket,
  // The `Right Curly Bracket`/ `}` / U+007D.
  Arcadia_PDLC_WordType_RightCurlyBracket,
  // The `Left Parenthesis` / `(` / U+0028.
  Arcadia_PDLC_WordType_LeftParenthesis,
  // The `Right Parenthesis` / `)` / U+0029.
  Arcadia_PDLC_WordType_RightParenthesis,
  // The `Period` / `.` / U+002E.
  Arcadia_PDLC_WordType_Period,


  // A name.
  Arcadia_PDLC_WordType_Name,
  // A register.
  Arcadia_PDLC_WordType_Register,

  // A boolean literal.
  // "true" or "false".
  Arcadia_PDLC_WordType_BooleanLiteral,
  // A real number literal.
  Arcadia_PDLC_WordType_RealLiteral,
  // An integer literal.
  Arcadia_PDLC_WordType_IntegerLiteral,
  // A string literal.
  Arcadia_PDLC_WordType_StringLiteral,
  // A void literal.
  // "void".
  Arcadia_PDLC_WordType_VoidLiteral,

  // "class"
  Arcadia_PDLC_WordType_Class,
  // "constructor"
  Arcadia_PDLC_WordType_Constructor,
  // "enumeration"
  Arcadia_PDLC_WordType_Enumeration,
  // "extends"
  Arcadia_PDLC_WordType_Extends,
  // "implements"
  Arcadia_PDLC_WordType_Implements,
  // "interface"
  Arcadia_PDLC_WordType_Interface,
  // "procedure"
  Arcadia_PDLC_WordType_Procedure,
  // "method"
  Arcadia_PDLC_WordType_Method,
  // "module",
  Arcadia_PDLC_WordType_Module,

  // "invoke"
  Arcadia_PDLC_WordType_Invoke,
  // "construct"
  Arcadia_PDLC_WordType_Construct,

  // "native"
  Arcadia_PDLC_WordType_Native,
  // "entry"
  Arcadia_PDLC_WordType_Entry,
  // "return"
  Arcadia_PDLC_WordType_Return,
  // "variable"
  Arcadia_PDLC_WordType_Variable,

  // "jump"
  Arcadia_PDLC_WordType_Jump,
  // "jumpIfTrue"
  Arcadia_PDLC_WordType_JumpIfTrue,
  // "jumpIfFalse"
  Arcadia_PDLC_WordType_JumpIfFalse,

  // "loadArgument"
  Arcadia_PDLC_WordType_LoadArgument,
  // "storeArgument
  Arcadia_PDLC_WordType_StoreArgument,
  // "loadField"
  Arcadia_PDLC_WordType_LoadField,
  // "storeField"
  Arcadia_PDLC_WordType_StoreField,

  // "raise"
  Arcadia_PDLC_WordType_Raise,

  // "add" (arithmetic operations)
  Arcadia_PDLC_WordType_Add,
  // "subtract" (arithmetic operations)
  Arcadia_PDLC_WordType_Subtract,
  // "multiply" (arithmetic operations)
  Arcadia_PDLC_WordType_Multiply,
  // "divide" (arithmetic operations)
  Arcadia_PDLC_WordType_Divide,

  // "isEqualTo" (relational operation)
  Arcadia_PDLC_WordType_IsEqualTo,
  // "isNotEqualTo" (relational operation),
  Arcadia_PDLC_WordType_IsNotEqualTo,
  // "isLowerThan" (relational operation)
  Arcadia_PDLC_WordType_IsLowerThan,
  // "isLowerThanOrEqualTo" (relational operation)
  Arcadia_PDLC_WordType_IsLowerThanOrEqualTo,
  // "isGreaterThan" (relational operation)
  Arcadia_PDLC_WordType_IsGreaterThan,
  // "isGreaterThanOrEqualTo" (relational operation)
  Arcadia_PDLC_WordType_IsGreaterThanOrEqualTo,

  // "negate" (arithmetic operations/logical operations)
  Arcadia_PDLC_WordType_Negate,

  // "not" (boolean operations)
  Arcadia_PDLC_WordType_Not,
  // "and" (boolean operations)
  Arcadia_PDLC_WordType_And,
  // "or" (boolean operations)
  Arcadia_PDLC_WordType_Or,

  // "concatenate" (list operations)
  Arcadia_PDLC_WordType_Concatenate,

  // "set"
  Arcadia_PDLC_WordType_Set,

  /// A single line comment.
  Arcadia_PDLC_WordType_SingleLineComment,
  /// A multi line comment.
  Arcadia_PDLC_WordType_MultiLineComment,
  // "call": explicit procedure invocation, including an empty argument list.
  Arcadia_PDLC_WordType_Call,
  // "abstract"
  Arcadia_PDLC_WordType_Abstract,

};

#endif // ARCADIA_PDLC_WORDTYPE_H_INCLUDED
