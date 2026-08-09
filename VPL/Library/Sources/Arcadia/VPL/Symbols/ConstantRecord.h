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

#if !defined(ARCADIA_VPL_SYMBOLS_CONSTANTBLOCK_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_CONSTANTBLOCK_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Constant.h"
#include "Arcadia/VPL/Tree/Include.h"
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

// A constant record. Contains members.
// The "constant block" maps to an "uniform block" in OpenGL which can both be accessed from any stage (e.g., vertex, fragment, etc.).
// An uniform block cannot be modified by the stages.
//
// ```
// 'constant' 'record' <name>
// '{'
//   <record body>
// '}' ';'
// <record body> : <record member>*
// <record member> : 'member' <name> ':' <type> ';'
// ```
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.ConstantRecord", Arcadia_VPL_Symbols_ConstantRecord,
                          u8"Arcadia.VPL.Symbols.Constant");

struct Arcadia_VPL_Symbols_ConstantRecordDispatch {
  Arcadia_VPL_Symbols_ConstantDispatch _parent;
};

struct Arcadia_VPL_Symbols_ConstantRecord {
  Arcadia_VPL_Symbols_Constant _parent;
  Arcadia_String* name;
  Arcadia_VPL_Symbols_Program* program;
  Arcadia_VPL_Tree_Node* node;

  Arcadia_List* fields;
  /// The mapped name.
  /// This is null initially and computed in later stages.
  Arcadia_String* mappedName;
  /// The scope of this constant record.
  /// This is null initially and computed in later stages.
  Arcadia_Languages_Scope* scope;
};

/// @brief Create a constant block.
/// @param thread A pointer to this thread.
/// @param name The name of the constant block.
/// @param progrma The program owning this constant block.
/// @param node The tree node of the definition of this constant record.
Arcadia_VPL_Symbols_ConstantRecord*
Arcadia_VPL_Symbols_ConstantRecord_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_VPL_Tree_Node* node
  );

/// @brief Get an immutable list of the fields of this constant block.
/// @param thread A pointer to this thread.
/// @param self A pointer to this constant block.
/// @return A pointer to this list.
Arcadia_List*
Arcadia_VPL_Symbols_ConstantRecord_getFields
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_ConstantRecord* self
  );

#endif // ARCADIA_VPL_SYMBOLS_CONSTANTBLOCK_H_INCLUDED
