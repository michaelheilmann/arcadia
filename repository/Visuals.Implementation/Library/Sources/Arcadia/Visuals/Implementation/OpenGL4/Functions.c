#include "Arcadia/Visuals/Implementation/OpenGL4/Functions.h"

void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_initialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* functions,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Linker* linker
  )
{
#define Define(Type, Name) \
  functions->Name = NULL;
  #include "Arcadia/Visuals/Implementation/OpenGL4/Functions.i"
#undef Define

#define Define(Type, Name) \
  functions->Name = (Type)(*linker->link)(thread, linker, #Name); \
  if (!functions->Name) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed); \
    Arcadia_Thread_jump(thread); \
  }

  #include "Arcadia/Visuals/Implementation/OpenGL4/Functions.i"

#undef Define
}

void
_Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions_uninitialize
  (
    Arcadia_Thread* thread,
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* functions
  )
{
#define Define(Type, Name) \
  functions->Name = NULL;
  #include "Arcadia/Visuals/Implementation/OpenGL4/Functions.i"
#undef Define
}
