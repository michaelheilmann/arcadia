#if !defined(ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED

#include "Arcadia/DDL/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Demo/Scene.h"
#include "Arcadia/Engine/Demo/SceneManager.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.Application", Arcadia_Engine_Demo_Application,
                          u8"Arcadia.Engine.Application");

struct Arcadia_Engine_Demo_ApplicationDispatch {
  Arcadia_Engine_ApplicationDispatch parent;
};

struct Arcadia_Engine_Demo_Application {
  Arcadia_Engine_Application parent;
  /// @brief A pointer to the signal slot or null.
  Arcadia_Slot* sceneOnQuitRequestedSlot;
  /// @brief A pointer to the scene manager.
  Arcadia_Engine_Demo_SceneManager* sceneManager;
};

Arcadia_Engine_Demo_Application*
Arcadia_Engine_Demo_Application_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_Demo_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  );

#endif // ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED
