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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Unicode/isUTF8.h"

#include "Arcadia/Ring1/Implementation/Unicode/UTF8ArrayIterator.h"

Arcadia_BooleanValue
Arcadia_Unicode_isUTF8
  (
    Arcadia_Thread* thread,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes,
    Arcadia_SizeValue* numberOfSymbols
  )
{
  _Arcadia_UTF8ArrayIterator it;
  _Arcadia_UTF8ArrayIterator_initialize(thread, &it, bytes, numberOfBytes);
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    while (_Arcadia_UTF8ArrayIterator_hasCodePoint(thread, &it)) {
      _Arcadia_UTF8ArrayIterator_next(thread, &it);
    }
    Arcadia_BooleanValue result = !_Arcadia_UTF8ArrayIterator_hasError(thread, &it);
    if (numberOfSymbols) *numberOfSymbols = _Arcadia_UTF8ArrayIterator_getCodePointIndex(thread, &it);
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
    Arcadia_Thread_jump(thread);
  }
}
