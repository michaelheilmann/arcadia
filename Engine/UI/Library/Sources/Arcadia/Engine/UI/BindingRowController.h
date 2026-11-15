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

#if !defined(ARCADIA_ENGINE_UI_BINDINGROWCONTROLLER_H_INCLUDED)
#define ARCADIA_ENGINE_UI_BINDINGROWCONTROLLER_H_INCLUDED
#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/InputBindingService.h"
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"
#include "Arcadia/Engine/UI/Nodes/ButtonNode.h"

// Composes ordinary button and label nodes; owns no rendering or action map.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.BindingRowController", Arcadia_Engine_UI_BindingRowController,
                          u8"Arcadia.Object")
struct Arcadia_Engine_UI_BindingRowControllerDispatch { Arcadia_ObjectDispatch _parent; };
struct Arcadia_Engine_UI_BindingRowController {
  Arcadia_Object _parent;
  Arcadia_Engine_UI_InputBindingService* service;
  Arcadia_Engine_UI_CanvasNode* canvas;
  Arcadia_String* action;
  Arcadia_Engine_UI_ButtonNode* edit;
  Arcadia_Engine_UI_ButtonNode* reset;
  Arcadia_Engine_UI_LabelNode* valueLabel;
  Arcadia_Engine_UI_LabelNode* statusLabel;
  Arcadia_Engine_UI_InputBinding defaultBinding;
  Arcadia_List* connections;
  Arcadia_BooleanValue enabled;
  Arcadia_BooleanValue captureSeen;
  Arcadia_BooleanValue presentationValid;
  Arcadia_Engine_UI_InputBinding previousBinding;
  Arcadia_Engine_UI_InputCaptureState previousState;
  Arcadia_BooleanValue previousCaptureSeen;
};

Arcadia_Engine_UI_BindingRowController*
Arcadia_Engine_UI_BindingRowController_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* service,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_String* action,
    Arcadia_Engine_UI_ButtonNode* edit,
    Arcadia_Engine_UI_ButtonNode* reset,
    Arcadia_Engine_UI_LabelNode* valueLabel,
    Arcadia_Engine_UI_LabelNode* statusLabel,
    Arcadia_Engine_UI_InputBinding defaultBinding
  );
void
Arcadia_Engine_UI_BindingRowController_refresh
  (
    Arcadia_Thread*,
    Arcadia_Engine_UI_BindingRowController*
  );

void
Arcadia_Engine_UI_BindingRowController_setEnabled
  (
    Arcadia_Thread*,
    Arcadia_Engine_UI_BindingRowController*,
    Arcadia_BooleanValue
  );

void
Arcadia_Engine_UI_BindingRowController_reset
  (
    Arcadia_Thread*,
    Arcadia_Engine_UI_BindingRowController*
  );

#endif // ARCADIA_ENGINE_UI_BINDINGROWCONTROLLER_H_INCLUDED
