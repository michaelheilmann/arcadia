#if !defined(ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Engine_Demo_SceneManager Arcadia_Engine_Demo_SceneManager;

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Demo_SceneDispatch {
  Arcadia_ObjectDispatch parent;

  void (*updateAudials)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*updateLogics)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value tick);
  void (*updateVisuals)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);

  void (*handleKeyboardKeyEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*);
  void (*handleMouseButtonEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MouseButtonEvent*);
  void (*handleMousePointerEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MousePointerEvent*);
};

struct Arcadia_Engine_Demo_Scene {
  Arcadia_Object parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_Demo_SceneManager* sceneManager;
  /// The "application quit request" event.
  Arcadia_Signal* applicationQuitRequestSignal;
};

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick
  );

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Demo_Scene_handleKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

void
Arcadia_Engine_Demo_Scene_handleMouseButtonEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

void
Arcadia_Engine_Demo_Scene_handleMousePointerEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

#endif // ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED
