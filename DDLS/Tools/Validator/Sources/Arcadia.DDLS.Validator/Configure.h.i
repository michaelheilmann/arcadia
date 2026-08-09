// The author of this software is Michael Heilmann (contact@michaelheilmann.com).
//
// Copyright(c) 2024-2025 Michael Heilmann (contact@michaelheilmann.com).
//
// Permission to use, copy, modify, and distribute this software for any
// purpose without fee is hereby granted, provided that this entire notice
// is included in all copies of any software which is or includes a copy
// or modification of this software and in all copies of the supporting
// documentation for such software.
//
// THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY.IN PARTICULAR, NEITHER THE AUTHOR NOR LUCENT MAKES ANY
// REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
// OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.

<<<<<<<< HEAD:DDLS/Library/Sources/Arcadia/DDLS/Nodes/ChoiceNode.h
#if !defined(ARCADIA_DDLS_NODES_CHOICENODE_H_INCLUDED)
#define ARCADIA_DDLS_NODES_CHOICENODE_H_INCLUDED

#if !defined(ARCADIA_DDLS_NODES_PRIVATE) || 1 != ARCADIA_DDLS_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDLS/Nodes/Include.h` instead")
#endif
#include "Arcadia/DDLS/Nodes/Node.h"

/// "choice" node.
Arcadia_declareObjectType(u8"Arcadia.DDLS.ChoiceNode", Arcadia_DDLS_ChoiceNode,
                          u8"Arcadia.DDLS.Node");

struct Arcadia_DDLS_ChoiceNode {
  Arcadia_DDLS_Node parent;
  Arcadia_List* choices;
};

Arcadia_DDLS_ChoiceNode*
Arcadia_DDLS_ChoiceNode_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_NODES_CHOICENODE_H_INCLUDED
========
#if !defined(TOOLS_TEMPLATEENGINE_CONFIGURE_H_INCLUDED)
#define TOOLS_TEMPLATEENGINE_CONFIGURE_H_INCLUDED

#endif // TOOLS_TEMPLATEENGINE_CONFIGURE_H_INCLUDED
>>>>>>>> 6242f84d (Update Week 1 January 2026):DDLS/Tools/Validator/Sources/Arcadia.DDLS.Validator/Configure.h.i
