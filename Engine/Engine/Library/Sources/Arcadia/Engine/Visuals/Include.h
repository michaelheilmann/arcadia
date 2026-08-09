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

#if !defined(ARCADIA_ENGINE_VISUALS_INCLUDE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_INCLUDE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Backend.h"
#include "Arcadia/Engine/Visuals/BackendContext.h"

#include "Arcadia/Engine/Visuals/Configuration.h"

#include "Arcadia/Engine/Visuals/CullMode.h"

#include "Arcadia/Engine/Visuals/DepthCompareFunction.h"

#include "Arcadia/Engine/Visuals/FillMode.h"

#include "Arcadia/Engine/Visuals/Icon.h"

#include "Arcadia/Engine/Visuals/Node.h"
#include "Arcadia/Engine/Visuals/NodeFactory.h"
#include "Arcadia/Engine/Visuals/Nodes/CameraNode.h"
#include "Arcadia/Engine/Visuals/Nodes/EnterPassNode.h"
#include "Arcadia/Engine/Visuals/Nodes/FrameBufferNode.h"
#include "Arcadia/Engine/Visuals/Nodes/MaterialNode.h"
#include "Arcadia/Engine/Visuals/Nodes/MeshNode.h"
#include "Arcadia/Engine/Visuals/Nodes/ModelNode.h"
#include "Arcadia/Engine/Visuals/Nodes/PixelBufferNode.h"
#include "Arcadia/Engine/Visuals/Nodes/RenderScene.h"
#include "Arcadia/Engine/Visuals/Nodes/TextureNode.h"
#include "Arcadia/Engine/Visuals/Nodes/ViewportNode.h"

#include "Arcadia/Engine/Visuals/TextureAddressMode.h"
#include "Arcadia/Engine/Visuals/TextureFilter.h"

#include "Arcadia/VPL/Include.h"

#include "Arcadia/Engine/Visuals/WindingMode.h"

#include "Arcadia/Engine/Visuals/Window.h"
#include "Arcadia/Engine/Visuals/WindowBackend.h"

//{

#include "Arcadia/Engine/Visuals/Diagnostics.h"

#include "Arcadia/Engine/Visuals/ApplicationQuitRequestedEvent.h"

#include "Arcadia/Engine/Visuals/CanvasActivationStateChangedEvent.h"
#include "Arcadia/Engine/Visuals/CanvasDPIChangedEvent.h"
#include "Arcadia/Engine/Visuals/CanvasEvent.h"
#include "Arcadia/Engine/Visuals/CanvasSizeChangedEvent.h"
#include "Arcadia/Engine/Visuals/WindowClosedEvent.h"
#include "Arcadia/Engine/Visuals/WindowPositionChangedEvent.h"
#include "Arcadia/Engine/Visuals/WindowSizeChangedEvent.h"

#include "Arcadia/Engine/Input/KeyboardKeyEvent.h"
#include "Arcadia/Engine/Input/MouseButtonEvent.h"
#include "Arcadia/Engine/Input/MousePointerEvent.h"

#include "Arcadia/Engine/Visuals/DisplayDevice.h"
#include "Arcadia/Engine/Visuals/DisplayMode.h"

//}

#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/FrameBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/EnterPassResource.h"
#include "Arcadia/Engine/Visuals/Resources/MaterialResource.h"
#include "Arcadia/Engine/Visuals/Resources/ModelResource.h"
#include "Arcadia/Engine/Visuals/Resources/ProgramResource.h"
#include "Arcadia/Engine/Visuals/Resources/TextureResource.h"
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"

void
Arcadia_Engine_Visuals_registerNodeFactories
  ( 
    Arcadia_Thread* thread,
    Arcadia_Set* types
  );

#endif // ARCADIA_ENGINE_VISUALS_INCLUDE_H_INCLUDED
