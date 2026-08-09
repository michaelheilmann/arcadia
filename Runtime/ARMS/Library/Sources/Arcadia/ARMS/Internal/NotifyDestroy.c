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

#include "Arcadia/ARMS/Internal/NotifyDestroy.h"

#if defined(Arcadia_ARMS_Configuration_WithNotifyDestroy) && 1 == Arcadia_ARMS_Configuration_WithNotifyDestroy

#include "Arcadia/ARMS/Internal/Common.h"

/* singly-linked list node */
typedef struct NotifyDestroyListNode NotifyDestroyListNode;

struct NotifyDestroyListNode {
  NotifyDestroyListNode* next;
  void* argument1;
  void* argument2;
  Arcadia_ARMS_NotifyDestroyCallback* callback;
};

/* hash map node */
typedef struct NotifyDestroyMapNode NotifyDestroyMapNode;

struct NotifyDestroyMapNode {
  NotifyDestroyMapNode* next;
  void* observed;
  NotifyDestroyListNode* list;
};

typedef struct NotifyDestroyMap NotifyDestroyMap;

struct NotifyDestroyMap {
  NotifyDestroyMapNode** buckets;
  Arcadia_ARMS_Size size;
  Arcadia_ARMS_Size capacity;
};

static NotifyDestroyMap* g_notifyDestroyMap = NULL;

// Clear the node list of a map node.
static void
NodeList_clear
  (
    NotifyDestroyMapNode* node
  )
{
  while (node->list) {
    NotifyDestroyListNode* node1 = node->list;
    node->list = node->list->next;
    //node1->callback(node1->argument1, node1->argument2);
    free(node1);
  }
}

// Remove all occurrences of (argument1, argument2, callback) from the node list of a map node.
static void
NodeList_remove
  (
    NotifyDestroyMapNode* node,
    void* argument1,
    void* argument2,
    Arcadia_ARMS_NotifyDestroyCallback* callback
  )
{
  NotifyDestroyListNode** previous1 = &node->list;
  NotifyDestroyListNode* current1 = node->list;
  while (current1) {
    if (current1->argument1 == argument1 && current1->argument2 == argument2 && current1->callback == callback) {
      NotifyDestroyListNode* node1 = current1;
      *previous1 = current1->next;
      current1 = current1->next;
      free(node1);
    } else {
      previous1 = &current1->next;
      current1 = current1->next;
    }
  }
}

static void
NodeList_notify
  (
    NotifyDestroyMapNode* node
  )
{
  while (node->list) {
    NotifyDestroyListNode* node1 = node->list;
    node->list = node->list->next;
    node1->callback(node1->argument1, node1->argument2);
    free(node1);
  }
}

Arcadia_ARMS_Status
Arcadia_ARMS_NotifyDestroyModule_startup
  (
  )
{
  g_notifyDestroyMap = malloc(sizeof(NotifyDestroyMap));
  if (!g_notifyDestroyMap) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  g_notifyDestroyMap->size = 0;
  g_notifyDestroyMap->capacity = 8;
  g_notifyDestroyMap->buckets = malloc(sizeof(NotifyDestroyMapNode) * g_notifyDestroyMap->capacity);
  if (!g_notifyDestroyMap->buckets) {
    free(g_notifyDestroyMap);
    g_notifyDestroyMap = NULL;
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  for (size_t i = 0, n = g_notifyDestroyMap->capacity; i < n; ++i) {
    g_notifyDestroyMap->buckets[i] = NULL;
  }
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_NotifyDestroyModule_shutdown
  (
  )
{
  if (g_notifyDestroyMap) {
    free(g_notifyDestroyMap);
    g_notifyDestroyMap = NULL;
  }
  return Arcadia_ARMS_Status_Success;
}

void
Arcadia_ARMS_NotifyDestroyModule_notifyDestroy
  (
    void* observed
  )
{
  size_t hashValue = (size_t)(uintptr_t)observed;
  size_t hashIndex = hashValue % g_notifyDestroyMap->capacity;
  NotifyDestroyMapNode** previous = &g_notifyDestroyMap->buckets[hashIndex];
  NotifyDestroyMapNode* current = g_notifyDestroyMap->buckets[hashIndex];
  while (current) {
    if (current->observed == observed) {
      NotifyDestroyMapNode* node = current;
      NodeList_notify(node);
      *previous = current->next;
      current = current->next;
      free(node);
      g_notifyDestroyMap->size--;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
}

Arcadia_ARMS_Status
Arcadia_ARMS_addNotifyDestroy
  (
    void* observed,
    void* argument1,
    void* argument2,
    Arcadia_ARMS_NotifyDestroyCallback* callback
  )
{
  size_t hashValue = (size_t)(uintptr_t)observed;
  size_t hashIndex = hashValue % g_notifyDestroyMap->capacity;
  NotifyDestroyMapNode *node = NULL;
  for (node = g_notifyDestroyMap->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->observed == observed) {
      break;
    }
  }
  if (!node) {
    node = malloc(sizeof(NotifyDestroyMapNode));
    if (!node) {
      return Arcadia_ARMS_Status_AllocationFailed;
    }
    node->observed = observed;
    node->list = NULL;
    node->next = g_notifyDestroyMap->buckets[hashIndex];
    g_notifyDestroyMap->buckets[hashIndex] = node;
    g_notifyDestroyMap->size++;
  }
  NotifyDestroyListNode* listNode = malloc(sizeof(NotifyDestroyListNode));
  if (!listNode) {
    return Arcadia_ARMS_Status_AllocationFailed;
  }
  listNode->next = node->list;
  node->list = listNode;
  listNode->callback = callback;
  listNode->argument1 = argument1;
  listNode->argument2 = argument2;
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_removeNotifyDestroyAll
  (
    void* observed
  )
{
  size_t hashValue = (size_t)(uintptr_t)observed;
  size_t hashIndex = hashValue % g_notifyDestroyMap->capacity;
  NotifyDestroyMapNode** previous = &g_notifyDestroyMap->buckets[hashIndex];
  NotifyDestroyMapNode* current = g_notifyDestroyMap->buckets[hashIndex];
  while (current) {
    if (current->observed == observed) {
      NotifyDestroyMapNode* node = current;
      NodeList_clear(node);
      *previous = current->next;
      current = current->next;
      free(node);
      g_notifyDestroyMap->size--;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  return Arcadia_ARMS_Status_Success;
}

Arcadia_ARMS_Status
Arcadia_ARMS_removeNotifyDestroy
  (
    void* observed,
    void* argument1,
    void* argument2,
    Arcadia_ARMS_NotifyDestroyCallback* callback
  )
{
  size_t hashValue = (size_t)(uintptr_t)observed;
  size_t hashIndex = hashValue % g_notifyDestroyMap->capacity;
  NotifyDestroyMapNode** previous = &g_notifyDestroyMap->buckets[hashIndex];
  NotifyDestroyMapNode* current = g_notifyDestroyMap->buckets[hashIndex];
  while (current) {
    if (current->observed == observed) {
      NotifyDestroyMapNode* node = current;
      NodeList_remove(node, argument1, argument2, callback);
      if (node->list) {
        previous = &current->next;
        current = current->next;
      } else {
        *previous = current->next;
        current = current->next;
        free(node);
        g_notifyDestroyMap->size--;
      }
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  return Arcadia_ARMS_Status_Success;
}

#endif // Arcadia_ARMS_Configuration_WithNotifyDestroy
