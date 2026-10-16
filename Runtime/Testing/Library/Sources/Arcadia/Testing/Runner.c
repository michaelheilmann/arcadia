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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Ring1/Include.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int
usage
  (
    const char* program
  )
{
  Arcadia_logf(Arcadia_LogFlags_Error, "Usage: %s [--case NAME | --list]\n", program);
  return EXIT_FAILURE;
}

static int
validateCases
  (
    const Arcadia_Testing_Case* cases,
    size_t count
  )
{
  if (!cases || !count) {
    Arcadia_logf(Arcadia_LogFlags_Error, "Invalid suite: a nonempty case table is required\n");
    return EXIT_FAILURE;
  }
  for (size_t i = 0; i < count; ++i) {
    if (!cases[i].name || !cases[i].name[0] || strpbrk(cases[i].name, "\r\n") || !cases[i].run) {
      Arcadia_logf(Arcadia_LogFlags_Error, "Invalid suite: invalid case descriptor at index %zu\n", i);
      return EXIT_FAILURE;
    }
    for (size_t j = 0; j < i; ++j) {
      if (!strcmp(cases[i].name, cases[j].name)) {
        Arcadia_logf(Arcadia_LogFlags_Error, "Invalid suite: duplicate case name: %s\n", cases[i].name);
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int
Arcadia_Testing_runSuite
  (
    int argc,
    char** argv,
    const Arcadia_Testing_Case* cases,
    size_t count
  )
{
  const char* program = argc > 0 && argv && argv[0] ? argv[0] : "test-suite";
  if (!argv || (argc != 1 && argc != 2 && argc != 3)) return usage(program);
  for (int i = 0; i < argc; ++i) {
    if (!argv[i]) return usage(program);
  }
  const char* selected = NULL;
  int listing = argc == 2 && !strcmp(argv[1], "--list");
  if (argc == 3 && !strcmp(argv[1], "--case") && argv[2][0]) {
    selected = argv[2];
  } else if (argc != 1 && !listing) {
    return usage(program);
  }
  if (validateCases(cases, count) != EXIT_SUCCESS) return EXIT_FAILURE;
  if (listing) {
    for (size_t i = 0; i < count; ++i) {
      if (fputs(cases[i].name, stdout) == EOF || fputc('\n', stdout) == EOF) {
        Arcadia_logf(Arcadia_LogFlags_Error, "Unable to list cases\n");
        return EXIT_FAILURE;
      }
    }
    if (fflush(stdout) == EOF) {
      Arcadia_logf(Arcadia_LogFlags_Error, "Unable to list cases\n");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  int result = EXIT_SUCCESS;
  size_t executed = 0;
  size_t skipped = 0;
  for (size_t i = 0; i < count; ++i) {
    if (selected && strcmp(selected, cases[i].name)) continue;
    Arcadia_logf(Arcadia_LogFlags_Info, "[ RUN      ] %s\n", cases[i].name);
    const char* previousCaseName = Arcadia_Tests_setCaseName(cases[i].name);
    int status = cases[i].run();
    Arcadia_Tests_setCaseName(previousCaseName);
    executed++;
    if (status == Arcadia_Testing_Skip) {
      Arcadia_logf(Arcadia_LogFlags_Info, "[  SKIPPED ] %s\n", cases[i].name);
      skipped++;
    } else if (status != EXIT_SUCCESS) {
      Arcadia_logf(Arcadia_LogFlags_Error, "[  FAILED  ] %s\n", cases[i].name);
      result = EXIT_FAILURE;
    } else {
      Arcadia_logf(Arcadia_LogFlags_Info, "[       OK ] %s\n", cases[i].name);
    }
  }
  if (!executed) {
    Arcadia_logf(Arcadia_LogFlags_Error, "Unknown case: %s\nAvailable cases:\n", selected ? selected : "(none)");
    for (size_t i = 0; i < count; ++i) Arcadia_logf(Arcadia_LogFlags_Error, "  %s\n", cases[i].name);
    return EXIT_FAILURE;
  }
  return result == EXIT_SUCCESS && skipped == executed ? Arcadia_Testing_Skip : result;
}
