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

#if !defined(ARCADIA_DDL_SEMANTICALANALYSIS_H_INCLUDED)
#define ARCADIA_DDL_SEMANTICALANALYSIS_H_INCLUDED

#include "Arcadia/DDL/Nodes/Include.h"

/// @code
/// class Arcadia.DataDefinitionLanguage.SemanticalAnalysis
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DataDefinitionLanguage.SemanticalAnalysis", Arcadia_DataDefinitionLanguage_SemanticalAnalysis,
                          u8"Arcadia.Object");

/// @brief Create a Data Definition Language semantical analysis.
/// @param thread A pointer to the thread.
/// @return A pointer to this thread.
Arcadia_DataDefinitionLanguage_SemanticalAnalysis*
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create
  (
    Arcadia_Thread* thread
  );

/// @brief Run this Arcadia Data Definition Language semantical analysis on a node.
/// @param thread A pointer to the thread.
/// @param self A pointer to this Data Definition Language semantical analysis.
/// @param node A pointer to the node.
/// @error Arcadia_Status_SemanticalError a map contains two entries with the same key
void
Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* self,
    Arcadia_DDL_Node* node
  );

#endif // ARCADIA_DDL_SEMANTICALANALYSIS_H_INCLUDED
