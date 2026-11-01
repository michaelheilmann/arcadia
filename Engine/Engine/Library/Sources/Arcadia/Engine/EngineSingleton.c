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

#include "Arcadia/Engine/Include.h"

// This cache is intentionally not a GC root. PDL has no global weak storage
// or destruction-notification binding; keep that runtime boundary native.
static Arcadia_Engine* g_instance;

static void
destroyCallback
  (
    void* context,
    Arcadia_Object* object
  )
{
  g_instance = NULL;
}

Arcadia_Engine*
Arcadia_Engine_getOrCreateImpl
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_Engine* instance = (Arcadia_Engine*)_Arcadia_EndCreate0(thread, _Arcadia_Engine_getType(thread), base);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, instance);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, &destroyCallback);
      g_instance = instance;
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  }
  return g_instance;
}
