// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/TypeSystem/InterfaceTypeNode.module.h"

#include "Arcadia/Ring1/Diagnostics.h"
#include "Arcadia/Ring1/Memory.h"
#include "Arcadia/Ring1/ThreadExtensions.h"

static bool g_registered = false;

static void
typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  );

static void
finalizeCallback
  (
    Arcadia_Process* context,
    InterfaceTypeNode* typeNode
  );

static void
visitCallback
  (
    Arcadia_Process* context,
    InterfaceTypeNode* typeNode
  );

static void
typeRemovedCallback
  (
    Arcadia_Process* context,
    const uint8_t* name,
    size_t nameLength
  )
{ g_registered = false; }

static void
finalizeCallback
  (
    Arcadia_Process* process,
    InterfaceTypeNode* typeNode
  )
{
  for (Arcadia_SizeValue i = 0, n = typeNode->extends.size; i < n; ++i) {
    if (Arcadia_ARMS_unlock(typeNode->extends.elements[i])) {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
    }
  }
  if (typeNode->extends.elements) {
    free(typeNode->extends.elements);
    typeNode->extends.elements = NULL;
  }
  typeNode->extends.size = 0;
  ((TypeNode*)typeNode)->name = NULL;
}

static void
visitCallback
  (
    Arcadia_Process* process,
    InterfaceTypeNode* typeNode
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  for (Arcadia_SizeValue i = 0, n = typeNode->extends.size; i < n; ++i) {
    Arcadia_Type_visit(thread, typeNode->extends.elements[i]);
  }
  Arcadia_Name_visit(thread, ((TypeNode*)typeNode)->name);
}

InterfaceTypeNode*
InterfaceTypeNode_allocate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_registered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 InterfaceTypeNodeName, sizeof(InterfaceTypeNodeName) - 1,
                                 Arcadia_Thread_getProcess(thread),
                                 (Arcadia_Process_TypeRemovedCallback*)&typeRemovedCallback,
                                 (Arcadia_Process_VisitCallback*)&visitCallback,
                                 (Arcadia_Process_FinalizeCallback*)&finalizeCallback);
    g_registered = Arcadia_BooleanValue_True;
  }
  InterfaceTypeNode* node = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&node, InterfaceTypeNodeName, sizeof(InterfaceTypeNodeName) - 1, sizeof(InterfaceTypeNode));
  Arcadia_Memory_fillZero(thread, node, sizeof(InterfaceTypeNode));
  return node;
}
