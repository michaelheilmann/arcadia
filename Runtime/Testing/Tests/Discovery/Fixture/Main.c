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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
fixtureGeneratedValue
  (
    void
  );

int
main
  (
    int argc,
    char** argv
  )
{
  if (fixtureGeneratedValue() != 42) return EXIT_FAILURE;
  const char* names[] = { "First", "two words", "punctuation[\";\\]", "Skip" };
  const char* environment = getenv("ARCADIA_DISCOVERY_TEST");
  if (!environment || strcmp(environment, "environment value") || argc < 3
      || strcmp(argv[1], "missing font.ttf")) return EXIT_FAILURE;
  if (argc == 3 && !strcmp(argv[2], "--list")) {
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i) puts(names[i]);
    return EXIT_SUCCESS;
  }
  if (argc == 4 && !strcmp(argv[2], "--case")) {
    FILE* file = fopen("Assets/Input.txt", "rb");
    if (!file) return EXIT_FAILURE;
    char input[16] = { 0 };
    size_t size = fread(input, 1, sizeof(input) - 1, file);
    fclose(file);
    if (size != 9 || strcmp(input, "pristine\n")) return EXIT_FAILURE;
    file = fopen("output.txt", "rb");
    if (file) { fclose(file); return EXIT_FAILURE; }
    file = fopen("output.txt", "wb");
    if (!file) return EXIT_FAILURE;
    fputs(argv[3], file);
    fclose(file);
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
      if (!strcmp(names[i], argv[3])) return i == 3 ? 77 : EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}
