#if !defined(ARCADIA_RING2_COMMANDLINE_COMMANDLINEARGUMENT_H_INCLUDED)
#define ARCADIA_RING2_COMMANDLINE_COMMANDLINEARGUMENT_H_INCLUDED

#include "Arcadia/Ring2/Strings/Include.h"

#define Arcadia_CommandLineArgumentError_MissingDoubleDash (1)
#define Arcadia_CommandLineArgumentError_InvalidName (2)
#define Arcadia_CommandLineArgumentError_InvalidValue (4)

/// For arguments of the form `--<name>` @code{key} is assigned `<name>` and @code{value} is assinged the null pointer.
/// For arguments of the form `--<name>=<value>` @code{key} is assigned `<name>` and @code{value} is assigned `<value>`.
Arcadia_declareObjectType(u8"Arcadia.Shell.CommandLineArgument", Arcadia_CommandLineArgument,
                          u8"Arcadia.Object");

struct Arcadia_CommandLineArgumentDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_CommandLineArgument {
  Arcadia_Object _parent;

  /// #Arcadia_BooleanValue_True if the command-line argument has a syntactical error.
  /// #Arcadia_BooleanValue_False otherwise.
  Arcadia_BooleanValue syntacticalError;

  /// The name. Must not be null of syntacticalError is #Arcadia_BooleanValue_False.
  /// Must be null if syntacticalError is #Arcadia_BooleanValue_True.
  Arcadia_String* name;

  /// The value. May be null.
  Arcadia_String* value;
};

Arcadia_CommandLineArgument*
Arcadia_CommandLineArgument_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue syntacticalError,
    Arcadia_String* name,
    Arcadia_String* value
  );

#endif // ARCADIA_RING2_COMMANDLINE_COMMANDLINEARGUMENT_H_INCLUDED
