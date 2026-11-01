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

#include "Arcadia/Games/Anima/MenuScene.h"

#include "Arcadia/FileSystem/Include.h"

static void
loadDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuSceneDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static void
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
rebuildOptionsContent
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
populateResolutionModes
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
setResolutionSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_BooleanValue next
  );

static void
applySelectedResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static void
onMasterVolumeChanged
  (
    Arcadia_Thread* thread
  );

static void
onMusicVolumeChanged
  (
    Arcadia_Thread* thread
  );

static void
onEffectsVolumeChanged
  (
    Arcadia_Thread* thread
  );

static void
onUIVolumeChanged
  (
    Arcadia_Thread* thread
  );

static void
onVoiceVolumeChanged
  (
    Arcadia_Thread* thread
  );

static Arcadia_String*
createResolutionLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* displayMode
  );

static Arcadia_BooleanValue
stringEquals
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    char const* value
  );

static Arcadia_String*
formatVolumeValue
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value value
  );

static Arcadia_Real64Value
getReal64ConfigurationValueOrDefault
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    char const* path[],
    Arcadia_SizeValue pathLength,
    Arcadia_Real64Value defaultValue
  );

static void
populateVolumeSettings
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};
static const Arcadia_Type_Operations _typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &_objectTypeOperations };

Arcadia_defineObjectType(u8"Arcadia.Games.Anima.MenuScene", Arcadia_Games_Anima_MenuScene,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene, &_typeOperations);

static void
insertDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_ADL_Definition* definition
  )
{
  Arcadia_Map_set(thread, definitions->definitions, Arcadia_Value_makeObjectReferenceValue(definition->name), Arcadia_Value_makeObjectReferenceValue(definition), NULL, NULL);
}

static Arcadia_ADL_Definition*
getDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    char const* name
  )
{
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions,
    Arcadia_String_createFromCxxString(thread, name));
  if (!definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return definition;
}

static Arcadia_ADL_UI_CanvasDefinition*
createCanvasDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    char const* name,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_ADL_UI_CanvasDefinition* definition = Arcadia_ADL_UI_CanvasDefinition_create(thread, definitions,
    Arcadia_String_createFromCxxString(thread, name), width, height);
  insertDefinition(thread, definitions, (Arcadia_ADL_Definition*)definition);
  return definition;
}

static Arcadia_Engine_UI_CanvasNode*
createCanvas
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  return Arcadia_Engine_UI_CanvasNode_createFromDefinition(thread, backendContext,
    createCanvasDefinition(thread, self->definitions, u8"Anima.Canvas.Root", 0, 0));
}

static void
loadDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/UI/Menu.adl"));
  Arcadia_ByteArrayBuilder* fileContents = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
  Arcadia_RuntimeByteArray* runtimeBytes = Arcadia_RuntimeByteArray_create(thread,
    Arcadia_ByteArrayBuilder_getBytes(thread, fileContents), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, fileContents));
  Arcadia_ByteArray* bytes = Arcadia_ByteArray_createByteArray(thread, runtimeBytes);
  Arcadia_UnicodeCodePointReader* codePointReader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread,
    (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, bytes));
  Arcadia_DDL_Node* root = Arcadia_DDL_DefaultReader_run(thread, Arcadia_DDL_DefaultReader_create(thread), codePointReader);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)root, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_DDL_ListNode* definitions = (Arcadia_DDL_ListNode*)root;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)definitions->elements); i < n; ++i) {
    Arcadia_DDL_Node* definition = (Arcadia_DDL_Node*)Arcadia_List_getObjectReferenceValueAt(thread, (Arcadia_List*)definitions->elements, i);
    Arcadia_ADL_Context_readFromNode(thread, context, self->definitions, definition, Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, self->definitions);
}

static Arcadia_Engine_UI_StackPanelNode*
createOptionRow
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    char const* name
  )
{
  return Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, backendContext,
    (Arcadia_ADL_UI_StackPanelDefinition*)getDefinition(thread, self->definitions, name));
}

static Arcadia_Object*
findWidget
  (
    Arcadia_Thread* thread,
    Arcadia_Object* root,
    char const* name
  )
{
  return Arcadia_Engine_UI_Node_findByName(thread, root, Arcadia_String_createFromCxxString(thread, name));
}

static void
setMode
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Games_Anima_MenuMode mode
  )
{
  if (self->mode == mode || !self->canvasNode) return;
  Arcadia_Object* oldRoot = self->mode == Arcadia_Games_Anima_MenuMode_Start ? (Arcadia_Object*)self->startRoot : (Arcadia_Object*)self->optionsRoot;
  Arcadia_Object* newRoot = mode == Arcadia_Games_Anima_MenuMode_Start ? (Arcadia_Object*)self->startRoot : (Arcadia_Object*)self->optionsRoot;
  if (oldRoot) Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)self->canvasNode, oldRoot);
  if (newRoot) Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->canvasNode, newRoot);
  self->mode = mode;
}

static void
onButtonActivated
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value n = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != n) { Arcadia_ValueStack_popValues(thread, n + 1); Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Games_Anima_MenuScene* self = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Games_Anima_MenuScene_getType(thread));
  Arcadia_Engine_UI_ButtonNode* button = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_ButtonNode_getType(thread));
  if (button == self->optionsButton) {
    setMode(thread, self, Arcadia_Games_Anima_MenuMode_Options);
  } else if (button == self->resolutionPreviousButton) {
    setResolutionSelection(thread, self, Arcadia_BooleanValue_False);
  } else if (button == self->resolutionNextButton) {
    setResolutionSelection(thread, self, Arcadia_BooleanValue_True);
  } else if (button == self->applyButton) {
    applySelectedResolution(thread, self);
  } else if (button == self->backButton) {
    setMode(thread, self, Arcadia_Games_Anima_MenuMode_Start);
  } else if (button == self->exitButton) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* event = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)event);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  }
}

static void
ensureDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  if (self->definitions) return;
  self->definitions = Arcadia_ADL_Definitions_create(thread);
  loadDefinitions(thread, self);
}

static void
clearContainer
  (
    Arcadia_Thread* thread,
    Arcadia_Object* container
  )
{
  while (Arcadia_Engine_UI_Container_getChildCount(thread, container)) {
    Arcadia_Object* child = Arcadia_Engine_UI_Container_getChildAt(thread, container, 0);
    Arcadia_Engine_UI_Container_removeChild(thread, container, child);
  }
}

static void
rebuildOptionsContent
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  if (!self->optionsContentRoot || !self->optionsTabs) return;
  clearContainer(thread, (Arcadia_Object*)self->optionsContentRoot);
  switch (Arcadia_Engine_UI_SegmentedChoiceNode_getSelectedIndex(thread, self->optionsTabs)) {
    case 0: {
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->resolutionRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->vsyncRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->brightnessRow);
    } break;
    case 1: {
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->masterVolumeRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->musicVolumeRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->effectsVolumeRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->uiVolumeRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->voiceVolumeRow);
    } break;
    case 2: {
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->mouseSensitivityRow);
    } break;
    case 3: {
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->subtitlesRow);
      Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->optionsContentRoot, (Arcadia_Object*)self->highContrastRow);
    } break;
  }
}

static Arcadia_String*
createResolutionLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* displayMode
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromInteger32(thread,
      Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, displayMode))));
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8" x ", 3)));
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromInteger32(thread,
      Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, displayMode))));
  return Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread,
    Arcadia_StringBuilder_getBytes(thread, builder), Arcadia_StringBuilder_getNumberOfBytes(thread, builder)));
}

static Arcadia_BooleanValue
stringEquals
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    char const* value
  )
{
  Arcadia_String* other = Arcadia_String_createFromCxxString(thread, value);
  Arcadia_Value left = Arcadia_Value_makeObjectReferenceValue(string);
  Arcadia_Value right = Arcadia_Value_makeObjectReferenceValue(other);
  return Arcadia_Value_isEqualTo(thread, &left, &right);
}

static Arcadia_String*
formatVolumeValue
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value value
  )
{
  Arcadia_Integer32Value hundredths = (Arcadia_Integer32Value)(value * 100.0 + 0.5);
  if (hundredths < 0) hundredths = 0;
  if (hundredths > 100) hundredths = 100;
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromInteger32(thread, hundredths / 100)));
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8".", 1)));
  Arcadia_Integer32Value fractionalPart = hundredths % 100;
  if (fractionalPart < 10) {
    Arcadia_StringBuilder_insertBack(thread, builder,
      Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"0", 1)));
  }
  Arcadia_StringBuilder_insertBack(thread, builder,
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromInteger32(thread, fractionalPart)));
  return Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread,
    Arcadia_StringBuilder_getBytes(thread, builder), Arcadia_StringBuilder_getNumberOfBytes(thread, builder)));
}

static Arcadia_Real64Value
getReal64ConfigurationValueOrDefault
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* configuration,
    char const* path[],
    Arcadia_SizeValue pathLength,
    Arcadia_Real64Value defaultValue
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Real64Value value = Arcadia_Engine_ApplicationConfiguration_getReal64(thread, configuration, path, pathLength);
    Arcadia_Thread_popJumpTarget(thread);
    return value;
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_Engine_ApplicationConfiguration_setReal64(thread, configuration, path, pathLength, defaultValue);
  return defaultValue;
}

typedef enum Arcadia_Games_Anima_AudioCategory {
  Arcadia_Games_Anima_AudioCategory_Master,
  Arcadia_Games_Anima_AudioCategory_Music,
  Arcadia_Games_Anima_AudioCategory_Effects,
  Arcadia_Games_Anima_AudioCategory_UserInterface,
  Arcadia_Games_Anima_AudioCategory_Voice,
} Arcadia_Games_Anima_AudioCategory;

typedef struct Arcadia_Games_Anima_AudioVolumeSetting {
  char const* key;
  Arcadia_Real64Value defaultValue;
} Arcadia_Games_Anima_AudioVolumeSetting;

static const Arcadia_Games_Anima_AudioVolumeSetting _audioVolumeSettings[] = {
  { u8"masterVolume",    1.00 },
  { u8"musicVolume",     0.70 },
  { u8"effectsVolume",   1.00 },
  { u8"uiVolume",        1.00 },
  { u8"voiceVolume",     1.00 },
};

static Arcadia_Engine_UI_SliderNode*
getVolumeSlider
  (
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Games_Anima_AudioCategory category
  )
{
  switch (category) {
    case Arcadia_Games_Anima_AudioCategory_Master: return self->masterVolumeSlider;
    case Arcadia_Games_Anima_AudioCategory_Music: return self->musicVolumeSlider;
    case Arcadia_Games_Anima_AudioCategory_Effects: return self->effectsVolumeSlider;
    case Arcadia_Games_Anima_AudioCategory_UserInterface: return self->uiVolumeSlider;
    case Arcadia_Games_Anima_AudioCategory_Voice: return self->voiceVolumeSlider;
  }
  return NULL;
}

static Arcadia_Engine_UI_LabelNode*
getVolumeValueLabel
  (
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Games_Anima_AudioCategory category
  )
{
  switch (category) {
    case Arcadia_Games_Anima_AudioCategory_Master: return self->masterVolumeValueLabel;
    case Arcadia_Games_Anima_AudioCategory_Music: return self->musicVolumeValueLabel;
    case Arcadia_Games_Anima_AudioCategory_Effects: return self->effectsVolumeValueLabel;
    case Arcadia_Games_Anima_AudioCategory_UserInterface: return self->uiVolumeValueLabel;
    case Arcadia_Games_Anima_AudioCategory_Voice: return self->voiceVolumeValueLabel;
  }
  return NULL;
}

static void
applyAudioCategoryVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Games_Anima_AudioCategory category,
    Arcadia_Real64Value volume
  )
{
  switch (category) {
    case Arcadia_Games_Anima_AudioCategory_Master: {
      if (((Arcadia_Engine_Scene*)self)->engine->audialsBackendContext) {
        Arcadia_Engine_Audials_BackendContext_setMasterVolume(thread,
          (Arcadia_Engine_Audials_BackendContext*)((Arcadia_Engine_Scene*)self)->engine->audialsBackendContext,
          volume);
      }
    } break;
    case Arcadia_Games_Anima_AudioCategory_Music: {
      if (self->ambienceNode) {
        Arcadia_Engine_Audials_StreamingSoundSourceNode_setVolume(thread, self->ambienceNode,
          self->ambienceBaseVolume * (Arcadia_Real32Value)volume);
      }
    } break;
    case Arcadia_Games_Anima_AudioCategory_Effects:
    case Arcadia_Games_Anima_AudioCategory_UserInterface:
    case Arcadia_Games_Anima_AudioCategory_Voice: {
    } break;
  }
}

static void
syncVolumeSetting
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Games_Anima_AudioCategory category
  )
{
  char const* path[] = { u8"audials", _audioVolumeSettings[category].key };
  Arcadia_Real64Value volume = getReal64ConfigurationValueOrDefault(thread,
    self->application->configuration, path, 2, _audioVolumeSettings[category].defaultValue);
  Arcadia_Engine_UI_SliderNode* slider = getVolumeSlider(self, category);
  Arcadia_Engine_UI_SliderNode_setValue(thread, slider, volume);
  volume = Arcadia_Engine_UI_SliderNode_getValue(thread, slider);
  Arcadia_Engine_ApplicationConfiguration_setReal64(thread, self->application->configuration, path, 2, volume);
  applyAudioCategoryVolume(thread, self, category, volume);
  Arcadia_Engine_UI_LabelNode_setText(thread, getVolumeValueLabel(self, category), formatVolumeValue(thread, volume));
}

static void
populateVolumeSettings
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  syncVolumeSetting(thread, self, Arcadia_Games_Anima_AudioCategory_Master);
  syncVolumeSetting(thread, self, Arcadia_Games_Anima_AudioCategory_Music);
  syncVolumeSetting(thread, self, Arcadia_Games_Anima_AudioCategory_Effects);
  syncVolumeSetting(thread, self, Arcadia_Games_Anima_AudioCategory_UserInterface);
  syncVolumeSetting(thread, self, Arcadia_Games_Anima_AudioCategory_Voice);
}

static void
handleVolumeChanged
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_AudioCategory category
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Games_Anima_MenuScene* self = (Arcadia_Games_Anima_MenuScene*)Arcadia_ValueStack_getObjectReferenceValueChecked
    (thread, 2, _Arcadia_Games_Anima_MenuScene_getType(thread));
  Arcadia_Engine_UI_SliderNode* slider = (Arcadia_Engine_UI_SliderNode*)Arcadia_ValueStack_getObjectReferenceValueChecked
    (thread, 1, _Arcadia_Engine_UI_SliderNode_getType(thread));
  Arcadia_Real64Value volume = Arcadia_Engine_UI_SliderNode_getValue(thread, slider);
  char const* path[] = { u8"audials", _audioVolumeSettings[category].key };
  Arcadia_Engine_ApplicationConfiguration_setReal64(thread, self->application->configuration, path, 2, volume);
  applyAudioCategoryVolume(thread, self, category, volume);
  Arcadia_Engine_UI_LabelNode_setText(thread, getVolumeValueLabel(self, category), formatVolumeValue(thread, volume));
}

static void
onMasterVolumeChanged
  (
    Arcadia_Thread* thread
  )
{
  handleVolumeChanged(thread, Arcadia_Games_Anima_AudioCategory_Master);
}

static void
onMusicVolumeChanged
  (
    Arcadia_Thread* thread
  )
{
  handleVolumeChanged(thread, Arcadia_Games_Anima_AudioCategory_Music);
}

static void
onEffectsVolumeChanged
  (
    Arcadia_Thread* thread
  )
{
  handleVolumeChanged(thread, Arcadia_Games_Anima_AudioCategory_Effects);
}

static void
onUIVolumeChanged
  (
    Arcadia_Thread* thread
  )
{
  handleVolumeChanged(thread, Arcadia_Games_Anima_AudioCategory_UserInterface);
}

static void
onVoiceVolumeChanged
  (
    Arcadia_Thread* thread
  )
{
  handleVolumeChanged(thread, Arcadia_Games_Anima_AudioCategory_Voice);
}

static void
populateResolutionModes
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  if (self->availableResolutionModes) return;
  self->availableResolutionModes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->availableResolutionLabels = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
  Arcadia_Engine_Visuals_BackendContext* backendContext =
    (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext;
  Arcadia_List* displays = Arcadia_Engine_Visuals_BackendContext_getDisplayDevices(thread, backendContext);
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displays)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  char const* monitorPath[] = { u8"visuals", u8"monitor" };
  Arcadia_Integer32Value monitorIndex = Arcadia_Engine_ApplicationConfiguration_getInteger32(thread,
    self->application->configuration, monitorPath, 2);
  if (monitorIndex < 0 || monitorIndex >= Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displays)) {
    monitorIndex = 0;
  }
  self->displayDevice = (Arcadia_Engine_Visuals_DisplayDevice*)Arcadia_List_getObjectReferenceValueAt(thread, displays, monitorIndex);

  char const* horizontalPath[] = { u8"visuals", u8"horizontalResolution" };
  char const* verticalPath[] = { u8"visuals", u8"verticalResolution" };
  Arcadia_Integer32Value configuredWidth = Arcadia_Engine_ApplicationConfiguration_getInteger32(thread,
    self->application->configuration, horizontalPath, 2);
  Arcadia_Integer32Value configuredHeight = Arcadia_Engine_ApplicationConfiguration_getInteger32(thread,
    self->application->configuration, verticalPath, 2);

  Arcadia_List* displayModes = Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes(thread, self->displayDevice);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)displayModes); i < n; ++i) {
    Arcadia_Engine_Visuals_DisplayMode* mode = (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread, displayModes, i);
    Arcadia_Integer32Value width = Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, mode);
    Arcadia_Integer32Value height = Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, mode);
    Arcadia_SizeValue insertIndex = 0;
    Arcadia_BooleanValue duplicate = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->availableResolutionModes); j < m; ++j) {
      Arcadia_Engine_Visuals_DisplayMode* existing = (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread, self->availableResolutionModes, j);
      Arcadia_Integer32Value existingWidth = Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, existing);
      Arcadia_Integer32Value existingHeight = Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, existing);
      if (width == existingWidth && height == existingHeight) {
        duplicate = Arcadia_BooleanValue_True;
        break;
      }
      if (width > existingWidth || (width == existingWidth && height > existingHeight)) {
        insertIndex = j + 1;
      }
    }
    if (duplicate) continue;
    Arcadia_List_insertAt(thread, self->availableResolutionModes, insertIndex,
      Arcadia_Value_makeObjectReferenceValue(mode));
    Arcadia_List_insertAt(thread, self->availableResolutionLabels, insertIndex,
      Arcadia_Value_makeObjectReferenceValue(createResolutionLabel(thread, mode)));
  }

  Arcadia_SizeValue numberOfModes = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->availableResolutionModes);
  if (!numberOfModes) {
    Arcadia_Engine_Visuals_DisplayMode* current = Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode(thread, self->displayDevice);
    if (!current) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, self->availableResolutionModes, (Arcadia_Object*)current);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->availableResolutionLabels, (Arcadia_Object*)createResolutionLabel(thread, current));
    numberOfModes = 1;
  }

  self->selectedResolutionIndex = 0;
  for (Arcadia_SizeValue i = 0; i < numberOfModes; ++i) {
    Arcadia_Engine_Visuals_DisplayMode* mode = (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread,
      self->availableResolutionModes, i);
    if (configuredWidth == Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, mode) &&
        configuredHeight == Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, mode)) {
      self->selectedResolutionIndex = i;
      break;
    }
  }
  Arcadia_Engine_UI_LabelNode_setText(thread, self->resolutionLabel,
    (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->availableResolutionLabels, self->selectedResolutionIndex));
}

static void
setResolutionSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_BooleanValue next
  )
{
  Arcadia_SizeValue numberOfModes = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->availableResolutionModes);
  if (!numberOfModes) return;
  if (next) {
    self->selectedResolutionIndex = (self->selectedResolutionIndex + 1) % numberOfModes;
  } else if (self->selectedResolutionIndex) {
    --self->selectedResolutionIndex;
  } else {
    self->selectedResolutionIndex = numberOfModes - 1;
  }
  Arcadia_Engine_UI_LabelNode_setText(thread, self->resolutionLabel,
    (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->availableResolutionLabels, self->selectedResolutionIndex));
}

static void
applySelectedResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  if (!self->application->windows || !Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->application->windows)) return;
  Arcadia_Engine_Visuals_DisplayMode* mode = (Arcadia_Engine_Visuals_DisplayMode*)Arcadia_List_getObjectReferenceValueAt(thread,
    self->availableResolutionModes, self->selectedResolutionIndex);
  Arcadia_Integer32Value width = Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, mode);
  Arcadia_Integer32Value height = Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, mode);
  Arcadia_Integer32Value colorDepth = Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, mode);
  char const* horizontalPath[] = { u8"visuals", u8"horizontalResolution" };
  char const* verticalPath[] = { u8"visuals", u8"verticalResolution" };
  char const* colorDepthPath[] = { u8"visuals", u8"colorDepth" };
  char const* windowModePath[] = { u8"visuals", u8"windowMode" };
  Arcadia_Engine_ApplicationConfiguration_setInteger32(thread, self->application->configuration, horizontalPath, 2, width);
  Arcadia_Engine_ApplicationConfiguration_setInteger32(thread, self->application->configuration, verticalPath, 2, height);
  Arcadia_Engine_ApplicationConfiguration_setInteger32(thread, self->application->configuration, colorDepthPath, 2, colorDepth);
  Arcadia_String* windowMode = Arcadia_Engine_ApplicationConfiguration_getString(thread, self->application->configuration, windowModePath, 2);
  Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueAt(thread,
    self->application->windows, 0);
  Arcadia_Engine_Visuals_Window_setSize(thread, window, width, height);
  if (stringEquals(thread, windowMode, u8"fullscreen")) {
    Arcadia_Engine_Visuals_DisplayMode_apply(thread, mode);
    Arcadia_Integer32Value left, top, right, bottom;
    Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, self->displayDevice, &left, &top, &right, &bottom);
    Arcadia_Engine_Visuals_Window_setPosition(thread, window, left, top);
  } else {
    Arcadia_Integer32Value left, top, right, bottom;
    Arcadia_Engine_Visuals_DisplayDevice_getBounds(thread, self->displayDevice, &left, &top, &right, &bottom);
    Arcadia_Integer32Value windowWidth, windowHeight;
    Arcadia_Engine_Visuals_WindowBackend_getSize(thread, window->backend, &windowWidth, &windowHeight);
    Arcadia_Engine_Visuals_Window_setPosition(thread, window, left + ((right - left) - windowWidth) / 2,
      top + ((bottom - top) - windowHeight) / 2);
  }
}

static void
onOptionsTabChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value n = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != n) { Arcadia_ValueStack_popValues(thread, n + 1); Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_Games_Anima_MenuScene* self = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Games_Anima_MenuScene_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread));
  rebuildOptionsContent(thread, self);
}

static void
ensureScene
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  ensureDefinitions(thread, self);
  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
  Arcadia_Engine_Visuals_BackendContext* backendContext = (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext;
  if (!self->ambienceNode) {
    Arcadia_ADL_StreamingSoundSourceNodeDefinition* definition =
      (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)getDefinition(thread, self->definitions, u8"Anima.Audio.ShipAmbience");
    if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_StreamingSoundSourceNodeDefinition_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    self->ambienceBaseVolume = definition->volume;
    self->ambienceNode = Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition
      (
        thread,
        (Arcadia_Engine_Audials_NodeFactory*)engine->audialsNodeFactory,
        (Arcadia_Engine_Audials_BackendContext*)engine->audialsBackendContext,
        definition
      );
    Arcadia_Engine_Audials_Node_render(thread, (Arcadia_Engine_Audials_Node*)self->ambienceNode);
    Arcadia_Engine_Audials_StreamingSoundSourceNode_play(thread, self->ambienceNode);
  }
  if (!self->enterPassNode) self->enterPassNode = Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, backendContext);
  if (!self->viewportNode) {
    self->viewportNode = Arcadia_Engine_Visuals_NodeFactory_createViewportNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, backendContext);
    Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode, Arcadia_Math_Color4Real32_create4(thread, 0.06f, 0.08f, 0.09f, 1.f));
    Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode, 0.f, 0.f, 1.f, 1.f);
  }
  if (!self->cameraNode) self->cameraNode = Arcadia_Engine_Visuals_NodeFactory_createCameraNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, backendContext);
  if (!self->canvasNode) self->canvasNode = createCanvas(thread, self, backendContext);
  if (!self->startRoot) {
    self->startRoot = Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, backendContext,
      (Arcadia_ADL_UI_StackPanelDefinition*)getDefinition(thread, self->definitions, u8"Anima.StackPanel.StartRoot"));
    self->continueButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->startRoot, u8"Anima.Button.Continue");
    self->newGameButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->startRoot, u8"Anima.Button.NewGame");
    self->optionsButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->startRoot, u8"Anima.Button.Options");
    self->exitButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->startRoot, u8"Anima.Button.Exit");
    Arcadia_Engine_UI_ButtonNode_setEnabled(thread, self->continueButton, Arcadia_BooleanValue_False);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->continueButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->newGameButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->optionsButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->exitButton), (Arcadia_Object*)self, &onButtonActivated);
  }
  if (!self->optionsRoot) {
    self->optionsRoot = Arcadia_Engine_UI_StackPanelNode_createFromDefinition(thread, backendContext,
      (Arcadia_ADL_UI_StackPanelDefinition*)getDefinition(thread, self->definitions, u8"Anima.StackPanel.OptionsRoot"));
    self->optionsTabs = (Arcadia_Engine_UI_SegmentedChoiceNode*)findWidget(thread, (Arcadia_Object*)self->optionsRoot, u8"Anima.SegmentedChoice.OptionsTabs");
    self->optionsContentRoot = (Arcadia_Engine_UI_StackPanelNode*)findWidget(thread, (Arcadia_Object*)self->optionsRoot, u8"Anima.StackPanel.OptionsContent");
    self->optionsFooterRoot = (Arcadia_Engine_UI_StackPanelNode*)findWidget(thread, (Arcadia_Object*)self->optionsRoot, u8"Anima.StackPanel.OptionsFooter");
    self->resolutionRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.Resolution");
    self->resolutionSelector = (Arcadia_Engine_UI_StackPanelNode*)findWidget(thread, (Arcadia_Object*)self->resolutionRow, u8"Anima.StackPanel.ResolutionSelector");
    self->resolutionLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->resolutionRow, u8"Anima.Label.ResolutionValue");
    self->resolutionPreviousButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->resolutionRow, u8"Anima.Button.ResolutionPrevious");
    self->resolutionNextButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->resolutionRow, u8"Anima.Button.ResolutionNext");
    self->backButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->optionsFooterRoot, u8"Anima.Button.Back");
    self->resetButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->optionsFooterRoot, u8"Anima.Button.Reset");
    self->applyButton = (Arcadia_Engine_UI_ButtonNode*)findWidget(thread, (Arcadia_Object*)self->optionsFooterRoot, u8"Anima.Button.Apply");
    self->vsyncRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.VSync");
    self->brightnessRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.Brightness");
    self->masterVolumeRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.MasterVolume");
    self->musicVolumeRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.MusicVolume");
    self->effectsVolumeRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.EffectsVolume");
    self->uiVolumeRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.UIVolume");
    self->voiceVolumeRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.VoiceVolume");
    self->mouseSensitivityRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.MouseSensitivity");
    self->subtitlesRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.Subtitles");
    self->highContrastRow = createOptionRow(thread, self, backendContext, u8"Anima.StackPanel.OptionsRow.HighContrast");
    self->vsyncToggle = (Arcadia_Engine_UI_ToggleNode*)findWidget(thread, (Arcadia_Object*)self->vsyncRow, u8"Anima.Toggle.VSync");
    self->brightnessSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->brightnessRow, u8"Anima.Slider.Brightness");
    self->masterVolumeSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->masterVolumeRow, u8"Anima.Slider.MasterVolume");
    self->masterVolumeValueLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->masterVolumeRow, u8"Anima.Label.MasterVolumeValue");
    self->musicVolumeSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->musicVolumeRow, u8"Anima.Slider.MusicVolume");
    self->musicVolumeValueLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->musicVolumeRow, u8"Anima.Label.MusicVolumeValue");
    self->effectsVolumeSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->effectsVolumeRow, u8"Anima.Slider.EffectsVolume");
    self->effectsVolumeValueLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->effectsVolumeRow, u8"Anima.Label.EffectsVolumeValue");
    self->uiVolumeSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->uiVolumeRow, u8"Anima.Slider.UIVolume");
    self->uiVolumeValueLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->uiVolumeRow, u8"Anima.Label.UIVolumeValue");
    self->voiceVolumeSlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->voiceVolumeRow, u8"Anima.Slider.VoiceVolume");
    self->voiceVolumeValueLabel = (Arcadia_Engine_UI_LabelNode*)findWidget(thread, (Arcadia_Object*)self->voiceVolumeRow, u8"Anima.Label.VoiceVolumeValue");
    self->mouseSensitivitySlider = (Arcadia_Engine_UI_SliderNode*)findWidget(thread, (Arcadia_Object*)self->mouseSensitivityRow, u8"Anima.Slider.MouseSensitivity");
    self->subtitlesToggle = (Arcadia_Engine_UI_ToggleNode*)findWidget(thread, (Arcadia_Object*)self->subtitlesRow, u8"Anima.Toggle.Subtitles");
    self->highContrastToggle = (Arcadia_Engine_UI_ToggleNode*)findWidget(thread, (Arcadia_Object*)self->highContrastRow, u8"Anima.Toggle.HighContrast");
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->vsyncRow, (Arcadia_Object*)self->vsyncToggle, 384, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->brightnessRow, (Arcadia_Object*)self->brightnessSlider, 160, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->masterVolumeRow, (Arcadia_Object*)self->masterVolumeSlider, 100, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->musicVolumeRow, (Arcadia_Object*)self->musicVolumeSlider, 100, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->effectsVolumeRow, (Arcadia_Object*)self->effectsVolumeSlider, 100, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->uiVolumeRow, (Arcadia_Object*)self->uiVolumeSlider, 100, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->voiceVolumeRow, (Arcadia_Object*)self->voiceVolumeSlider, 100, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->mouseSensitivityRow, (Arcadia_Object*)self->mouseSensitivitySlider, 160, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->subtitlesRow, (Arcadia_Object*)self->subtitlesToggle, 384, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->highContrastRow, (Arcadia_Object*)self->highContrastToggle, 384, 0, 0, 0);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->resolutionRow, (Arcadia_Object*)self->resolutionSelector, 184, 0, 0, 0);
    populateVolumeSettings(thread, self);
    populateResolutionModes(thread, self);
    Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)self->applyButton, 120, 44);
    Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)self->resetButton, 120, 44);
    Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)self->backButton, 120, 44);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->optionsFooterRoot, (Arcadia_Object*)self->backButton, 0, 0, 328, 0);
    rebuildOptionsContent(thread, self);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SegmentedChoiceNode_getChangedSignal(thread, self->optionsTabs), (Arcadia_Object*)self, &onOptionsTabChanged);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->backButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->applyButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->resolutionPreviousButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->resolutionNextButton), (Arcadia_Object*)self, &onButtonActivated);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->masterVolumeSlider), (Arcadia_Object*)self, &onMasterVolumeChanged);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->musicVolumeSlider), (Arcadia_Object*)self, &onMusicVolumeChanged);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->effectsVolumeSlider), (Arcadia_Object*)self, &onEffectsVolumeChanged);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->uiVolumeSlider), (Arcadia_Object*)self, &onUIVolumeChanged);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->voiceVolumeSlider), (Arcadia_Object*)self, &onVoiceVolumeChanged);
  }
  if (Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)self->canvasNode) == 0) {
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->canvasNode, (Arcadia_Object*)self->startRoot);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Games_Anima_MenuScene);
  Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 3);
  Arcadia_Value sceneManager = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_Value application = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_ValueStack_pushValue(thread, &engine);
  Arcadia_ValueStack_pushValue(thread, &sceneManager);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (3 != _numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->application = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Application_getType(thread));
  self->mode = Arcadia_Games_Anima_MenuMode_Start;
  self->definitions = NULL;
  self->availableResolutionModes = NULL;
  self->availableResolutionLabels = NULL;
  self->displayDevice = NULL;
  self->selectedResolutionIndex = 0;
  self->cameraNode = NULL; self->enterPassNode = NULL; self->viewportNode = NULL; self->canvasNode = NULL;
  self->ambienceNode = NULL;
  self->ambienceBaseVolume = 1.f;
  self->startRoot = NULL; self->optionsRoot = NULL; self->optionsContentRoot = NULL; self->optionsFooterRoot = NULL;
  self->vsyncRow = NULL; self->brightnessRow = NULL; self->masterVolumeRow = NULL; self->musicVolumeRow = NULL; self->effectsVolumeRow = NULL; self->uiVolumeRow = NULL; self->voiceVolumeRow = NULL; self->mouseSensitivityRow = NULL; self->subtitlesRow = NULL; self->highContrastRow = NULL;
  self->resolutionRow = NULL; self->resolutionSelector = NULL;
  self->continueButton = NULL; self->newGameButton = NULL; self->optionsButton = NULL; self->exitButton = NULL;
  self->masterVolumeValueLabel = NULL; self->musicVolumeValueLabel = NULL; self->effectsVolumeValueLabel = NULL; self->uiVolumeValueLabel = NULL; self->voiceVolumeValueLabel = NULL;
  self->resolutionLabel = NULL;
  self->optionsTabs = NULL; self->vsyncToggle = NULL; self->brightnessSlider = NULL; self->masterVolumeSlider = NULL; self->musicVolumeSlider = NULL; self->effectsVolumeSlider = NULL; self->uiVolumeSlider = NULL; self->voiceVolumeSlider = NULL; self->mouseSensitivitySlider = NULL; self->subtitlesToggle = NULL; self->highContrastToggle = NULL;
  self->applyButton = NULL; self->resetButton = NULL; self->backButton = NULL;
  self->resolutionPreviousButton = NULL; self->resolutionNextButton = NULL;
  Arcadia_LeaveConstructor(Arcadia_Games_Anima_MenuScene);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuSceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value))&updateVisualsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*))&handleMouseButtonEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*))&handleMousePointerEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*))&handleKeyboardKeyEventImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self
  )
{
  if (self->definitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  if (self->application) Arcadia_Object_visit(thread, (Arcadia_Object*)self->application);
  if (self->displayDevice) Arcadia_Object_visit(thread, (Arcadia_Object*)self->displayDevice);
  if (self->availableResolutionModes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->availableResolutionModes);
  if (self->availableResolutionLabels) Arcadia_Object_visit(thread, (Arcadia_Object*)self->availableResolutionLabels);
  if (self->cameraNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  if (self->ambienceNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambienceNode);
  if (self->enterPassNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  if (self->viewportNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  if (self->canvasNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->canvasNode);
  if (self->startRoot) Arcadia_Object_visit(thread, (Arcadia_Object*)self->startRoot);
  if (self->optionsRoot) Arcadia_Object_visit(thread, (Arcadia_Object*)self->optionsRoot);
  if (self->vsyncRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->vsyncRow);
  if (self->brightnessRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->brightnessRow);
  if (self->masterVolumeRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->masterVolumeRow);
  if (self->musicVolumeRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->musicVolumeRow);
  if (self->effectsVolumeRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->effectsVolumeRow);
  if (self->uiVolumeRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->uiVolumeRow);
  if (self->voiceVolumeRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->voiceVolumeRow);
  if (self->mouseSensitivityRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->mouseSensitivityRow);
  if (self->subtitlesRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->subtitlesRow);
  if (self->highContrastRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->highContrastRow);
  if (self->resolutionRow) Arcadia_Object_visit(thread, (Arcadia_Object*)self->resolutionRow);
}

static void
layoutRoot
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* root,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Integer32Value rootWidth,
    Arcadia_Integer32Value rootHeight
  )
{
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)root, (width - rootWidth) / 2, (height - rootHeight) / 2, rootWidth, rootHeight);
}

static void
updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  ensureScene(thread, self);
  layoutRoot(thread, self->startRoot, width, height, 420, 430);
  layoutRoot(thread, self->optionsRoot, width, height, 760, 560);
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, 0.f, (Arcadia_Real32Value)width, 0.f, (Arcadia_Real32Value)height, -1.f, 1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, (Arcadia_Real32Value)width, (Arcadia_Real32Value)height);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, NULL);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->enterPassNode, self->enterPassNode);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, self->canvasNode, width, height);
  Arcadia_Engine_UI_CanvasNode_updateVisuals(thread, self->canvasNode, self->enterPassNode);
}

static void
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  ensureScene(thread, self);
  Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)self->canvasNode, event);
}

static void
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  ensureScene(thread, self);
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)self->canvasNode, event);
}

static void
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Games_Anima_MenuScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  ensureScene(thread, self);
  Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)self->canvasNode, event);
}

Arcadia_Games_Anima_MenuScene*
Arcadia_Games_Anima_MenuScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager,
    Arcadia_Engine_Application* application
  )
{
  _Arcadia_BeginCreate(Arcadia_Games_Anima_MenuScene);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, engine);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sceneManager);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, application);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Games_Anima_MenuScene);
}
