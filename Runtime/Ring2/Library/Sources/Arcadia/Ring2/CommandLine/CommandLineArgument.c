#define ARCADIA_RING2_MODULE (1)
#include "Arcadia/Ring2/CommandLine/CommandLineArgument.h"

static void
Arcadia_CommandLineArgument_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  );

static void
Arcadia_CommandLineArgument_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgumentDispatch* self
  );

static void
Arcadia_CommandLineArgument_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  );

static void
Arcadia_CommandLineArgument_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_CommandLineArgument_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_CommandLineArgument_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_CommandLineArgument_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_CommandLineArgument_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.CommandLineArgument", Arcadia_CommandLineArgument,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

void
Arcadia_CommandLineArgument_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  )
{
  Arcadia_EnterConstructor(Arcadia_CommandLineArgument);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->syntacticalError = Arcadia_ValueStack_getBooleanValue(thread, 3);
  if (!Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  } else {
    self->name = NULL;
  }
  if (!Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->value = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  } else {
    self->value = NULL;
  }
  Arcadia_LeaveConstructor(Arcadia_CommandLineArgument);
}

static void
Arcadia_CommandLineArgument_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgumentDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_CommandLineArgument_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_CommandLineArgument_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CommandLineArgument* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_CommandLineArgument*
Arcadia_CommandLineArgument_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue syntacticalError,
    Arcadia_String* name,
    Arcadia_String* value
  )
{
  _Arcadia_BeginCreate(Arcadia_CommandLineArgument);
  Arcadia_ValueStack_pushBooleanValue(thread, syntacticalError);
  if (name) Arcadia_ValueStack_pushObjectReferenceValue(thread, name); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (value) Arcadia_ValueStack_pushObjectReferenceValue(thread, value); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_CommandLineArgument);
}
