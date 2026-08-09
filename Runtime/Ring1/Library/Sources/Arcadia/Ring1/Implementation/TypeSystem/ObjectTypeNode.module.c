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
#include "Arcadia/Ring1/Implementation/TypeSystem/ObjectTypeNode.module.h"

#include "Arcadia/Ring1/Implementation/Diagnostics.h"
#include "Arcadia/Ring1/Implementation/Memory.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"

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
    ObjectTypeNode* typeNode
  );

static void
visitCallback
  (
    Arcadia_Process* context,
    ObjectTypeNode* typeNode
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
    ObjectTypeNode* typeNode
  )
{
  if (typeNode->parentObjectType) {
    if (Arcadia_ARMS_unlock(typeNode->parentObjectType)) {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: <error>\n", __FILE__, __LINE__);
    }
  }
  if (typeNode->dispatch) {
    free(typeNode->dispatch);
    typeNode->dispatch = NULL;
  }
}

static void
visitCallback
  (
    Arcadia_Process* process,
    ObjectTypeNode* typeNode
  )
{
  if (typeNode->parentObjectType) {
    Arcadia_Type_visit(Arcadia_Process_getThread(process), typeNode->parentObjectType);
  }
  Arcadia_Name_visit(Arcadia_Process_getThread(process), ((TypeNode*)typeNode)->name);
}

ObjectTypeNode*
ObjectTypeNode_allocate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_registered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 ObjectTypeNodeName, sizeof(ObjectTypeNodeName) - 1,
                                 Arcadia_Thread_getProcess(thread),
                                 (Arcadia_Process_TypeRemovedCallback*)&typeRemovedCallback,
                                 (Arcadia_Process_VisitCallback*)&visitCallback,
                                 (Arcadia_Process_FinalizeCallback*)&finalizeCallback);
    g_registered = Arcadia_BooleanValue_True;
  }
  ObjectTypeNode* node = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&node, ObjectTypeNodeName, sizeof(ObjectTypeNodeName) - 1, sizeof(ObjectTypeNode));
  Arcadia_Memory_fillZero(thread, node, sizeof(ObjectTypeNode));
  return node;
}
