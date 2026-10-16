#if !defined(ARCADIA_PDLC_AST_VISITOR_H_INCLUDED)
#define ARCADIA_PDLC_AST_VISITOR_H_INCLUDED

#include "Arcadia//Ring2/Include.h"
typedef struct Arcadia_PDLC_AST_ModuleNode Arcadia_PDLC_AST_ModuleNode;
typedef struct Arcadia_PDLC_AST_ModuleDefinitionNode Arcadia_PDLC_AST_ModuleDefinitionNode;
typedef struct Arcadia_PDLC_AST_ProcedureDefinitionNode Arcadia_PDLC_AST_ProcedureDefinitionNode;
typedef struct Arcadia_PDLC_AST_ClassDefinitionNode Arcadia_PDLC_AST_ClassDefinitionNode;
typedef struct Arcadia_PDLC_AST_EnumerationDefinitionNode Arcadia_PDLC_AST_EnumerationDefinitionNode;
typedef struct Arcadia_PDLC_AST_InterfaceDefinitionNode Arcadia_PDLC_AST_InterfaceDefinitionNode;

Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.Visitor", Arcadia_PDLC_AST_Visitor,
                          u8"Arcadia.Object");

struct Arcadia_PDLC_AST_VisitorDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*visitModuleNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ModuleNode*);
  void (*visitModuleDefinitionNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ModuleDefinitionNode*);
  void (*visitProcedureDefinitionNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ProcedureDefinitionNode*);
  void (*visitClassDefinitionNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_ClassDefinitionNode*);
  void (*visitEnumerationDefinitionNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_EnumerationDefinitionNode*);
  void (*visitInterfaceDefinitionNode)(Arcadia_Thread*, Arcadia_PDLC_AST_Visitor*, Arcadia_PDLC_AST_InterfaceDefinitionNode*);
};

struct Arcadia_PDLC_AST_Visitor {
  Arcadia_Object _parent;
};

void
Arcadia_PDLC_AST_Visitor_visitModuleNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_ModuleNode* node
  );

void
Arcadia_PDLC_AST_Visitor_visitModuleDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_ModuleDefinitionNode* node
  );

void
Arcadia_PDLC_AST_Visitor_visitProcedureDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_ProcedureDefinitionNode* node
  );

void
Arcadia_PDLC_AST_Visitor_visitClassDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_ClassDefinitionNode* node
  );

void
Arcadia_PDLC_AST_Visitor_visitEnumerationDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_EnumerationDefinitionNode* node
  );

void
Arcadia_PDLC_AST_Visitor_visitInterfaceDefinitionNode
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_Visitor* self,
    Arcadia_PDLC_AST_InterfaceDefinitionNode* node
  );

#endif // ARCADIA_PDLC_AST_VISITOR_H_INCLUDED
