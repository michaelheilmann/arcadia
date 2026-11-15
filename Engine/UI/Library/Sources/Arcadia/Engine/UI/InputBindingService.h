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

#if !defined(ARCADIA_ENGINE_UI_INPUTBINDINGSERVICE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_INPUTBINDINGSERVICE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/Nodes/CanvasNode.h"

// Stable, platform-neutral binding. Code is KeyboardKey for Keyboard,
// or a physical mouse button id (0..4). Left/right modifiers are normalized.
typedef enum Arcadia_Engine_UI_InputBindingDevice {
  Arcadia_Engine_UI_InputBindingDevice_Keyboard = 1,
  Arcadia_Engine_UI_InputBindingDevice_MouseButton = 2,
} Arcadia_Engine_UI_InputBindingDevice;

typedef enum Arcadia_Engine_UI_InputBindingModifier {
  Arcadia_Engine_UI_InputBindingModifier_Shift = 1,
  Arcadia_Engine_UI_InputBindingModifier_Control = 2,
  Arcadia_Engine_UI_InputBindingModifier_Alt = 4,
} Arcadia_Engine_UI_InputBindingModifier;

typedef struct Arcadia_Engine_UI_InputBinding {
  Arcadia_Natural32Value code;
  Arcadia_Natural8Value device;
  Arcadia_Natural8Value modifiers;
  Arcadia_BooleanValue bound;
} Arcadia_Engine_UI_InputBinding;

typedef enum Arcadia_Engine_UI_InputBindingResult {
  Arcadia_Engine_UI_InputBindingResult_Unchanged,
  Arcadia_Engine_UI_InputBindingResult_Changed,
  Arcadia_Engine_UI_InputBindingResult_Conflict,
  Arcadia_Engine_UI_InputBindingResult_Rejected,
} Arcadia_Engine_UI_InputBindingResult;

typedef enum Arcadia_Engine_UI_InputCaptureState {
  Arcadia_Engine_UI_InputCaptureState_Idle,
  Arcadia_Engine_UI_InputCaptureState_Listening,
  Arcadia_Engine_UI_InputCaptureState_Committed,
  Arcadia_Engine_UI_InputCaptureState_Cancelled,
  Arcadia_Engine_UI_InputCaptureState_Conflict,
  Arcadia_Engine_UI_InputCaptureState_Rejected,
} Arcadia_Engine_UI_InputCaptureState;

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.InputBindingService", Arcadia_Engine_UI_InputBindingService,
                          u8"Arcadia.Object")

struct Arcadia_Engine_UI_InputBindingServiceDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_UI_InputBindingService {
  Arcadia_Object _parent;
  Arcadia_List* actionNames;
  Arcadia_Engine_UI_InputBinding* bindings;
  Arcadia_SizeValue bindingCapacity;
  Arcadia_Signal* changed;
  Arcadia_Engine_UI_CanvasNode* captureCanvas;
  Arcadia_String* captureAction;
  Arcadia_String* conflictAction;
  // Independent left/right Shift, Control and Alt key down flags.
  Arcadia_Natural8Value modifierSides;
  Arcadia_Engine_UI_InputCaptureState captureState;
};

Arcadia_Engine_UI_InputBindingService*
Arcadia_Engine_UI_InputBindingService_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_UI_InputBindingService_addAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  );

void
Arcadia_Engine_UI_InputBindingService_removeAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  );

Arcadia_Engine_UI_InputBinding
Arcadia_Engine_UI_InputBindingService_get
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  );

Arcadia_Engine_UI_InputBindingResult
Arcadia_Engine_UI_InputBindingService_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action,
    Arcadia_Engine_UI_InputBinding binding
  );

void
Arcadia_Engine_UI_InputBindingService_beginCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_String* action
  );

void
Arcadia_Engine_UI_InputBindingService_cancelCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  );

Arcadia_Engine_UI_InputCaptureState
Arcadia_Engine_UI_InputBindingService_getCaptureState
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  );

Arcadia_String*
Arcadia_Engine_UI_InputBindingService_getConflictingAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  );

Arcadia_Signal*
Arcadia_Engine_UI_InputBindingService_getChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  );

#endif // ARCADIA_ENGINE_UI_INPUTBINDINGSERVICE_H_INCLUDED
