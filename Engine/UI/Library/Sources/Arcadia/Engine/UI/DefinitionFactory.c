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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/DefinitionFactory.h"

#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/UI/definitionChildCount.h"
#include "Arcadia/Engine/UI/resolveDefinitionChild.h"
#include "Arcadia/Engine/UI/appendDefinitionChild.h"

Arcadia_Object*
Arcadia_Engine_UI_createNodeFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_Definition* source
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_ButtonNode_create(thread, backendContext, (Arcadia_ADL_UI_ButtonDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_CanvasDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_CanvasNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_CanvasDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_LabelDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_LabelNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_LabelDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_ScrollPanelDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_ScrollPanelNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_ScrollPanelDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_SegmentedChoiceDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_SegmentedChoiceNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_SegmentedChoiceDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_SliderDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_SliderNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_SliderDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_StackPanelDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_StackPanelDefinition*)source);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source, _Arcadia_ADL_UI_ToggleDefinition_getType(thread))) {
    return (Arcadia_Object*)Arcadia_Engine_UI_ToggleNode_createFromDefinition(thread, backendContext, (Arcadia_ADL_UI_ToggleDefinition*)source);
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Thread_jump(thread);
}

Arcadia_Natural64Value
Arcadia_Engine_UI_definitionChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* children
  )
{
  return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)children);
}

void
Arcadia_Engine_UI_appendDefinitionChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* target,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_Container_addChild(thread, target, child);
}

Arcadia_ADL_Definition*
Arcadia_Engine_UI_resolveDefinitionChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* children,
    Arcadia_Natural64Value index
  )
{
  if (index > Arcadia_SizeValue_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_Reference* reference = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, children, (Arcadia_SizeValue)index, _Arcadia_ADL_Reference_getType(thread));
  Arcadia_ADL_Reference_resolve(thread, reference);
  return reference->definition;
}
