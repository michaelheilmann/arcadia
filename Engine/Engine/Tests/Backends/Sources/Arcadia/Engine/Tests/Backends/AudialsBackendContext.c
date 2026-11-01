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

#include "Arcadia/Engine/Tests/Backends/Cases.h"
#include "Arcadia/Engine/Include.h"
#include <math.h>

typedef struct AudialsContextProbe AudialsContextProbe;
typedef struct AudialsContextProbeDispatch AudialsContextProbeDispatch;

struct AudialsContextProbe {
  Arcadia_Engine_Audials_BackendContext _parent;
  Arcadia_String* label;
  Arcadia_Media_SampleBuffer* buffer;
  Arcadia_Engine_Audials_AudioStreamProvider* provider;
  Arcadia_Real64Value volume;
  Arcadia_SizeValue calls[4];
  Arcadia_BooleanValue fail;
  Arcadia_BooleanValue returnNull;
};

struct AudialsContextProbeDispatch {
  Arcadia_Engine_Audials_BackendContextDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.AudialsContextProbe", AudialsContextProbe,
                          u8"Arcadia.Engine.Audials.BackendContext");

static Arcadia_SizeValue destroyed;

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Object* argument
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  if (argument) Arcadia_ValueStack_pushObjectReferenceValue(thread, argument);
  Arcadia_ValueStack_pushNatural8Value(thread, argument ? 1 : 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    AudialsContextProbe* self
  )
{
  Arcadia_EnterConstructor(AudialsContextProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->buffer = NULL;
  self->provider = NULL;
  self->volume = 0.;
  for (size_t i = 0; i < 4; ++i) self->calls[i] = 0;
  self->fail = Arcadia_BooleanValue_False;
  self->returnNull = Arcadia_BooleanValue_False;
  self->label = Arcadia_String_createFromCxxString(thread, "audials context payload");
  Arcadia_LeaveConstructor(AudialsContextProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    AudialsContextProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
  if (self->buffer) Arcadia_Object_visit(thread, (Arcadia_Object*)self->buffer);
  if (self->provider) Arcadia_Object_visit(thread, (Arcadia_Object*)self->provider);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    AudialsContextProbe* self
  )
{ destroyed++; }

static void
enterCallback
  (
    Arcadia_Thread* thread,
    AudialsContextProbe* self,
    Arcadia_SizeValue operation
  )
{
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "audials context payload", sizeof("audials context payload") - 1));
  self->calls[operation]++;
}

static Arcadia_Engine_Audials_Implementation_SoundSourceResource*
createSound
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* receiver,
    Arcadia_Media_SampleBuffer* buffer
  )
{
  AudialsContextProbe* self = (void*)receiver;
  enterCallback(thread, self, 0);
  if (buffer) Arcadia_Tests_assertTrue(thread, Arcadia_Media_SampleBuffer_getSampleRate(thread, buffer) == 8000);
  self->buffer = buffer;
  return self->returnNull ? NULL : (void*)create(thread, _Arcadia_Engine_Audials_Implementation_SoundSourceResource_getType(thread), (Arcadia_Object*)self);
}

static Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*
createStreaming
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* receiver,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  )
{
  AudialsContextProbe* self = (void*)receiver;
  enterCallback(thread, self, 1);
  if (provider) Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate(thread, provider) == 48000);
  self->provider = provider;
  return self->returnNull ? NULL : (void*)create(thread, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread), (Arcadia_Object*)self);
}

static void
setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* receiver,
    Arcadia_Real64Value volume
  )
{
  AudialsContextProbe* self = (void*)receiver;
  enterCallback(thread, self, 2);
  self->volume = volume;
}

static void
update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_BackendContext* receiver
  )
{ enterCallback(thread, (void*)receiver, 3); }

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    AudialsContextProbeDispatch* self
  )
{
  Arcadia_Engine_Audials_BackendContextDispatch* dispatch = (void*)self;
  dispatch->createSoundSourceResource = &createSound;
  dispatch->createStreamingSoundSourceResource = &createStreaming;
  dispatch->setMasterVolume = &setVolume;
  ((Arcadia_Engine_BackendContextDispatch*)self)->update = &update;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.AudialsContextProbe", AudialsContextProbe,
                         u8"Arcadia.Engine.Audials.BackendContext", Arcadia_Engine_Audials_BackendContext,
                         &typeOperations);

static void
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Audials_BackendContext* self,
    Arcadia_Object* argument
  )
{
  switch (operation) {
    case 0: Arcadia_Engine_Audials_BackendContext_createSoundSourceResource(thread, self, (void*)argument); break;
    case 1: Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource(thread, self, (void*)argument); break;
    case 2: Arcadia_Engine_Audials_BackendContext_setMasterVolume(thread, self, 0.25); break;
    case 3: Arcadia_Engine_BackendContext_update(thread, (void*)self); break;
  }
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Audials_BackendContext* self,
    Arcadia_Object* argument,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    call(thread, operation, self, argument);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_TypeValue type = _Arcadia_Engine_Audials_BackendContext_getType(thread);
  Arcadia_Engine_Audials_BackendContextDispatch* root = (void*)Arcadia_ObjectType_getDispatch(type);
  Arcadia_Tests_assertTrue(thread, !root->createSoundSourceResource && !root->createStreamingSoundSourceResource && root->setMasterVolume);
  Arcadia_Engine_Audials_BackendContext* unbound = (void*)create(thread, type, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  Arcadia_Real64Value volumes[] = { 0., -0., 0.25, 1., -1., 2., INFINITY, -INFINITY, NAN };
  for (size_t i = 0; i < sizeof(volumes) / sizeof(volumes[0]); ++i) {
    Arcadia_Engine_Audials_BackendContext_setMasterVolume(thread, unbound, volumes[i]);
  }
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget arity;
  Arcadia_Thread_pushJumpTarget(thread, &arity);
  if (Arcadia_JumpTarget_save(&arity)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 1);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, type, depth);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Thread_setStatus(thread, 0);
  }
  AudialsContextProbe* self = (void*)create(thread, _AudialsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_BackendContext_getType(thread)));
  for (size_t i = 0; i < sizeof(volumes) / sizeof(volumes[0]); ++i) {
    Arcadia_Engine_Audials_BackendContext_setMasterVolume(thread, (void*)self, volumes[i]);
    Arcadia_Tests_assertTrue(thread, isnan(volumes[i]) ? isnan(self->volume) : self->volume == volumes[i]);
    if (volumes[i] == 0.) Arcadia_Tests_assertTrue(thread, !!signbit(self->volume) == !!signbit(volumes[i]));
  }
  Arcadia_Engine_BackendContext_update(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, self->calls[2] == sizeof(volumes) / sizeof(volumes[0]) && self->calls[3] == 1);
  Arcadia_Media_SampleBuffer* buffer = Arcadia_Media_SampleBuffer_create(thread, 1, 8000, Arcadia_Media_SampleFormat_Integer16);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
  Arcadia_Engine_Audials_AudioStreamProvider* provider = (void*)create(thread, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread), NULL);
  provider->sampleRate = 48000;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, provider);
  for (unsigned nullable = 0; nullable < 2; ++nullable) {
    Arcadia_Engine_Audials_Implementation_SoundSourceResource* sound = Arcadia_Engine_Audials_BackendContext_createSoundSourceResource(thread, (void*)self, nullable ? NULL : buffer);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, sound);
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* streaming = Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource(thread, (void*)self, nullable ? NULL : provider);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, streaming);
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Audials_Implementation_Resource*)sound)->context == (void*)self);
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Audials_Implementation_Resource*)streaming)->context == (void*)self);
    Arcadia_Tests_assertTrue(thread, self->buffer == (nullable ? NULL : buffer) && self->provider == (nullable ? NULL : provider));
    Arcadia_ValueStack_popValues(thread, 2);
  }
  self->returnNull = Arcadia_BooleanValue_True;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Audials_BackendContext_createSoundSourceResource(thread, (void*)self, buffer));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource(thread, (void*)self, provider));
  self->fail = Arcadia_BooleanValue_True;
  for (unsigned op = 0; op < 4; ++op) expectFailure(thread, op, (void*)self, NULL, Arcadia_Status_OperationFailed);
  self->fail = Arcadia_BooleanValue_False;
  for (unsigned op = 0; op < 3; ++op) {
    expectFailure(thread, op, NULL, NULL, Arcadia_Status_ArgumentTypeInvalid);
    expectFailure(thread, op, (void*)self->label, NULL, Arcadia_Status_ArgumentTypeInvalid);
  }
  for (unsigned op = 0; op < 2; ++op) {
    expectFailure(thread, op, unbound, NULL, Arcadia_Status_OperationInvalid);
  }
  // The wrapper alone roots the new receiver and argument during callback GC.
  AudialsContextProbe* temporary = (void*)create(thread, _AudialsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, temporary);
  Arcadia_Media_SampleBuffer* temporaryBuffer = Arcadia_Media_SampleBuffer_create(thread, 1, 8000, Arcadia_Media_SampleFormat_Integer16);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_Audials_Implementation_SoundSourceResource* temporarySound = Arcadia_Engine_Audials_BackendContext_createSoundSourceResource(thread, (void*)temporary, temporaryBuffer);
  Arcadia_Tests_assertTrue(thread, temporary->buffer == temporaryBuffer && temporarySound);
  temporary = (void*)create(thread, _AudialsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, temporary);
  Arcadia_Engine_Audials_AudioStreamProvider* temporaryProvider = (void*)create(thread, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread), NULL);
  temporaryProvider->sampleRate = 48000;
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* temporaryStreaming = Arcadia_Engine_Audials_BackendContext_createStreamingSoundSourceResource(thread, (void*)temporary, temporaryProvider);
  Arcadia_Tests_assertTrue(thread, temporary->provider == temporaryProvider && temporaryStreaming);
  Arcadia_ValueStack_popValues(thread, 4);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  destroyed = 0;
  self = (void*)create(thread, _AudialsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Engine_BackendContext_update(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, destroyed == 0 && self->calls[3] == 1);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_audialsBackendContext
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
