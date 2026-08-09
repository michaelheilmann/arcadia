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

#if !defined(ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_BACKENDCONTEXT_H_INCLUDED

#include "Arcadia/Engine/Include.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#include <math.h>

/// @code
/// class Arcadia.Engine.Audials.Implementation.OpenAL.BackendContext extends Arcadia.Engine.Audials.BackendContext {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.BackendContext", Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext,
                          u8"Arcadia.Engine.Audials.BackendContext");

struct Arcadia_Engine_Audials_Implementation_OpenAL_BackendContextDispatch {
  Arcadia_Engine_Audials_BackendContextDispatch _parent;
};

struct Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext {
  Arcadia_Engine_Audials_BackendContext _parent;

  // The list of resources.
  Arcadia_List* resources;

  ALCdevice* alcDevice;
  ALCcontext* alcContext;
};

// @todo Make private.
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_getOrCreate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_OPENAL_BACKENDCONTEXT_H_INCLUDED
