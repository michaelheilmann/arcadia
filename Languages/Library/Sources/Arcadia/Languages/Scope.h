#if !defined(ARCADIA_LANGUAGES_SCOPE_H_INCLUDED)
#define ARCADIA_LANGUAGES_SCOPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_Languages_StringTable Arcadia_Languages_StringTable;

// Very naive implementation of a scope.
Arcadia_declareObjectType(u8"Arcadia.Languages.Scope", Arcadia_Languages_Scope,
                          u8"Arcadia.Object");

struct Arcadia_Languages_ScopeDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_Scope {
  Arcadia_Object _parent;
  Arcadia_Map* entries;
  Arcadia_Languages_Scope* enclosing;
};

Arcadia_Languages_Scope*
Arcadia_Languages_Scope_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* enclosing
  );

void
Arcadia_Languages_Scope_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  );

Arcadia_BooleanValue
Arcadia_Languages_Scope_enter
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* key,
    Arcadia_Object* value
  );

Arcadia_Object*
Arcadia_Languages_Scope_lookup
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* name,
    Arcadia_BooleanValue recursive
  );

Arcadia_BooleanValue
Arcadia_Languages_Scope_contains
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* name,
    Arcadia_BooleanValue recursive
  );

Arcadia_SizeValue
Arcadia_Languages_Scope_count
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_BooleanValue recursive
  );

#endif // ARCADIA_LANGUAGES_SCOPE_H_INCLUDED
