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

#if !defined(ARCADIA_TEMPLATEENGINE_ENVIRONMENT_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_ENVIRONMENT_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Environment", Environment,
                          u8"Arcadia.Object");

struct EnvironmentDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Environment {
  Arcadia_Object _parent;
  /// The enclosing environment or null.
  Environment* enclosing;
  /// Map from strings to strings.
  /// The key string is the variable name, the value string is the variable value.
  Arcadia_Map* variables;
};

Environment*
Environment_create
  (
    Arcadia_Thread* thread,
    Environment* enclosing
  );

Environment*
Environment_loadString
  (
    Arcadia_Thread* thread,
    Arcadia_String* source
  );

/// @brief
/// Get the value of  variable.
/// @param thread
/// A pointer to this thread.
/// @param self
/// A pointer to this environment.
/// @param key
/// The name of the variable (a string value).
/// @param recursive
/// #Arcadia_BooleanValue_True: Search the environments from this environments outwards until the variable is found.
/// #Arcadia_BooleanValue_True: Search this (only) environment.
/// @return
/// The variable value (a string value) if the variable was found, a void value otherwise.
Arcadia_Value
Environment_get
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_BooleanValue recursive
  );

void
Environment_set
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_Value value
  );

#endif // ARCADIA_TEMPLATEENGINE_ENVIRONMENT_H_INCLUDED
