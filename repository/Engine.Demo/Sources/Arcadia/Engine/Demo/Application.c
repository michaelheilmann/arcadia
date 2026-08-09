#include "Arcadia/Engine/Demo/Application.h"

#include "Arcadia/Engine/Demo/Configuration.h"
#include "Arcadia/Engine/Demo/Audials.h"
#include "Arcadia/Engine/Demo/Visuals.h"
#include <stdlib.h>

static void
Arcadia_Engine_Demo_Application_startupImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  );

static void
Arcadia_Engine_Demo_Application_shutdownImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  );

static void
Arcadia_Engine_Demo_Application_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  );

static void
Arcadia_Engine_Demo_Application_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ApplicationDispatch* self
  );

static void
Arcadia_Engine_Demo_Application_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_Application_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_Application_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_Application_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_Application_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_Application_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_Application_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.Application", Arcadia_Engine_Demo_Application,
                         u8"Arcadia.Engine.Application", Arcadia_Engine_Application,
                         &_Arcadia_Engine_Demo_Application_typeOperations);

static void
Arcadia_Engine_Demo_Application_startupImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  )
{
  // Startup visuals.
  Arcadia_Engine_Application_startupVisuals(thread, ((Arcadia_Engine_Application*)self)->engine, ((Arcadia_Engine_Application*)self)->configuration, ((Arcadia_Engine_Application*)self)->windows);
  // Startup audials.
  Arcadia_Engine_Application_startupAudials(thread, ((Arcadia_Engine_Application*)self)->engine, ((Arcadia_Engine_Application*)self)->configuration);
}

static void
Arcadia_Engine_Demo_Application_shutdownImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  )
{
  // Save the configuration.
  // This can raise errors in particular because of input/output operations.
  // We choose to ignore these errors and make sure that the cleanups below are performed.
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Cfg_saveConfiguration(thread, ((Arcadia_Engine_Application*)self)->configuration);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
  }

  // Ensure the windows are closed.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Application*)self)->windows); i < n; ++i) {
    Arcadia_Engine_Visuals_Window* window =
      (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueCheckedAt
        (
          thread,
          ((Arcadia_Engine_Application*)self)->windows,
          i,
          _Arcadia_Engine_Visuals_Window_getType(thread)
        );
    Arcadia_Engine_Visuals_Window_close(thread, window);
  }

  // Clean the message queue. FIXME: The messages prevent the engine from being gc'ed.
  while (Arcadia_Engine_dequeEvent(thread, ((Arcadia_Engine_Application*)self)->engine)) {
    Arcadia_logf(Arcadia_LogFlags_Info, "%s:%d: purging message\n", __FILE__, __LINE__);
  }

  // Release the audials backend context.
  if (((Arcadia_Engine_Application*)self)->engine && ((Arcadia_Engine_Application*)self)->engine->audialsBackendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)((Arcadia_Engine_Application*)self)->engine->audialsBackendContext);
    ((Arcadia_Engine_Application*)self)->engine->audialsBackendContext = NULL;
  }
  // Release the visuals backend context.
  if (((Arcadia_Engine_Application*)self)->engine && ((Arcadia_Engine_Application*)self)->engine->visualsBackendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)((Arcadia_Engine_Application*)self)->engine->visualsBackendContext);
    ((Arcadia_Engine_Application*)self)->engine->visualsBackendContext = NULL;
  }
}

static void
Arcadia_Engine_Demo_Application_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_Application);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  ((Arcadia_Engine_Application*)self)->configuration = (Arcadia_DDL_Node*)Cfg_loadConfiguration(thread);
  self->sceneManager = Arcadia_Engine_Demo_SceneManager_create(thread, ((Arcadia_Engine_Application*)self)->engine);
  self->sceneOnQuitRequestedSlot = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_Application);
}

static void
Arcadia_Engine_Demo_Application_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ApplicationDispatch* self
  )
{
  ((Arcadia_Engine_ApplicationDispatch*)self)->startup = (void (*)(Arcadia_Thread*,Arcadia_Engine_Application*))&Arcadia_Engine_Demo_Application_startupImpl;
  ((Arcadia_Engine_ApplicationDispatch*)self)->shutdown = (void (*)(Arcadia_Thread*, Arcadia_Engine_Application*)) & Arcadia_Engine_Demo_Application_shutdownImpl;
}

static void
Arcadia_Engine_Demo_Application_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self
  )
{
  if (self->sceneManager) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneManager);
  }
}

Arcadia_Engine_Demo_Application*
Arcadia_Engine_Demo_Application_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_Application);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Demo_Application);
}

static Arcadia_BooleanValue
filterWindows
  (
    Arcadia_Thread* thread,
    Arcadia_Value context,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isEqualTo(thread, &context, &value)) {
    return Arcadia_BooleanValue_False;
  } else {
    return Arcadia_BooleanValue_True;
  }
}

void
Arcadia_Engine_Demo_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  )
{
  Arcadia_List_filter(thread, ((Arcadia_Engine_Application*)self)->windows, Arcadia_Value_makeObjectReferenceValue(event->window), &filterWindows);
  Arcadia_Engine_Application_setQuitRequested(thread, (Arcadia_Engine_Application*)self, Arcadia_BooleanValue_True);
}
