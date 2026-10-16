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
#include <stddef.h>

typedef Arcadia_BooleanValue
ProviderRead
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* written
  );
struct ProviderDispatchModel { Arcadia_ObjectDispatch base; ProviderRead* read; };
struct ProviderModel { Arcadia_Object base; Arcadia_Natural32Value sampleRate; };
_Static_assert(sizeof(struct ProviderDispatchModel) == sizeof(Arcadia_Engine_Audials_AudioStreamProviderDispatch), "sample rate getter must remain nonvirtual");
_Static_assert(offsetof(struct ProviderDispatchModel, read) == offsetof(Arcadia_Engine_Audials_AudioStreamProviderDispatch, read), "read slot ABI");
_Static_assert(sizeof(struct ProviderModel) == sizeof(Arcadia_Engine_Audials_AudioStreamProvider), "provider storage ABI");
_Static_assert(offsetof(struct ProviderModel, sampleRate) == offsetof(Arcadia_Engine_Audials_AudioStreamProvider, sampleRate), "sample rate ABI");
_Static_assert(_Generic(&Arcadia_Engine_Audials_AudioStreamProvider_read, ProviderRead*: 1, default: 0), "read signature ABI");
_Static_assert(_Generic(&Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate,
  Arcadia_Natural32Value (*)(Arcadia_Thread*, Arcadia_Engine_Audials_AudioStreamProvider*): 1, default: 0), "sample rate getter ABI");

typedef struct StreamProviderProbe StreamProviderProbe;
typedef struct StreamProviderProbeDispatch StreamProviderProbeDispatch;
struct StreamProviderProbe {
  Arcadia_Engine_Audials_AudioStreamProvider _parent;
  Arcadia_String* label;
  Arcadia_BooleanValue eof;
  Arcadia_SizeValue written;
  Arcadia_SizeValue calls;
  Arcadia_Media_DSP_Buffer* last; // Borrowed capture, not a GC root.
};
struct StreamProviderProbeDispatch { Arcadia_Engine_Audials_AudioStreamProviderDispatch _parent; };
Arcadia_declareObjectType(u8"Tests.StreamProviderProbe", StreamProviderProbe, u8"Arcadia.Engine.Audials.AudioStreamProvider");
static Arcadia_BooleanValue fail;
static Arcadia_SizeValue destroyed;

static Arcadia_Object*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    StreamProviderProbe* self
  )
{
  Arcadia_EnterConstructor(StreamProviderProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->label = NULL; self->last = NULL; self->eof = false; self->written = 3; self->calls = 0;
  ((Arcadia_Engine_Audials_AudioStreamProvider*)self)->sampleRate = 8000;
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  self->label = Arcadia_String_createFromCxxString(thread, "stream provider");
  Arcadia_LeaveConstructor(StreamProviderProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    StreamProviderProbe* self
  )
{ if (self->label) Arcadia_Object_visit(thread, (void*)self->label); }

static void
destructImpl
  (
    Arcadia_Thread* thread,
    StreamProviderProbe* self
  )
{ destroyed++; }

static Arcadia_BooleanValue
readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* receiver,
    Arcadia_Media_DSP_Buffer* target,
    Arcadia_SizeValue* written
  )
{
  if (fail) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed); Arcadia_Thread_jump(thread); }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  StreamProviderProbe* self = (void*)receiver;
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "stream provider", sizeof("stream provider") - 1));
  if (target) {
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, target) == 3 && Arcadia_Media_DSP_Buffer_getSampleRate(thread, target) == 8000);
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
    samples[0] = -1.f; samples[1] = .25f; samples[2] = 1.f;
  }
  self->last = target; self->calls++;
  if (written) *written = self->eof ? 0 : self->written;
  return !self->eof;
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    StreamProviderProbeDispatch* self
  )
{ ((Arcadia_Engine_Audials_AudioStreamProviderDispatch*)self)->read = &readImpl; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};
static const Arcadia_Type_Operations typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations };
Arcadia_defineObjectType(u8"Tests.StreamProviderProbe", StreamProviderProbe, u8"Arcadia.Engine.Audials.AudioStreamProvider", Arcadia_Engine_Audials_AudioStreamProvider, &typeOperations);

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  StreamProviderProbe* self = (void*)create0(thread, _StreamProviderProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Natural32Value rates[] = { 0, 1, 8000, 48000, UINT32_MAX };
  for (size_t i = 0; i < sizeof(rates) / sizeof(rates[0]); ++i) {
    ((Arcadia_Engine_Audials_AudioStreamProvider*)self)->sampleRate = rates[i];
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate(thread, (void*)self) == rates[i]);
  }
  Arcadia_Media_DSP_Buffer* target = Arcadia_Media_DSP_Buffer_create(thread, 8000, 3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, target);
  Arcadia_SizeValue written = 0;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (void*)self, target, &written) && written == 3 && self->last == target);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, target);
  Arcadia_Tests_assertTrue(thread, samples[0] == -1.f && samples[1] == .25f && samples[2] == 1.f);
  self->written = Arcadia_SizeValue_Maximum;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (void*)self, target, &written) && written == Arcadia_SizeValue_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (void*)self, NULL, NULL));
  self->eof = true;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (void*)self, target, &written) && written == 0);
  Arcadia_Object* unbound = create0(thread, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  for (unsigned receiver = 0; receiver < 4; ++receiver) {
    fail = receiver == 0;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_Audials_AudioStreamProvider_read(thread, receiver == 0 ? (void*)self : receiver == 1 ? (void*)unbound : receiver == 2 ? NULL : (void*)target, target, &written);
      Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (receiver == 0 ? Arcadia_Status_OperationFailed : receiver == 1 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid) && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
    fail = false;
  }
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 1); Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread), depth);
    Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid && Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Thread_setStatus(thread, 0);
  }
  StreamProviderProbe* temporary = (void*)create0(thread, _StreamProviderProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, temporary);
  Arcadia_Media_DSP_Buffer* temporaryTarget = Arcadia_Media_DSP_Buffer_create(thread, 8000, 3);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (void*)temporary, temporaryTarget, &written) && temporary->last == temporaryTarget);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_SizeValue before = destroyed;
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == before + 1 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_audioStreamProvider
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
