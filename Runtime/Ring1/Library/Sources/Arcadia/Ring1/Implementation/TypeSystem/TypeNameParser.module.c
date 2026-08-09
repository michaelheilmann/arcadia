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
#include "Arcadia/Ring1/Implementation/TypeSystem/TypeNameParser.module.h"

#include "Arcadia/Ring1/Include.h"

static bool
isAlphabetic
  (
    char const* current,
    char const* start,
    char const* end
  )
{
  if (current == end) {
    return false;
  }
  return ('a' <= *current && *current <= 'z')
      || ('A' <= *current && *current <= 'Z');
}

static bool
isAlphanumeric
  (
    char const* current,
    char const* start,
    char const* end
  )
{
  if (current == end) {
    return false;
  }
  return '0' <= *current && *current <= '9';
}

static bool
isUnderscore
  (
    char const* current,
    char const* start,
    char const* end
  )
{
  if (current == end) {
    return false;
  }
  return '_' == *current;
}

static bool
isPeriod
  (
    char const* current,
    char const* start,
    char const* end
  )
{
  if (current == end) {
    return false;
  }
  return '.' == *current;
}

static bool
isEnd
  (
    char const* current,
    char const* start,
    char const* end
  )
{ return current == end; }

static void
parseName
  (
    Arcadia_Thread* thread,
    char const** current,
    char const* start,
    char const* end
  )
{
  if (!isAlphabetic(*current, start, end) && !isUnderscore(*current, start, end)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  (*current)++;
  while (isAlphabetic(*current, start, end) || isUnderscore(*current, start, end) || isAlphanumeric(*current, start, end)) {
    (*current)++;
  }
}

static void
parseQualifiedName
  (
    Arcadia_Thread* thread,
    char const** current,
    char const* start,
    char const* end
  )
{
  parseName(thread, current, start, end);
  while (isPeriod(*current, start, end)) {
    (*current)++;
    parseName(thread, current, start, end);
  }
}

void
Arcadia_Names_parseTypeName
  (
    Arcadia_Thread* thread,
    const char *bytes,
    size_t numberOfBytes
  )
{
  char const* current = bytes,
            * start = bytes,
            * end = ((uint8_t const*)bytes) + numberOfBytes;
  parseQualifiedName(thread, &current, start, end);
  if (!isEnd(current, start, end)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}
