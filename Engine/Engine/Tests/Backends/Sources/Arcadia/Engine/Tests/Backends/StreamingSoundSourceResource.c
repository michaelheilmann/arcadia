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

typedef struct StreamingResourceProbe StreamingResourceProbe;
typedef struct StreamingResourceProbeDispatch StreamingResourceProbeDispatch;
struct StreamingResourceProbe {
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource _parent;
  Arcadia_String* label;
  Arcadia_BooleanValue playing;
  Arcadia_Real32Value volume;
  Arcadia_SizeValue calls[6];
};
struct StreamingResourceProbeDispatch { Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch _parent; };
Arcadia_declareObjectType(u8"Tests.StreamingResourceProbe", StreamingResourceProbe, u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource");
static Arcadia_BooleanValue fail;
static Arcadia_SizeValue destroyed;

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* args,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &args[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    StreamingResourceProbe* self
  )
{
  Arcadia_EnterConstructor(StreamingResourceProbe);
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_ValueStack_pushValue(thread, &value); Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = NULL; self->playing = false; self->volume = 0.f;
  for (size_t i = 0; i < 6; ++i) self->calls[i] = 0;
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  self->label = Arcadia_String_createFromCxxString(thread, "streaming resource");
  Arcadia_LeaveConstructor(StreamingResourceProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    StreamingResourceProbe* self
  )
{ if (self->label) Arcadia_Object_visit(thread, (void*)self->label); }

static void
destructImpl
  (
    Arcadia_Thread* thread,
    StreamingResourceProbe* self
  )
{ destroyed++; }

static StreamingResourceProbe*
record
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* receiver,
    unsigned operation
  )
{
  if (fail) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed); Arcadia_Thread_jump(thread); }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  StreamingResourceProbe* self = (void*)receiver;
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "streaming resource", sizeof("streaming resource") - 1));
  self->calls[operation]++;
  return self;
}

static Arcadia_BooleanValue
isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{ return record(thread, self, 0)->playing; }

#define PLAYBACK(Name, Slot, State) \
  static void Name(Arcadia_Thread* thread, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self) { record(thread, self, Slot)->playing = State; }
PLAYBACK(play, 1, true)
PLAYBACK(pause, 2, false)
PLAYBACK(stop, 3, false)
#undef PLAYBACK

static void
setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self,
    Arcadia_Real32Value value
  )
{ record(thread, self, 4)->volume = value; }

static void
update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{ record(thread, self, 5); }

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    StreamingResourceProbeDispatch* self
  )
{
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* base = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self;
  base->isPlaying = &isPlaying; base->play = &play; base->pause = &pause; base->stop = &stop;
  base->setVolume = &setVolume; base->update = &update;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};
static const Arcadia_Type_Operations typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations };
Arcadia_defineObjectType(u8"Tests.StreamingResourceProbe", StreamingResourceProbe, u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource, &typeOperations);

static void
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  switch (operation) {
    case 0: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying(thread, self); break;
    case 1: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play(thread, self); break;
    case 2: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause(thread, self); break;
    case 3: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop(thread, self); break;
    case 4: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume(thread, self, .25f); break;
    case 5: Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_update(thread, self); break;
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
  Arcadia_Object* context = create(thread, _Arcadia_Engine_Audials_BackendContext_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_Value args[] = { Arcadia_Value_makeObjectReferenceValue(context), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void) };
  Arcadia_TypeValue type = _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread);
  // Reject bad arity before reading/forwarding any argument.
  for (unsigned variant = 0; variant < 5; ++variant) {
    Arcadia_Value bad[] = { args[0], args[1] };
    Arcadia_Natural8Value count = variant == 0 ? 0 : variant == 1 ? 2 : 1;
    if (variant == 2) bad[0] = args[1];
    if (variant == 3) bad[0] = Arcadia_Value_makeNatural32Value(1);
    if (variant == 0) Arcadia_ValueStack_pushNatural32Value(thread, 99); // Caller canary is not an argument.
    if (variant == 4) {
      bad[0] = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, "wrong context"));
      Arcadia_ValueStack_pushValue(thread, &bad[0]);
    }
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      create(thread, type, bad, count);
      Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (variant < 2 ? Arcadia_Status_NumberOfArgumentsInvalid : Arcadia_Status_ArgumentTypeInvalid) && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
    if (variant == 0 || variant == 4) Arcadia_ValueStack_popValues(thread, 1);
  }
  StreamingResourceProbe* self = (void*)create(thread, _StreamingResourceProbe_getType(thread), args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Audials_Implementation_Resource*)self)->context == (void*)context && ((Arcadia_Engine_Audials_Implementation_Resource*)self)->referenceCount == 0);
  for (unsigned operation = 0; operation < 6; ++operation) call(thread, operation, (void*)self);
  for (size_t i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, self->calls[i] == 1);
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying(thread, (void*)self));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying(thread, (void*)self));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop(thread, (void*)self);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying(thread, (void*)self));
  Arcadia_Real32Value volumes[] = { 0.f, -0.f, -1.f, 2.f, INFINITY, -INFINITY, NAN };
  for (size_t i = 0; i < sizeof(volumes) / sizeof(volumes[0]); ++i) {
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume(thread, (void*)self, volumes[i]);
    Arcadia_Tests_assertTrue(thread, isnan(volumes[i]) ? isnan(self->volume) : self->volume == volumes[i]);
    if (self->volume == 0.f) Arcadia_Tests_assertTrue(thread, !!signbit(self->volume) == !!signbit(volumes[i]));
  }
  Arcadia_Object* unbound = create(thread, type, args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  for (unsigned operation = 0; operation < 6; ++operation) {
    for (unsigned receiver = 0; receiver < 4; ++receiver) {
      fail = receiver == 0;
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        call(thread, operation, receiver == 0 ? (void*)self : receiver == 1 ? (void*)unbound : receiver == 2 ? NULL : (void*)context);
        Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (receiver == 0 ? Arcadia_Status_OperationFailed : receiver == 1 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid) && Arcadia_ValueStack_getSize(thread) == depth);
        Arcadia_Thread_setStatus(thread, 0);
      }
      fail = false;
    }
  }
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_streamingSoundSourceResource
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
