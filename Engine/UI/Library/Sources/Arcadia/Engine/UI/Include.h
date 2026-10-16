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

#if !defined(ARCADIA_ENGINE_UI_INCLUDE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_INCLUDE_H_INCLUDED

#pragma push_macro("ARCADIA_ENGINE_UI_PRIVATE")
#undef ARCADIA_ENGINE_UI_PRIVATE
#define ARCADIA_ENGINE_UI_PRIVATE (1)

#include "Arcadia/Engine/UI/Activatable.h"
#include "Arcadia/Engine/UI/Container.h"
#include "Arcadia/Engine/UI/DefinitionFactory.h"
#include "Arcadia/Engine/UI/Focusable.h"
#include "Arcadia/Engine/UI/HitTestable.h"
#include "Arcadia/Engine/UI/KeyboardInput.h"
#include "Arcadia/Engine/UI/PointerInput.h"
#include "Arcadia/Engine/UI/Renderable.h"
#include "Arcadia/Engine/UI/Scrollable.h"
#include "Arcadia/Engine/UI/ValueControl.h"
#include "Arcadia/Engine/UI/Nodes/ButtonNode.h"
#include "Arcadia/Engine/UI/Nodes/CanvasNode.h"
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"
#include "Arcadia/Engine/UI/Nodes/PanelNode.h"
#include "Arcadia/Engine/UI/Nodes/ScrollPanelNode.h"
#include "Arcadia/Engine/UI/Nodes/SegmentedChoiceNode.h"
#include "Arcadia/Engine/UI/Nodes/SliderNode.h"
#include "Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.h"
#include "Arcadia/Engine/UI/Nodes/StackPanelNode.h"
#include "Arcadia/Engine/UI/Nodes/ToggleNode.h"

#undef ARCADIA_ENGINE_UI_PRIVATE
#pragma pop_macro("ARCADIA_ENGINE_UI_PRIVATE")

#endif // ARCADIA_ENGINE_UI_INCLUDE_H_INCLUDED
