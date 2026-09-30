#if !defined(ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED

#include "Arcadia/DDL/Include.h"
#include "Arcadia/Engine/Include.h"

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
  /// @brief #Arcadia_BooleanValue_True if a screenshot was requested and not yet written.
  /// A screenshot can only be captured between Window_beginRender and Window_endRender,
  /// hence a request made by an input event is fulfilled during the next rendering pass.
  /// Default is #Arcadia_BooleanValue_False.
  Arcadia_BooleanValue screenshotRequested;
  /// @brief The number of screenshots written so far.
  /// Default is 0.
  Arcadia_Natural32Value screenshotCount;
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
