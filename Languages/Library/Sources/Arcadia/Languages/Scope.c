#include "Arcadia/Languages/Scope.h"

static void
Arcadia_Languages_Scope_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  );

static void
Arcadia_Languages_Scope_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_ScopeDispatch* self
  );

static void
Arcadia_Languages_Scope_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_Scope_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_Scope_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_Scope_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.Scope", Arcadia_Languages_Scope,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Languages_Scope_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_Scope);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->entries = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  if (!Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->enclosing = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_Scope_getType(thread));
  } else {
    self->enclosing = NULL;
  }
  Arcadia_LeaveConstructor(Arcadia_Languages_Scope);
}

static void
Arcadia_Languages_Scope_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_ScopeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_Scope_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  )
{
  if (self->entries) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entries);
  }
  if (self->enclosing) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enclosing);
  }
}

Arcadia_Languages_Scope*
Arcadia_Languages_Scope_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* enclosing
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_Scope);
  if (enclosing) Arcadia_ValueStack_pushObjectReferenceValue(thread, enclosing); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Languages_Scope);
}

void
Arcadia_Languages_Scope_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self
  )
{ 
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->entries);
}

Arcadia_BooleanValue
Arcadia_Languages_Scope_enter
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* key,
    Arcadia_Object* value
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(key);
  Arcadia_Value v = Arcadia_Map_get(thread, self->entries, k);
  if (Arcadia_Value_isObjectReferenceValue(&v)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Map_set(thread, self->entries, Arcadia_Value_makeObjectReferenceValue(key), Arcadia_Value_makeObjectReferenceValue(value), NULL, NULL);
  return Arcadia_BooleanValue_True;
}

Arcadia_Object*
Arcadia_Languages_Scope_lookup
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* name,
    Arcadia_BooleanValue recursive
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(name);
  if (recursive) {
    Arcadia_Languages_Scope* scope = self;
    do {
      Arcadia_Value v = Arcadia_Map_get(thread, scope->entries, k);
      if (Arcadia_Value_isObjectReferenceValue(&v)) {
        return Arcadia_Value_getObjectReferenceValue(&v);
      }
      scope = scope->enclosing;
    } while (scope);
    return NULL;
  } else {
    Arcadia_Value v = Arcadia_Map_get(thread, self->entries, k);
    if (Arcadia_Value_isObjectReferenceValue(&v)) {
      return Arcadia_Value_getObjectReferenceValue(&v);
    } else {
      return NULL;
    }
  }
}

Arcadia_BooleanValue
Arcadia_Languages_Scope_contains
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_String* name,
    Arcadia_BooleanValue recursive
  )
{ return NULL != Arcadia_Languages_Scope_lookup(thread, self, name, recursive); }

Arcadia_SizeValue
Arcadia_Languages_Scope_count
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scope* self,
    Arcadia_BooleanValue recursive
  )
{
  Arcadia_SizeValue count = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->entries);
  if (recursive && self->enclosing) {
    count += Arcadia_Languages_Scope_count(thread, self->enclosing, recursive);
  }
  return count;
}
