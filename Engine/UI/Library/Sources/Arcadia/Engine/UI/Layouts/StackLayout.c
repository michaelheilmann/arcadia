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
#include "Arcadia/Engine/UI/Layouts/StackLayout.h"
#include "Arcadia/Engine/UI/Include.h"
#include <stdint.h>

static Arcadia_Integer32Value
clampAvailable
  (
    int64_t value
  )
{
  if (value <= 0) return 0;
  if (value > Arcadia_Integer32Value_Maximum / 2) return Arcadia_Integer32Value_Maximum / 2;
  return (Arcadia_Integer32Value)value;
}

static Arcadia_Integer32Value
alignedOffset
  (
    Arcadia_Integer32Value available,
    Arcadia_Integer32Value used,
    Arcadia_Engine_UI_LayoutAlignment alignment
  )
{
  int64_t extra = (int64_t)available - used;
  if (extra < 0) extra = 0;
  if (alignment == Arcadia_Engine_UI_LayoutAlignment_End) return (Arcadia_Integer32Value)extra;
  if (alignment == Arcadia_Engine_UI_LayoutAlignment_Center ||
      alignment == Arcadia_Engine_UI_LayoutAlignment_Stretch) return (Arcadia_Integer32Value)(extra / 2);
  return 0;
}

static void
validateAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutAlignment alignment
  )
{
  if (alignment < Arcadia_Engine_UI_LayoutAlignment_Start ||
      alignment > Arcadia_Engine_UI_LayoutAlignment_Stretch) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Engine_UI_StackLayoutSlot
emptySlot
  (
    void
  )
{
  Arcadia_Engine_UI_StackLayoutSlot slot = { 0 };
  return slot;
}

static Arcadia_Engine_UI_StackLayoutSlot const*
getSlot
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayoutSlot const* slots,
    Arcadia_SizeValue slotCount,
    Arcadia_SizeValue index,
    Arcadia_Engine_UI_StackLayoutSlot* fallback
  )
{
  if (!slots) {
    *fallback = emptySlot();
    return fallback;
  }
  if (index >= slotCount) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return &slots[index];
}

// The container owns per-child metadata. The strategy only borrows it.
static Arcadia_Engine_UI_StackLayoutSlot const*
getContainerSlots
  (
    Arcadia_Thread* thread,
    Arcadia_Object* container
  )
{
  if (Arcadia_Object_isInstanceOf(thread, container, _Arcadia_Engine_UI_LayoutPanelNode_getType(thread))) {
    return Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots(thread,
      (Arcadia_Engine_UI_LayoutPanelNode*)container);
  }
  return NULL;
}

static void
measureImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Object* container,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight,
    Arcadia_Integer32Value* desiredWidth,
    Arcadia_Integer32Value* desiredHeight
  )
{
  Arcadia_Engine_UI_StackLayoutSettings const* settings = &((Arcadia_Engine_UI_StackLayout*)object)->settings;
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, container);
  Arcadia_Engine_UI_StackLayoutSlot const* slots = getContainerSlots(thread, container);
  Arcadia_Integer32Value contentAvailableWidth = clampAvailable((int64_t)availableWidth - settings->paddingLeft - settings->paddingRight);
  Arcadia_Integer32Value contentAvailableHeight = clampAvailable((int64_t)availableHeight - settings->paddingBottom - settings->paddingTop);
  int64_t mainSize = 0, crossSize = 0;
  Arcadia_Natural64Value widgetCount = 0;
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
    if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
    Arcadia_Engine_UI_StackLayoutSlot fallback;
    Arcadia_Engine_UI_StackLayoutSlot const* slot = getSlot(thread, slots, (Arcadia_SizeValue)count, (Arcadia_SizeValue)i, &fallback);
    Arcadia_Integer32Value childAvailableWidth = clampAvailable((int64_t)contentAvailableWidth - slot->marginLeft - slot->marginRight);
    Arcadia_Integer32Value childAvailableHeight = clampAvailable((int64_t)contentAvailableHeight - slot->marginBottom - slot->marginTop);
    Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)child, childAvailableWidth, childAvailableHeight);
    Arcadia_Integer32Value childWidth, childHeight;
    Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
    mainSize += settings->vertical ? (int64_t)childHeight + slot->marginTop + slot->marginBottom
                                   : (int64_t)childWidth + slot->marginLeft + slot->marginRight;
    int64_t childCrossSize = settings->vertical ? (int64_t)childWidth + slot->marginLeft + slot->marginRight
                                                : (int64_t)childHeight + slot->marginTop + slot->marginBottom;
    if (childCrossSize > crossSize) crossSize = childCrossSize;
    widgetCount++;
  }
  if (widgetCount > 1) mainSize += (int64_t)settings->gap * (widgetCount - 1);
  int64_t width = (settings->vertical ? crossSize : mainSize) + settings->paddingLeft + settings->paddingRight;
  int64_t height = (settings->vertical ? mainSize : crossSize) + settings->paddingBottom + settings->paddingTop;
  if (width < 0 || height < 0 || width > Arcadia_Integer32Value_Maximum / 2 || height > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  *desiredWidth = (Arcadia_Integer32Value)width;
  *desiredHeight = (Arcadia_Integer32Value)height;
}

static void
arrangeChildren
  (
    Arcadia_Thread* thread,
    Arcadia_Object* container,
    Arcadia_Engine_UI_StackLayoutSettings const* settings,
    Arcadia_Engine_UI_StackLayoutSlot const* slots,
    Arcadia_SizeValue slotCount,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_BooleanValue legacy
  )
{
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, container);
  if (!count) return;
  Arcadia_Integer32Value contentLeft = left + settings->paddingLeft;
  Arcadia_Integer32Value contentBottom = bottom + settings->paddingBottom;
  Arcadia_Integer32Value contentWidth = clampAvailable((int64_t)width - settings->paddingLeft - settings->paddingRight);
  Arcadia_Integer32Value contentHeight = clampAvailable((int64_t)height - settings->paddingBottom - settings->paddingTop);
  int64_t total = 0;
  Arcadia_Natural64Value widgetCount = 0;
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
    if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
    Arcadia_Engine_UI_StackLayoutSlot fallback;
    Arcadia_Engine_UI_StackLayoutSlot const* slot = getSlot(thread, slots, slotCount, (Arcadia_SizeValue)i, &fallback);
    Arcadia_Integer32Value childWidth, childHeight;
    if (legacy) Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
    else Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
    total += settings->vertical ? (int64_t)childHeight + slot->marginTop + slot->marginBottom
                                : (int64_t)childWidth + slot->marginLeft + slot->marginRight;
    widgetCount++;
  }
  if (!widgetCount) return;
  total += (int64_t)settings->gap * (widgetCount - 1);
  if (total < Arcadia_Integer32Value_Minimum / 2 || total > Arcadia_Integer32Value_Maximum / 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Integer32Value mainTotal = (Arcadia_Integer32Value)total;
  if (settings->vertical) {
    Arcadia_Integer32Value y = contentBottom + contentHeight - alignedOffset(contentHeight, mainTotal, settings->mainAlignment);
    for (Arcadia_Natural64Value i = 0; i < count; ++i) {
      Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
      if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
      Arcadia_Engine_UI_StackLayoutSlot fallback;
      Arcadia_Engine_UI_StackLayoutSlot const* slot = getSlot(thread, slots, slotCount, (Arcadia_SizeValue)i, &fallback);
      Arcadia_Engine_UI_LayoutAlignment crossAlignment = slot->hasAlignmentOverride ? slot->crossAlignment : settings->crossAlignment;
      Arcadia_Integer32Value childWidth, childHeight;
      if (legacy) Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      else Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      Arcadia_Integer32Value childAvailableWidth = clampAvailable((int64_t)contentWidth - slot->marginLeft - slot->marginRight);
      y -= slot->marginTop;
      if (crossAlignment == Arcadia_Engine_UI_LayoutAlignment_Stretch) {
        childWidth = childAvailableWidth;
        if (legacy) Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, childWidth, childHeight);
      }
      y -= childHeight;
      Arcadia_Integer32Value x = contentLeft + slot->marginLeft + alignedOffset(childAvailableWidth, childWidth, crossAlignment);
      if (legacy) Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)child, x, y);
      else Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)child, x, y, childWidth, childHeight);
      y -= slot->marginBottom + settings->gap;
    }
  } else {
    Arcadia_Integer32Value x = contentLeft + alignedOffset(contentWidth, mainTotal, settings->mainAlignment);
    for (Arcadia_Natural64Value i = 0; i < count; ++i) {
      Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
      if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
      Arcadia_Engine_UI_StackLayoutSlot fallback;
      Arcadia_Engine_UI_StackLayoutSlot const* slot = getSlot(thread, slots, slotCount, (Arcadia_SizeValue)i, &fallback);
      Arcadia_Engine_UI_LayoutAlignment crossAlignment = slot->hasAlignmentOverride ? slot->crossAlignment : settings->crossAlignment;
      Arcadia_Integer32Value childWidth, childHeight;
      if (legacy) Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      else Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      Arcadia_Integer32Value childAvailableHeight = clampAvailable((int64_t)contentHeight - slot->marginBottom - slot->marginTop);
      x += slot->marginLeft;
      if (crossAlignment == Arcadia_Engine_UI_LayoutAlignment_Stretch) {
        childHeight = childAvailableHeight;
        if (legacy) Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, childWidth, childHeight);
      }
      Arcadia_Integer32Value y = contentBottom + slot->marginBottom + alignedOffset(childAvailableHeight, childHeight, crossAlignment);
      if (legacy) Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)child, x, y);
      else Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)child, x, y, childWidth, childHeight);
      x += childWidth + slot->marginRight + settings->gap;
    }
  }
}

static void
layoutMeasureImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight,
    Arcadia_Integer32Value* desiredWidth,
    Arcadia_Integer32Value* desiredHeight
  )
{ measureImpl(thread, self, container, availableWidth, availableHeight, desiredWidth, desiredHeight); }

static void
layoutArrangeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Engine_UI_StackLayout* layout = (Arcadia_Engine_UI_StackLayout*)self;
  Arcadia_Engine_UI_StackLayoutSlot const* slots = getContainerSlots(thread, container);
  Arcadia_SizeValue count = Arcadia_Engine_UI_Container_getChildCount(thread, container);
  arrangeChildren(thread, container, &layout->settings, slots, count, left, bottom, width, height, Arcadia_BooleanValue_False);
}

static void
initializeLayoutDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutDispatch* self
  )
{
  self->measure = &layoutMeasureImpl;
  self->arrange = &layoutArrangeImpl;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayout* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_StackLayout);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->settings.vertical = Arcadia_ValueStack_getBooleanValue(thread, 2);
  self->settings.gap = Arcadia_ValueStack_getInteger32Value(thread, 1);
  if (self->settings.gap < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->settings.paddingLeft = self->settings.paddingBottom = 0;
  self->settings.paddingRight = self->settings.paddingTop = 0;
  self->settings.mainAlignment = Arcadia_Engine_UI_LayoutAlignment_Center;
  self->settings.crossAlignment = Arcadia_Engine_UI_LayoutAlignment_Center;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_StackLayout);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.StackLayout", Arcadia_Engine_UI_StackLayout,
    u8"Arcadia.Object", Arcadia_Object, &typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Layout_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_LayoutDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeLayoutDispatchImpl }
  )

Arcadia_Engine_UI_StackLayout*
Arcadia_Engine_UI_StackLayout_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_StackLayout);
  Arcadia_ValueStack_pushBooleanValue(thread, vertical);
  Arcadia_ValueStack_pushInteger32Value(thread, gap);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_UI_StackLayout);
}

void
Arcadia_Engine_UI_StackLayout_setPadding
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayout* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  )
{
  if (left < 0 || bottom < 0 || right < 0 || top < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->settings.paddingLeft = left;
  self->settings.paddingBottom = bottom;
  self->settings.paddingRight = right;
  self->settings.paddingTop = top;
}

void
Arcadia_Engine_UI_StackLayout_setAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayout* self,
    Arcadia_Engine_UI_LayoutAlignment mainAlignment,
    Arcadia_Engine_UI_LayoutAlignment crossAlignment
  )
{
  validateAlignment(thread, mainAlignment);
  validateAlignment(thread, crossAlignment);
  self->settings.mainAlignment = mainAlignment;
  self->settings.crossAlignment = crossAlignment;
}

void
Arcadia_Engine_UI_StackLayout_arrangeLegacy
  (
    Arcadia_Thread* thread,
    Arcadia_Object* container,
    Arcadia_Engine_UI_StackLayoutSettings const* settings,
    Arcadia_Engine_UI_StackLayoutSlot const* slots,
    Arcadia_SizeValue slotCount,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  arrangeChildren(thread, container, settings, slots, slotCount, left, bottom, width, height, Arcadia_BooleanValue_True);
}
