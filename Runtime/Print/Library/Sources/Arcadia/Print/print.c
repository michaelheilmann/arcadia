#include "Arcadia/Print/print.h"

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Ring2/Include.h"

/// @error Arcadia.Status.AllocationFailed
/// @error Arcadia.Status.ArgumentTypeInvalid
static inline Arcadia_FileHandle* getFileHandle(Arcadia_Thread* thread, Arcadia_SizeValue index) {
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index);
  if (!Arcadia_Value_isObjectReferenceValue(&value)) {
    //Arcadia_Diagnostics_logValueStackElement(thread, Arcadia_LogFlags_Debug, index);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* objectValue = Arcadia_Value_getObjectReferenceValue(&value);
  if (!Arcadia_Object_isInstanceOf(thread, objectValue, _Arcadia_FileHandle_getType(thread))) {
    //Arcadia_Diagnostics_logValueStackElement(thread, Arcadia_LogFlags_Debug, index);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_FileHandle*)objectValue;
}

/// @error Arcadia.Status.AllocationFailed
/// @error Arcadia.Status.ArgumentTypeInvalid
static inline Arcadia_String* getString(Arcadia_Thread* thread, Arcadia_SizeValue index) {
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index);
  if (!Arcadia_Value_isObjectReferenceValue(&value)) {
    //Arcadia_Diagnostics_logValueStackElement(thread, Arcadia_LogFlags_Debug, index);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* objectValue = Arcadia_Value_getObjectReferenceValue(&value);
  if (!Arcadia_Object_isInstanceOf(thread, objectValue, _Arcadia_String_getType(thread))) {
    //Arcadia_Diagnostics_logValueStackElement(thread, Arcadia_LogFlags_Debug, index);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_String*)objectValue;
}

/// @error Arcadia.Status.AllocationFailed
/// @error Arcadia.Status.ArgumentTypeInvalid
/// @error Arcadia.Status.OperationFailed
static inline void
printString
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* fileHandle,
    Arcadia_String* string
  )
{
  Arcadia_SizeValue bytesToWrite = Arcadia_String_getNumberOfBytes(thread, string), bytesWritten = 0;
  Arcadia_Natural8Value const* bytes = Arcadia_String_getBytes(thread, string);
  while (bytesToWrite > bytesWritten) {
    Arcadia_SizeValue bytesWritteNow = 0;
    Arcadia_FileHandle_write(thread, fileHandle, bytes + bytesWritten, bytesToWrite - bytesWritten, &bytesWritteNow);
    bytesWritten += bytesWritteNow;
  }
}

/// @error Arcadia.Status.StackCorruption calling convention was violated
/// @error Arcadia.Status.AllocationFailed
/// @error Arcadia.Status.ArgumentTypeInvalid
/// @error Arcadia.Status.OperationFailed
void
Arcadia_print
  (
    Arcadia_Thread* thread
  )
{
  // (1) read & validate stack size.
  // Arcadia_Status_StackCorruption is raised.
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  if (stackSize < 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
  }
  Arcadia_SizeValue numberOfArguments;

  Arcadia_JumpTarget jumpTarget;
  // (2) read & validate number of arguments.
  // Arcadia_Status_StackCorruption is raised if the number of arguments cannot be read.
  // Arcadia_Status_NumberOfArguments is raised if the number of arguments is smaller than 1.
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  if (numberOfArguments < 1) {
    // Number of arguments.
    Arcadia_ValueStack_popValues(thread, numberOfArguments + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_FileHandle* fileHandle = getFileHandle(thread, 1);
    // (3) print arguments arguments
    for (Arcadia_SizeValue i = numberOfArguments - 1; i > 0; --i) {
      Arcadia_String* string = getString(thread, 2 + i - 1);
      printString(thread, fileHandle, string);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, numberOfArguments + 1);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, numberOfArguments + 1);
    Arcadia_Thread_jump(thread);
  }
}

