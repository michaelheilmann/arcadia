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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Input/KeyboardKey.h"

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
};

Arcadia_defineEnumerationType(u8"Arcadia.Engine.Input.KeyboardKey", Arcadia_Engine_Input_KeyboardKey,
                              &_typeOperations);

Arcadia_String*
Arcadia_Engine_Input_KeyboardKey_toString
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKey self
  )
{
  switch (self) {
  /*#define withAliases*/
  #define Alias(alias, aliased)
  #define Define(name, value, description)\
    case Arcadia_Engine_Input_KeyboardKey_##name: {\
      return Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, description, sizeof(description) - 1)); \
    };
  #include "Arcadia/Engine/Input/KeyboardKey.i"
  #undef Define
  #undef Alias
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    };
  };
}
