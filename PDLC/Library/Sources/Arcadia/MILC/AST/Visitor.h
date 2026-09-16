#if !defined(ARCADIA_MILC_AST_VISITOR_H_INCLUDED)
#define ARCADIA_MILC_AST_VISITOR_H_INCLUDED

#include "Arcadia//Ring2/Include.h"
typedef struct Arcadia_MILC_AST_ModuleNode Arcadia_MILC_AST_ModuleNode;
typedef struct Arcadia_MILC_AST_ModuleDefinitionNode Arcadia_MILC_AST_ModuleDefinitionNode;
typedef struct Arcadia_MILC_AST_ProcedureDefinitionNode Arcadia_MILC_AST_ProcedureDefinitionNode;
typedef struct Arcadia_MILC_AST_ClassDefinitionNode Arcadia_MILC_AST_ClassDefinitionNode;
typedef struct Arcadia_MILC_AST_EnumerationDefinitionNode Arcadia_MILC_AST_EnumerationDefinitionNode;

Arcadia_declareObjectType(u8"Arcadia.MILC.AST.Visitor", Arcadia_MILC_AST_Visitor,
                          u8"Arcadia.Object");
       
struct Arcadia_MILC_AST_VisitorDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*visitModuleNode)(Arcadia_Thread*, Arcadia_MILC_AST_Visitor*, Arcadia_MILC_AST_ModuleNode*);
  void (*visitModuleDefinitionNode)(Arcadia_Thread*, Arcadia_MILC_AST_Visitor*, Arcadia_MILC_AST_ModuleDefinitionNode*);
  void (*visitProcedureDefinitionNode)(Arcadia_Thread*, Arcadia_MILC_AST_Visitor*, Arcadia_MILC_AST_ProcedureDefinitionNode*);
  void (*visitClassDefinitionNode)(Arcadia_Thread*, Arcadia_MILC_AST_Visitor*, Arcadia_MILC_AST_ClassDefinitionNode*);
  void (*visitEnumerationDefinitionNode)(Arcadia_Thread*, Arcadia_MILC_AST_Visitor*, Arcadia_MILC_AST_EnumerationDefinitionNode*);
};

struct Arcadia_MILC_AST_Visitor {
  Arcadia_Object _parent;
};

void
Arcadia_MILC_AST_Visitor_visitModuleNode
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_Visitor* self,
    Arcadia_MILC_AST_ModuleNode* node
  );

void
Arcadia_MILC_AST_Visitor_visitModuleDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_Visitor* self,
    Arcadia_MILC_AST_ModuleDefinitionNode* node
  );

void
Arcadia_MILC_AST_Visitor_visitProcedureDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_Visitor* self,
    Arcadia_MILC_AST_ProcedureDefinitionNode* node
  );

void
Arcadia_MILC_AST_Visitor_visitClassDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_Visitor* self,
    Arcadia_MILC_AST_ClassDefinitionNode* node
  );

void
Arcadia_MILC_AST_Visitor_visitEnumerationDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_Visitor* self,
    Arcadia_MILC_AST_EnumerationDefinitionNode* node
  );

#endif // ARCADIA_MILC_AST_VISITOR_H_INCLUDED
