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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_MODULE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Annotations/NoReturn.h"

#if Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC
#include <stddef.h>
#endif

#include "Arcadia/Ring1/Implementation/Process.h"

typedef struct Arcadia_JumpTarget Arcadia_JumpTarget; // Forward declaration.
typedef struct Arcadia_Thread Arcadia_Thread; // Forward declaration.
typedef struct Arcadia_Process Arcadia_Process; // Forward declaration.


typedef struct ModuleInfo {
  /// @brief Pointer to the name of the module. The name is a static constant C string.
  const char* name;
  /// @brief Visit the module.
  /// @warning Internal function.
  Arcadia_Process_ArenaVisitCallback* onVisit;
  /// @brief Startup the module.
  /// @warning Internal function.
  void (*onStartUp)(Arcadia_Thread* thread);
  /// @brief Shutdown the module.
  /// @warning Internal function.
  void (*onShutDown)(Arcadia_Thread* thread);
  /// @brief Must be invoked in the pre mark phase.
  /// @warning Internal function.
  Arcadia_Process_ArenaPreMarkCallback* onPreMark;
  /// @brief Must be invoked in the finalize phase.
  /// @warning Internal function.
  Arcadia_Process_ArenaFinalizeCallback* onFinalize;
} ModuleInfo;

#define Arcadia_DeclareModule(Name, cName) \
  const ModuleInfo* \
  cName##_getModule \
    ( \
    );

#define Arcadia_DefineModule(Name, cName) \
  static void \
  _##cName##_onStartUp \
    ( \
      Arcadia_Thread* thread \
    ); \
\
  static void \
  _##cName##_onShutDown \
    ( \
      Arcadia_Thread* thread \
    ); \
\
  static void \
  _##cName##_onPreMark \
    ( \
      Arcadia_Thread* thread, \
      bool purgeCache \
    ); \
\
  static void \
  _##cName##_onFinalize \
    ( \
      Arcadia_Thread* thread, \
      size_t* destroyed \
    ); \
\
  static void \
  _##cName##_onVisit \
    ( \
      Arcadia_Thread* thread \
    ); \
\
  static const ModuleInfo _##cName##_moduleInfo = { \
    .name = Name, \
    .onVisit = &_##cName##_onVisit, \
    .onStartUp = &_##cName##_onStartUp, \
    .onShutDown = &_##cName##_onShutDown, \
    .onPreMark = &_##cName##_onPreMark, \
    .onFinalize = &_##cName##_onFinalize, \
  }; \
\
  const ModuleInfo* \
  cName##_getModule \
    ( \
    ) \
  { return &_##cName##_moduleInfo; }

#endif // ARCADIA_RING1_IMPLEMENTATION_MODULE_H_INCLUDED
