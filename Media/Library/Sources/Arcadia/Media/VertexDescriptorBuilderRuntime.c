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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/VertexDescriptor.module.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/Collections/Include.h"

Arcadia_Natural64Value
Arcadia_Media_vertexElementCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* elements
  )
{
  if (!elements) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Collection_getSize(thread, (void*)elements);
}

Arcadia_Natural64Value
Arcadia_Media_vertexElementEndAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* elements,
    Arcadia_Natural64Value index
  )
{
  if (!elements || index > Arcadia_SizeValue_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_VertexElementDescriptor* element = (void*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, elements, (Arcadia_SizeValue)index, _Arcadia_Media_VertexElementDescriptor_getType(thread));
  // Do arithmetic in Size before widening: this preserves native Size-width
  // overflow semantics on both 32- and 64-bit builds.
  Arcadia_SizeValue end = element->offset;
  switch (element->syntactics) {
    case Arcadia_Media_VertexElementSyntactics_Real32: end += sizeof(Arcadia_Real32Value); break;
    case Arcadia_Media_VertexElementSyntactics_Real32Real32: end += 2 * sizeof(Arcadia_Real32Value); break;
    case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32: end += 3 * sizeof(Arcadia_Real32Value); break;
    case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32: end += 4 * sizeof(Arcadia_Real32Value); break;
    default:
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
  }
  return end;
}

Arcadia_Media_VertexDescriptor*
Arcadia_Media_createVertexDescriptorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* elements,
    Arcadia_Natural64Value stride
  )
{
  if (!elements || stride > Arcadia_SizeValue_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ImmutableList* snapshot = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(elements));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, snapshot);
  Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptor_create(thread, (Arcadia_SizeValue)stride, snapshot);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
