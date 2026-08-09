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

#if !defined(ARCADIA_TEMPLATEENGINE_PARSER_TREE_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_PARSER_TREE_H_INCLUDED

#include "Arcadia.TemplateEngine/Parser/TreeType.h"

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Tree", Directives_Tree,
                          u8"Arcadia.Object");

struct Directives_TreeDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Directives_Tree {
  Arcadia_Object _parent;

  /// The the type of this tree.
  Arcadia_TemplateEngine_TreeType type;

  // Arcadia_TemplateEngine_TreeType_File.
  struct {
    Arcadia_List* children;
  } file;

  // Arcadia_TemplateEngine_TreeType_InvokeExpr.
  struct {
    Arcadia_String* target;
    Arcadia_List* arguments;
  } invokeExpr;

  // Arcadia_TemplateEngine_TreeType_NameExpr.
  struct {
    Arcadia_String* name;
  } nameExpr;

  // Arcadia_TemplateEngine_TreeType_StringLiteralExpr.
  struct {
    Arcadia_String* string;
  } stringLiteralExpr;

  // Arcadia_TemplateEngine_TreeType_Text.
  struct {
    Arcadia_String* text;
  } text;

};

Directives_Tree*
Directives_Tree_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value type,
    Arcadia_String* name
  );

Directives_Tree*
Directives_Tree_createAtLiteralExpr
  (
    Arcadia_Thread* thread
  );

Directives_Tree*
Directives_Tree_createFile
  (
    Arcadia_Thread* thread
  );

Directives_Tree*
Directives_Tree_createInvokeExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_List* arguments
  );

Directives_Tree*
Directives_Tree_createNameExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

Directives_Tree*
Directives_Tree_createStringLiteralExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  );

Directives_Tree*
Directives_Tree_createText
  (
    Arcadia_Thread* thread,
    Arcadia_String* text
  );

#endif // ARCADIA_TEMPLATEENGINE_PARSER_TREE_H_INCLUDED
