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

#if !defined(ARCADIA_ENGINE_INPUT_KEYBOARDKEY_H_INCLUDED)
#define ARCADIA_ENGINE_INPUT_KEYBOARDKEY_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// https://michaelheilmann.com/Arcadia/Engine/#Arcadia_Engine_Input_KeyboardKey
Arcadia_declareEnumerationType(u8"Arcadia.Engine.Input.KeyboardKey",
                               Arcadia_Engine_Input_KeyboardKey);

enum Arcadia_Engine_Input_KeyboardKey {

#define withAliases
#define Alias(alias,aliased) Arcadia_Engine_Input_KeyboardKey_##alias = Arcadia_Engine_Input_KeyboardKey_##aliased,
#define Define(name,value,description) Arcadia_Engine_Input_KeyboardKey_##name = value,

#include "Arcadia/Engine/Input/KeyboardKey.i"

#undef Define
#undef Alias
#undef withAliases

};

// https://michaelheilnmann.com/Arcadia/Engine/#Arcadia_Engine_Input_KeyboardKey_toString
Arcadia_String*
Arcadia_Engine_Input_KeyboardKey_toString
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKey self
  );

#endif // ARCADIA_ENGINE_INPUT_KEYBOARDKEY_H_INCLUDED
