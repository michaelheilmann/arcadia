#if !defined(ARCADIA_PRINT_PRINT_H_INCLUDED)
#define ARCADIA_PRINT_PRINT_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

/// Expects the number of arguments on the stack followed by the arguments.
/// The first argument must be an Arcadia.FileHandle. The remaining arguments are Arcadia.String.
/// Prints the numberOfArguments - 1 strings to the file handle. Removes all arguments and the number of arguments from the stack.
void
Arcadia_print
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PRINT_PRINT_H_INCLUDED
