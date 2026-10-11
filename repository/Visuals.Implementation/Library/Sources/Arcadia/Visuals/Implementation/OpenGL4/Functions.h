#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_FUNCTIONS_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_FUNCTIONS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"

// unmanaged
typedef struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions;
// unmanaged
typedef struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker;

struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker {
  void* (*link)(Arcadia_Thread* thread, _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker* linker, const char* name);
};

struct _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions {
#define Define(Type, Name) \
  Type Name;
#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.i"
#undef Define
};

/// @error functions is a null pointer
void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_initialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* functions,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker* linker
  );

/// @error functions is a null pointer
void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_uninitialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* functions
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_FUNCTIONS_H_INCLUDED
