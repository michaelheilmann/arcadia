// The author of this software is Michael Heilmann (contact@michaelheilmann.com).
//
// Copyright(c) 2024-2025 Michael Heilmann (contact@michaelheilmann.com).
//
// Permission to use, copy, modify, and distribute this software for any
// purpose without fee is hereby granted, provided that this entire notice
// is included in all copies of any software which is or includes a copy
// or modification of this software and in all copies of the supporting
// documentation for such software.
//
// THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY.IN PARTICULAR, NEITHER THE AUTHOR NOR LUCENT MAKES ANY
// REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
// OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.

// Start of input.
Define(StartOfInput, "<start of input>")
// End of input.
Define(EndOfInput, "<end of input>")

// LF, CR or CR LF
Define(LineTerminator, "<line terminator>")
// Any non-empty sequence of the symbols tabulator and whitespace.
Define(WhiteSpace, "<white space>")

// The `Colon` / `:` / U+003A.
Define(Colon, "<colon>")
// The `Comma` / `,` / U+002C.
Define(Comma, "<comma>")

// The `Left Curly Bracket` / `{` / U+007B.
Define(LeftCurlyBracket, "<left curly bracket>")
// The `Right Curly Bracket`/ `}` / U+007D.
Define(RightCurlyBracket, "<right curly bracket>")

// The `Left Square Bracket` / `[` / U+005B.
Define(LeftSquareBracket, "<left square bracket>")
// The `Right Square Bracket`/ `]` / U+005D.
Define(RightSquareBracket, "<right square bracket>")

// A name.
Define(Name, "<name>")

// A boolean literal.
// "true" or "false".
Define(BooleanLiteral, "<boolean literal>")
// A real number literal.
Define(RealLiteral, "<real literal>")
// An integer literal.
Define(IntegerLiteral, "<integer literal>")
// A string literal.
Define(StringLiteral, "<string literal>")
// A void literal.
// "void".
Define(VoidLiteral, "<void literal>")

/// A single line comment.
Define(SingleLineComment, "<single line comment>")
/// A multi line comment.
Define(MultiLineComment, "<multi line comment>")
