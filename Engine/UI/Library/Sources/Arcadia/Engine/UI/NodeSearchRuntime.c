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

#include "Arcadia/Engine/UI/Include.h"

Arcadia_BooleanValue
Arcadia_Engine_UI_nodeNameMatchesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* root,
    Arcadia_String* name
  )
{
  if (!root) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Object_isInstanceOf(thread, root, _Arcadia_Engine_UI_WidgetNode_getType(thread))) {
    Arcadia_String* candidate = Arcadia_Engine_UI_WidgetNode_getName(thread, (void*)root);
    if (candidate) {
      if (!name) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, name);
      return Arcadia_String_getNumberOfBytes(thread, candidate) == length
        && Arcadia_String_isEqualTo_pn(thread, candidate, Arcadia_String_getBytes(thread, name), length);
    }
  }
  return false;
}

Arcadia_Natural64Value
Arcadia_Engine_UI_nodeChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* root
  )
{
  if (!root) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Object_isInstanceOf(thread, root, _Arcadia_Engine_UI_Container_getType(thread))
    ? Arcadia_Engine_UI_Container_getChildCount(thread, root) : 0;
}

Arcadia_BooleanValue
Arcadia_Engine_UI_nodeHasValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* node
  )
{ return node != NULL; }

Arcadia_Object*
Arcadia_Engine_UI_nodeChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* root,
    Arcadia_Natural64Value index
  )
{
  if (!root) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Engine_UI_Container_getChildAt(thread, root, index);
}
