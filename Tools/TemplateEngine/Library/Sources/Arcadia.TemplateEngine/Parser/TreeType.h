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

#if !defined(ARCADIA_TEMPLATEENGINE_PARSER_TREETYPE_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_PARSER_TREETYPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.TemplateEngine.TreeType", Arcadia_TemplateEngine_TreeType);

// <file> : (<text> | <directive>)* 
// <directive> : <expr>           
// <expr> : <name expr>
//        | <string literal expr>
//        | <at literal expr>
//        | <invoke expr>
//
// <name expr> : <NAME>
// <string literal expr> : <STRING>
// 
// <invoke expr> : <NAME> <arguments>
// <arguments> : <argument> <arguments>
//             | e
enum Arcadia_TemplateEngine_TreeType {

  // See <name expr>.
  // Action: The value of the variable denoted by <NAME> is written to the output.
  Arcadia_TemplateEngine_TreeType_NameExpr,
    
  // See <string literal expr>.
  // Action: <STRING> is written to the output.
  Arcadia_TemplateEngine_TreeType_StringLiteralExpr,

  // See <at literal expr>.
  // Action: '@' is written to the output.
  Arcadia_TemplateEngine_TreeType_AtLiteralExpr,
  
  // See <invoke expr>.
  // Action: The procedure denoted by <NAME> is invoked with the arguments <arguments> and its result value is written to the output.
  Arcadia_TemplateEngine_TreeType_InvokeExpr,

  // See <text>.
  // Action: The text is written to the output.
  Arcadia_TemplateEngine_TreeType_Text,

  // See <file>.
  // Action: The children are iterated over from left to right and the action for the respective node is executed.
  Arcadia_TemplateEngine_TreeType_File,

};

#endif // ARCADIA_TEMPLATEENGINE_PARSER_TREETYPE_H_INCLUDED
