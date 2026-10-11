#include "Arcadia/Visuals/Implementation/Windows/_CharConv.h"

Arcadia_String*
_fromMultiByte
  (
    Arcadia_Thread* thread,
    CONST CHAR* p,
    size_t n
  )
{
  return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, p, n)));
}

Arcadia_String*
_fromWideChar
  (
    Arcadia_Thread* thread,
    CONST WCHAR* p,
    size_t n
  )
{
  int m = WideCharToMultiByte(CP_UTF8, 0, p, -1, NULL, 0, NULL, NULL);
  if (!m) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  CHAR* q = malloc(sizeof(char) * m);
  if (!q) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!WideCharToMultiByte(CP_UTF8, 0, p, -1, q, m, NULL, NULL)) {
    free(q);
    q = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* s;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    s = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, q, m)));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(q);
    q = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  free(q);
  q = NULL;
  return s;
}

BOOL
_multiByteToWideChar
  (
    const char* p,
    size_t n,
    wchar_t** q,
    size_t* m
  )
{
  int l = MultiByteToWideChar(CP_ACP, 0, p, n, NULL, 0);
  if (l <= 0) {
    return FALSE;
  }
  wchar_t* r = malloc(sizeof(wchar_t) * l);
  if (!r) {
    return FALSE;
  }
  if (MultiByteToWideChar(CP_ACP, 0, p, n, r, l) <= 0) {
    free(r);
    r = NULL;
    return FALSE;
  }
  *q = r;
  *m = l;
  return TRUE;
}
