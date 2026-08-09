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
#include "Arcadia/Ring1/Implementation/makeBitmask.h"

#include "Arcadia/Ring1/Include.h"

Arcadia_Natural16Value
Arcadia_makeBitmaskN16
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  )
{
  static const Arcadia_SizeValue width = 16;
  if (i >= width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // "mask" is initially empty.
  Arcadia_Natural8Value mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Arcadia_SizeValue j = i, m = i + n; j < m; ++j) {
    mask |= UINT16_C(1) << (width - j - 1);
  }
  return mask;
}

Arcadia_Natural32Value
Arcadia_makeBitmaskN32
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  )
{
  static const Arcadia_SizeValue width = 32;
  if (i >= width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // "mask" is initially empty.
  Arcadia_Natural8Value mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Arcadia_SizeValue j = i, m = i + n; j < m; ++j) {
    mask |= UINT32_C(1) << (width - j - 1);
  }
  return mask;
}

Arcadia_Natural64Value
Arcadia_makeBitmaskN64
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  )
{
  static const Arcadia_SizeValue width = 64;
  if (i >= width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // "mask" is initially empty.
  Arcadia_Natural8Value mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Arcadia_SizeValue j = i, m = i + n; j < m; ++j) {
    mask |= UINT64_C(1) << (width - j - 1);
  }
  return mask;
}

Arcadia_Natural8Value
Arcadia_makeBitmaskN8
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue i,
    Arcadia_SizeValue n
  )
{
  static const Arcadia_SizeValue width = 8;
  if (i >= width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // i = <width of type> - 1 and n = 1 is valid.
  if (i + n > width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // "mask" is initially empty.
  Arcadia_Natural8Value mask = 0;
  // set the to be cleared bits in "mask" to 1.
  for (Arcadia_SizeValue j = i, m = i + n; j < m; ++j) {
    mask |= UINT8_C(1) << (width - j - 1);
  }
  return mask;
}
