#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_FUNCTIONS_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_FUNCTIONS_H_INCLUDED

struct _Arcadia_Visuals_Implementation_OpenGL4_GLX_Functions {
#define Define(Type, Name) \
  Type Name;
#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/Functions.i"
#undef Define
};

#endif // ARCADIA_VISUALS_IMPLEMENTATION_OPENGL4_GLX_FUNCTIONS_H_INCLUDED
