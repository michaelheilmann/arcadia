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

#if !defined(ARCADIA_ENGINE_UI_LAYOUT_H_INCLUDED)
#define ARCADIA_ENGINE_UI_LAYOUT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

// A layout capability, independent of any specific object hierarchy.
// Implementations operate on Containers but do not own their children.
Arcadia_declareInterfaceType(u8"Arcadia.Engine.UI.Layout", Arcadia_Engine_UI_Layout)

struct Arcadia_Engine_UI_LayoutDispatch {
  Arcadia_InterfaceDispatch _parent;
  void (*measure)(Arcadia_Thread*, Arcadia_Object*, Arcadia_Object*, Arcadia_Integer32Value, Arcadia_Integer32Value, Arcadia_Integer32Value*, Arcadia_Integer32Value*);
  void (*arrange)(Arcadia_Thread*, Arcadia_Object*, Arcadia_Object*, Arcadia_Integer32Value, Arcadia_Integer32Value, Arcadia_Integer32Value, Arcadia_Integer32Value);
};

/// @brief Calculate a container's desired size using its children.
/// @remarks Implementations may call WidgetNode_measure on children obtained via Container.
///          The layout must not retain children or change actual bounds in this operation.
void
Arcadia_Engine_UI_Layout_measure
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight,
    Arcadia_Integer32Value* desiredWidth,
    Arcadia_Integer32Value* desiredHeight
  );

/// @brief Arrange a container's children within the given actual bounds.
/// @remarks The caller sets the container's own bounds before this operation.
void
Arcadia_Engine_UI_Layout_arrange
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

#endif // ARCADIA_ENGINE_UI_LAYOUT_H_INCLUDED
