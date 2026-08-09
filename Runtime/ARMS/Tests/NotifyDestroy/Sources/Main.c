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

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// strlen
#include <string.h>
// true, false, bool
#include <stdbool.h>

#include "Arcadia/ARMS/Include.h"

typedef struct Object {
  char* name;
} Object;

static bool g_notifyDestroyCalled = false;
static bool g_visitCalled = false;
static bool g_finalizeCalled = false;

static void
Object_notifyDestroy
  (
    void* context,
    Object* object
  )
{
  g_notifyDestroyCalled = true;
}

static void
Object_visit
  (
    void* context,
    Object* object
  )
{
  g_visitCalled = true;
}

static void
Object_finalize
  (
    void* context,
    Object* object
  )
{
  g_finalizeCalled = true;
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (Arcadia_ARMS_startup()) {
    return EXIT_FAILURE;
  }
  if (Arcadia_ARMS_addType("Object", strlen("Object"), NULL, NULL, (Arcadia_ARMS_VisitCallbackFunction*)&Object_visit, (Arcadia_ARMS_FinalizeCallbackFunction*)&Object_finalize)) {
    Arcadia_ARMS_RunStatistics statistics = Arcadia_ARMS_RunStatistics_StaticInitializer();
    Arcadia_ARMS_run(&statistics);
    Arcadia_ARMS_shutdown();
    return EXIT_FAILURE;
  }
  Object* sender = NULL;
  if (Arcadia_ARMS_allocate((void**)&sender, "Object", strlen("Object"), sizeof(Object))) {
    Arcadia_ARMS_RunStatistics statistics = Arcadia_ARMS_RunStatistics_StaticInitializer();
    Arcadia_ARMS_run(&statistics);
    Arcadia_ARMS_shutdown();
    return EXIT_FAILURE;
  }
  if (Arcadia_ARMS_addNotifyDestroy(sender, NULL, NULL, (Arcadia_ARMS_NotifyDestroyCallback*)&Object_notifyDestroy)) {
    Arcadia_ARMS_RunStatistics statistics = Arcadia_ARMS_RunStatistics_StaticInitializer();
    Arcadia_ARMS_run(&statistics);
    Arcadia_ARMS_shutdown();
    return EXIT_FAILURE;
  }
  Arcadia_ARMS_RunStatistics statistics = Arcadia_ARMS_RunStatistics_StaticInitializer();
  Arcadia_ARMS_run(&statistics);
  Arcadia_ARMS_shutdown();
  if (g_visitCalled || !g_finalizeCalled || !g_notifyDestroyCalled) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
