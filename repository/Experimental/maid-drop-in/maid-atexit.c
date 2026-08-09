/**
 * @file maid-atexit.c
 * @brief Source file of the atexit functionality.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#include "maid-atexit.h"

#include "maid-global-mutex.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _maid_node _maid_node;

struct _maid_node {
  _maid_node* next;
  void* context;
  void (*callback)(void* context);
};

static bool g_registered = false;
static bool g_running = false;
static _maid_node* g_nodes = NULL;

static void
_maid_atexit_callback
  (
    void
  )
{
  if (maid_global_mutex_lock()) {
    // This should not happen. Undefined behavior of the environment.
  }
  if (g_running) {
    maid_global_mutex_unlock();
    // This should not happen. Undefined behavior of the environment.
  }
  g_running = true;
  while (g_nodes) {
    _maid_node* node = g_nodes;
    g_nodes = g_nodes->next;
    node->callback(node->context);
    free(node);
  }
  maid_global_mutex_unlock();
}

int maid_atexit_register(maid_atexit_handler_id* id, void* context, void (*callback)(void* context)) {
  if (maid_global_mutex_lock()) {
    return 1;
  }
  if (g_running) {
    maid_global_mutex_unlock();
    return 1;
  }
  if (!g_registered) {
    if (atexit(_maid_atexit_callback)) {
      maid_global_mutex_unlock();
      return 1;
    }
    g_registered = true;
  }
  _maid_node* node = malloc(sizeof(_maid_node));
  if (!node) {
    maid_global_mutex_unlock();
    return 1;
  }
  node->context = context;
  node->callback = callback;
  *id = (uintptr_t)id;
  return 0;
}

int maid_atexit_unregister(maid_atexit_handler_id id) {
  if (maid_global_mutex_lock()) {
    return 1;
  }
  if (g_running) {
    maid_global_mutex_unlock();
    return 1;
  }
  _maid_node** previous = &g_nodes;
  _maid_node* current = g_nodes;
  while (current) {
    if (current == (void*)id) {
      _maid_node* node = current;
      *previous = current->next;
      current = current->next;
      free(node);
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  maid_global_mutex_unlock();
  return 0;
}
