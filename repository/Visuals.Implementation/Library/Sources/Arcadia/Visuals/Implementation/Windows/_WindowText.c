#include "Arcadia/Visuals/Implementation/Windows/_WindowText.h"

#if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
  #define LPARAM_MAX INT64_MAX
  #define LRESULT_MAX INT64_MAX
#elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
  #define LPARAM_MAX LONG_MAX
  #define LRESULT_MAX LONG_MAX
#else
  #error("unsupported/unknown instruction set architecture")
#endif

Arcadia_String*
_getWindowText
  (
    Arcadia_Thread* thread,
    HWND hWnd
  )
{
  SetLastError(0);

  LRESULT lResult;
  // Get the length of the text EXCLUDING the zero terminator.
  lResult = SendMessageA(hWnd, WM_GETTEXT, (WPARAM)0, (LPARAM)0);
  if (0 == lResult && GetLastError()) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (lResult < 0 || lResult > Arcadia_SizeValue_Maximum - 1) {
    // Sanity check:
    // a) Negative values do not make sense.
    // b) We must be able to convert the value into an Arcadia_SizeValue value and then add +1.
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  // Allocate a buffer for the text plus the zero terminator.
  Arcadia_SizeValue bufferSize = ((Arcadia_SizeValue)lResult) + Arcadia_SizeValue_Literal(1);
  char* buffer = malloc(bufferSize);
  if (!buffer) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  // Get the text INCLUDING the zero terminator.
  if (bufferSize > LPARAM_MAX) {
    free(buffer);
    buffer = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  lResult = SendMessageA(hWnd, (int)WM_GETTEXT, (WPARAM)buffer, (LPARAM)bufferSize);
  if (0 == lResult && GetLastError()) {
    free(buffer);
    buffer = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* text = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    text = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, buffer, bufferSize - 1)));
    free(buffer);
    buffer = NULL;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(buffer);
    buffer = NULL;
    Arcadia_Thread_jump(thread);
  }
  return text;
}

void
_setWindowText
  (
    Arcadia_Thread* thread,
    HWND hWnd,
    Arcadia_String* text
  )
{
  Arcadia_Natural32Value zeroTerminator = 0;
  Arcadia_StringBuilder* buffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, buffer, Arcadia_Value_makeObjectReferenceValue(text));
  Arcadia_StringBuilder_insertBackCodePoints(thread, buffer, &zeroTerminator, 1);
  SendMessageA(hWnd, WM_SETTEXT, (WPARAM)0, (LPARAM)Arcadia_StringBuilder_getBytes(thread, buffer));
}
