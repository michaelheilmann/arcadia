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

#if !defined(ARCADIA_ENGINE_APPLICATIONCONFIGURATION_H_INCLUDED)
#define ARCADIA_ENGINE_APPLICATIONCONFIGURATION_H_INCLUDED

#include "Arcadia/DDL/Include.h"

// { <name[0]> = { <name[1]> : { <name[2]> : ... { <name[n-1]> : <value>} } } }
// node must be a map node
void
Arcadia_Engine_ApplicationConfiguration_setBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_Integer32Value value
  );

// node must be a map node
// errors: not exists or conversion failed
Arcadia_BooleanValue
Arcadia_Engine_ApplicationConfiguration_getBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  );

// { <name[0]> = { <name[1]> : { <name[2]> : ... { <name[n-1]> : <value>} } } }
// node must be a map node
void
Arcadia_Engine_ApplicationConfiguration_setInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_Integer32Value value
  );

// node must be a map node
// errors: not exists or conversion failed
Arcadia_Integer32Value
Arcadia_Engine_ApplicationConfiguration_getInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  );

// { <name[0]> = { <name[1]> : { <name[2]> : ... { <name[n-1]> : <value>} } } }
// node must be a map node
void
Arcadia_Engine_ApplicationConfiguration_setString
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames,
    Arcadia_String* value
  );

// node must be a map node
// errors: not exists or conversion failed
Arcadia_String*
Arcadia_Engine_ApplicationConfiguration_getString
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  );

// { <name[0]> = { <name[1]> : { <name[2]> : ... { <name[n-1]> : { } } } } }
// node must be a map node
void
Arcadia_Engine_ApplicationConfiguration_setSection
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* node,
    char const* names[],
    Arcadia_SizeValue numberOfNames
  );

void
Arcadia_Engine_ApplicationConfiguration_saveConfiguration
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration
  );

Arcadia_DDL_MapNode*
Arcadia_Engine_ApplicationConfiguration_loadConfiguration
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_APPLICATIONCONFIGURATION_H_INCLUDED
