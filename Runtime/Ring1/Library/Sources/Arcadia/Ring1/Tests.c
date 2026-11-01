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
#include "Arcadia/Ring1/Tests.h"
#include <inttypes.h>
#include <string.h>

#if defined(_MSC_VER)
static __declspec(thread) const char* currentCaseName;
#else
static _Thread_local const char* currentCaseName;
#endif

const char*
Arcadia_Tests_getCaseName
  (
    void
  )
{
  return currentCaseName;
}

const char*
Arcadia_Tests_setCaseName
  (
    const char* name
  )
{
  const char* previous = currentCaseName;
  currentCaseName = name;
  return previous;
}

static void
reportHeader
  (
    const char* kind,
    const char* file,
    int line,
    const char* expectedExpression,
    const char* actualExpression
  )
{
  Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: case `%s`: %s equality (%s vs %s): ",
    file, line, currentCaseName ? currentCaseName : "<unselected>", kind, expectedExpression, actualExpression);
}

static void
fail
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  Arcadia_Thread_jump(thread);
}

#define DefineEquality(Name, Type, Format, Cast) \
  void \
  Arcadia_Tests_assertEqual##Name##Impl \
    ( \
      Arcadia_Thread* thread, \
      Type expected, \
      Type actual, \
      const char* file, \
      int line, \
      const char* expectedExpression, \
      const char* actualExpression \
    ) \
  { \
    if (expected == actual) return; \
    reportHeader(#Name, file, line, expectedExpression, actualExpression); \
    Arcadia_logf(Arcadia_LogFlags_Error, "expected=" Format " actual=" Format "\n", (Cast)expected, (Cast)actual); \
    fail(thread); \
  }

DefineEquality(Integer64, Arcadia_Integer64Value, "%" PRId64, int64_t)
DefineEquality(Natural64, Arcadia_Natural64Value, "%" PRIu64, uint64_t)
DefineEquality(Real64, Arcadia_Real64Value, "%.17g", double)
DefineEquality(Size, Arcadia_SizeValue, "%zu", size_t)
DefineEquality(Status, Arcadia_Status, "%" PRIu32, uint32_t)
#undef DefineEquality

static void
printBytes
  (
    const unsigned char* bytes,
    size_t size
  )
{
  if (!bytes) {
    Arcadia_logf(Arcadia_LogFlags_Error, "<null> (length=%zu)", size);
    return;
  }
  Arcadia_logf(Arcadia_LogFlags_Error, "\"");
  for (size_t i = 0; i < size && i < 32; ++i) {
    unsigned char c = bytes[i];
    if (c == '\n') Arcadia_logf(Arcadia_LogFlags_Error, "\\n");
    else if (c == '\r') Arcadia_logf(Arcadia_LogFlags_Error, "\\r");
    else if (c == '\t') Arcadia_logf(Arcadia_LogFlags_Error, "\\t");
    else if (c == '\\' || c == '"') Arcadia_logf(Arcadia_LogFlags_Error, "\\%c", (int)c);
    else if (c >= 32 && c < 127) Arcadia_logf(Arcadia_LogFlags_Error, "%c", (int)c);
    else Arcadia_logf(Arcadia_LogFlags_Error, "\\x%02x", (unsigned)c);
  }
  Arcadia_logf(Arcadia_LogFlags_Error, "\"%s (length=%zu)", size > 32 ? "..." : "", size);
}

static void
reportDifference
  (
    const void* expected,
    size_t expectedSize,
    const void* actual,
    size_t actualSize
  )
{
  Arcadia_logf(Arcadia_LogFlags_Error, "expected=");
  printBytes(expected, expectedSize);
  Arcadia_logf(Arcadia_LogFlags_Error, " actual=");
  printBytes(actual, actualSize);
  if (expected && actual) {
    const unsigned char* e = expected;
    const unsigned char* a = actual;
    size_t i = 0;
    while (i < expectedSize && i < actualSize && e[i] == a[i]) ++i;
    Arcadia_logf(Arcadia_LogFlags_Error, " first-difference=%zu expected-byte=", i);
    if (i < expectedSize) Arcadia_logf(Arcadia_LogFlags_Error, "0x%02x", (unsigned)e[i]);
    else Arcadia_logf(Arcadia_LogFlags_Error, "<end>");
    Arcadia_logf(Arcadia_LogFlags_Error, " actual-byte=");
    if (i < actualSize) Arcadia_logf(Arcadia_LogFlags_Error, "0x%02x", (unsigned)a[i]);
    else Arcadia_logf(Arcadia_LogFlags_Error, "<end>");
  }
  Arcadia_logf(Arcadia_LogFlags_Error, "\n");
}

void
Arcadia_Tests_assertEqualCxxStringImpl
  (
    Arcadia_Thread* thread,
    const char* expected,
    const char* actual,
    const char* file,
    int line,
    const char* expectedExpression,
    const char* actualExpression
  )
{
  if ((!expected && !actual) || (expected && actual && !strcmp(expected, actual))) return;
  reportHeader("CxxString", file, line, expectedExpression, actualExpression);
  reportDifference(expected, expected ? strlen(expected) : 0, actual, actual ? strlen(actual) : 0);
  fail(thread);
}

void
Arcadia_Tests_assertEqualBytesImpl
  (
    Arcadia_Thread* thread,
    const void* expected,
    Arcadia_SizeValue expectedSize,
    const void* actual,
    Arcadia_SizeValue actualSize,
    const char* file,
    int line,
    const char* expectedExpression,
    const char* actualExpression
  )
{
  if (expectedSize == actualSize && (!expectedSize || (expected && actual && !memcmp(expected, actual, expectedSize)))) return;
  reportHeader("Bytes", file, line, expectedExpression, actualExpression);
  reportDifference(expected, expectedSize, actual, actualSize);
  fail(thread);
}
