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

#if !defined(_WIN32)
  #define _POSIX_C_SOURCE 200809L
#endif

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Ring1/Include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#if defined(_WIN32)
  #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif
  #if !defined(NOMINMAX)
    #define NOMINMAX
  #endif
  #include <windows.h>
  #include <io.h>
  #include <fcntl.h>
  #define test_dup _dup
  #define test_dup2 _dup2
  #define test_close _close
  #define test_fileno _fileno
#else
  #include <unistd.h>
  #define test_dup dup
  #define test_dup2 dup2
  #define test_close close
  #define test_fileno fileno
#endif

#define CHECK(Expression) do { \
  if (!(Expression)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #Expression); return EXIT_FAILURE; } \
} while (0)

static size_t callCount;
static int callOrder[16];
static int secondStatus;

static int
first
  (
    void
  )
{
  if (callCount < 16) callOrder[callCount] = 1;
  ++callCount;
  return EXIT_SUCCESS;
}

static int
second
  (
    void
  )
{
  if (callCount < 16) callOrder[callCount] = 2;
  ++callCount;
  return secondStatus;
}

static int
third
  (
    void
  )
{
  if (callCount < 16) callOrder[callCount] = 3;
  ++callCount;
  return EXIT_SUCCESS;
}

static const Arcadia_Testing_Case sampleCases[] = {
  { "First", first }, { "second", second }, { "Third", third },
};

struct Observation {
  int status;
  size_t count;
  int order[16];
  char output[4096];
  char error[4096];
};

static FILE*
captureFile
  (
    void
  )
{
#if defined(_WIN32)
  // CRT tmpfile may use the drive root. Keep captures in the user's temp folder.
  wchar_t directory[MAX_PATH], path[MAX_PATH];
  DWORD size = GetTempPathW(MAX_PATH, directory);
  if (!size || size >= MAX_PATH || !GetTempFileNameW(directory, L"act", 0, path)) return NULL;
  HANDLE file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
    FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
  if (file == INVALID_HANDLE_VALUE) { DeleteFileW(path); return NULL; }
  int descriptor = _open_osfhandle((intptr_t)file, _O_RDWR | _O_BINARY);
  if (descriptor < 0) { CloseHandle(file); return NULL; }
  FILE* stream = _fdopen(descriptor, "w+b");
  if (!stream) _close(descriptor);
  return stream;
#else
  return tmpfile();
#endif
}

static int
readCapture
  (
    FILE* file,
    char* buffer,
    size_t capacity
  )
{
  if (fseek(file, 0, SEEK_SET)) return EXIT_FAILURE;
  size_t size = fread(buffer, 1, capacity - 1, file);
  buffer[size] = '\0';
  if (fgetc(file) != EOF || ferror(file)) return EXIT_FAILURE;
  // Compare logical text lines on both Windows (CRLF) and POSIX (LF).
  size_t out = 0;
  for (size_t i = 0; i < size; ++i) {
    if (buffer[i] == '\r' && i + 1 < size && buffer[i + 1] == '\n') continue;
    buffer[out++] = buffer[i];
  }
  buffer[out] = '\0';
  return EXIT_SUCCESS;
}

static int
invoke
  (
    int argc,
    char** argv,
    const Arcadia_Testing_Case* cases,
    size_t count,
    struct Observation* observation
  )
{
  int result = EXIT_FAILURE;
  int savedOutput = -1, savedError = -1;
  int outputDescriptor = test_fileno(stdout), errorDescriptor = test_fileno(stderr);
  FILE* output = NULL;
  FILE* error = NULL;
  memset(observation, 0, sizeof(*observation));
  callCount = 0;
  memset(callOrder, 0, sizeof(callOrder));
  if (fflush(stdout) || fflush(stderr)) goto cleanup;
  output = captureFile();
  error = captureFile();
  if (!output || !error) goto cleanup;
  savedOutput = test_dup(outputDescriptor);
  savedError = test_dup(errorDescriptor);
  if (savedOutput < 0 || savedError < 0) goto cleanup;
  if (test_dup2(test_fileno(output), outputDescriptor) < 0 || test_dup2(test_fileno(error), errorDescriptor) < 0) goto cleanup;
  observation->status = Arcadia_Testing_runSuite(argc, argv, cases, count);
  observation->count = callCount;
  memcpy(observation->order, callOrder, sizeof(callOrder));
  if (fflush(stdout) || fflush(stderr)) goto cleanup;
  if (test_dup2(savedOutput, outputDescriptor) < 0 || test_dup2(savedError, errorDescriptor) < 0) goto cleanup;
  if (readCapture(output, observation->output, sizeof(observation->output)) != EXIT_SUCCESS
    || readCapture(error, observation->error, sizeof(observation->error)) != EXIT_SUCCESS) goto cleanup;
  result = EXIT_SUCCESS;
cleanup:
  if (savedOutput >= 0) { test_dup2(savedOutput, outputDescriptor); test_close(savedOutput); }
  if (savedError >= 0) { test_dup2(savedError, errorDescriptor); test_close(savedError); }
  clearerr(stdout);
  clearerr(stderr);
  if (output) fclose(output);
  if (error) fclose(error);
  if (result != EXIT_SUCCESS) fprintf(stderr, "Unable to capture runner output\n");
  return result;
}

static int
execution
  (
    void
  )
{
  struct Observation observed;
  char* argv[] = { "runner" };
  secondStatus = EXIT_SUCCESS;
  CHECK(invoke(1, argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 3);
  CHECK(observed.order[0] == 1 && observed.order[1] == 2 && observed.order[2] == 3);
  CHECK(!strcmp(observed.output, "[ RUN      ] First\n[       OK ] First\n[ RUN      ] second\n[       OK ] second\n[ RUN      ] Third\n[       OK ] Third\n"));
  CHECK(!observed.error[0]);
  // Sharing a callback is valid; only duplicate names are ambiguous.
  const Arcadia_Testing_Case aliases[] = { { "AliasA", first }, { "AliasB", first } };
  CHECK(invoke(1, argv, aliases, 2, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 2 && observed.order[0] == 1 && observed.order[1] == 1);
  return EXIT_SUCCESS;
}

static int
selection
  (
    void
  )
{
  struct Observation observed;
  char* argv[] = { "runner", "--case", "second" };
  secondStatus = EXIT_SUCCESS;
  CHECK(invoke(3, argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 1 && observed.order[0] == 2);
  CHECK(!strcmp(observed.output, "[ RUN      ] second\n[       OK ] second\n") && !observed.error[0]);
  argv[2] = "Second";
  CHECK(invoke(3, argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0]);
  CHECK(strstr(observed.error, "Unknown case: Second") && strstr(observed.error, "Available cases:"));
  argv[2] = "Missing";
  CHECK(invoke(3, argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0]);
  const Arcadia_Testing_Case distinct[] = { { "First", first }, { "first", third }, { "two words", second } };
  argv[2] = "first";
  CHECK(invoke(3, argv, distinct, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 1 && observed.order[0] == 3);
  argv[2] = "two words";
  CHECK(invoke(3, argv, distinct, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 1 && observed.order[0] == 2);
  return EXIT_SUCCESS;
}

static int
arguments
  (
    void
  )
{
  struct Observation observed;
  struct Input { int argc; char* argv[6]; } inputs[] = {
    { 0, { NULL } }, { -1, { NULL } }, { 1, { NULL } },
    { 2, { "runner", NULL } }, { 3, { "runner", "--case", NULL } },
    { 2, { "runner", "--case" } }, { 2, { "runner", "--help" } },
    { 2, { "runner", "First" } }, { 3, { "runner", "--case", "" } },
    { 3, { "runner", "--list", "First" } },
    { 4, { "runner", "--case", "First", "--list" } },
    { 5, { "runner", "--case", "First", "--case", "Third" } },
  };
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    CHECK(invoke(inputs[i].argc, inputs[i].argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0]);
    CHECK(strstr(observed.error, "Usage:"));
  }
  CHECK(invoke(1, NULL, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0] && strstr(observed.error, "Usage:"));
  return EXIT_SUCCESS;
}

static int
descriptors
  (
    void
  )
{
  struct Observation observed;
  const Arcadia_Testing_Case invalid[][2] = {
    { { "Good", first }, { NULL, second } },
    { { "Good", first }, { "", second } },
    { { "Good", first }, { "Bad", NULL } },
    { { "Good", first }, { "Good", second } },
    { { "Good", first }, { "Bad\nName", second } },
    { { "Good", first }, { "Bad\rName", second } },
  };
  char* modes[][3] = { { "runner", NULL, NULL }, { "runner", "--case", "Good" }, { "runner", "--list", NULL } };
  const int argcs[] = { 1, 3, 2 };
  for (size_t mode = 0; mode < 3; ++mode) {
    for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
      CHECK(invoke(argcs[mode], modes[mode], invalid[i], 2, &observed) == EXIT_SUCCESS);
      CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0]);
      CHECK(strstr(observed.error, "Invalid suite:"));
    }
    CHECK(invoke(argcs[mode], modes[mode], NULL, 1, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0] && strstr(observed.error, "Invalid suite:"));
    CHECK(invoke(argcs[mode], modes[mode], sampleCases, 0, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0] && strstr(observed.error, "Invalid suite:"));
    CHECK(invoke(argcs[mode], modes[mode], NULL, 0, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && !observed.count && !observed.output[0] && strstr(observed.error, "Invalid suite:"));
  }
  return EXIT_SUCCESS;
}

static int
discovery
  (
    void
  )
{
  struct Observation observed;
  char* argv[] = { "runner", "--list" };
  secondStatus = 42;
  CHECK(invoke(2, argv, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && !observed.count && !observed.error[0]);
  CHECK(!strcmp(observed.output, "First\nsecond\nThird\n"));
  secondStatus = EXIT_SUCCESS;
  for (size_t i = 0; i < 3; ++i) {
    char* selected[] = { "runner", "--case", (char*)sampleCases[i].name };
    CHECK(invoke(3, selected, sampleCases, 3, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_SUCCESS && observed.count == 1 && observed.order[0] == (int)i + 1);
  }
  const Arcadia_Testing_Case single[] = { { "two words", third } };
  CHECK(invoke(2, argv, single, 1, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && !observed.count && !strcmp(observed.output, "two words\n") && !observed.error[0]);
  return EXIT_SUCCESS;
}

static int
failures
  (
    void
  )
{
  struct Observation observed;
  char* all[] = { "runner" };
  char* selected[] = { "runner", "--case", "second" };
  const int statuses[] = { EXIT_FAILURE, 42, -7 };
  for (size_t i = 0; i < sizeof(statuses) / sizeof(statuses[0]); ++i) {
    secondStatus = statuses[i];
    CHECK(invoke(1, all, sampleCases, 3, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && observed.count == 3);
    CHECK(observed.order[0] == 1 && observed.order[1] == 2 && observed.order[2] == 3);
    CHECK(strstr(observed.error, "[  FAILED  ] second") && strstr(observed.output, "[       OK ] Third"));
    CHECK(invoke(3, selected, sampleCases, 3, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && observed.count == 1 && observed.order[0] == 2);
    CHECK(strstr(observed.error, "[  FAILED  ] second") && !strstr(observed.output, "Third"));
  }
  return EXIT_SUCCESS;
}

static int
ordinaryFailure
  (
    void
  )
{
  ++callCount;
  return EXIT_FAILURE;
}

static int
skips
  (
    void
  )
{
  struct Observation observed;
  const char* parent = Arcadia_Tests_getCaseName();
  char* all[] = { "runner" };
  char* selected[] = { "runner", "--case", "second" };
  secondStatus = Arcadia_Testing_Skip;
  CHECK(invoke(3, selected, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == Arcadia_Testing_Skip && observed.count == 1);
  CHECK(strstr(observed.output, "[  SKIPPED ] second") && !observed.error[0]);
  CHECK(!strstr(observed.output, "[       OK ] second"));
  CHECK(invoke(1, all, sampleCases, 3, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 3);
  CHECK(strstr(observed.output, "[  SKIPPED ] second") && strstr(observed.output, "[       OK ] Third"));
  const Arcadia_Testing_Case skipped[] = { { "one", second }, { "two", second } };
  CHECK(invoke(1, all, skipped, 2, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == Arcadia_Testing_Skip && observed.count == 2 && !observed.error[0]);
  char* listing[] = { "runner", "--list" };
  CHECK(invoke(2, listing, skipped, 2, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && !observed.count && !observed.error[0]);
  CHECK(!strcmp(observed.output, "one\ntwo\n"));
  const Arcadia_Testing_Case failed[] = { { "one", second }, { "bad", NULL } };
  CHECK(invoke(1, all, failed, 2, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && !observed.count);
  const Arcadia_Testing_Case mixed[] = { { "skip", second }, { "failure", ordinaryFailure }, { "pass", first }, { "skipAgain", second } };
  CHECK(invoke(1, all, mixed, 4, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && observed.count == 4);
  CHECK(strstr(observed.error, "[  FAILED  ] failure") && strstr(observed.output, "[  SKIPPED ] skipAgain"));
  CHECK(Arcadia_Tests_getCaseName() == parent);
  secondStatus = EXIT_SUCCESS;
  return EXIT_SUCCESS;
}

static void
checkInnerCase
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertEqualCxxString(thread, "Inner", Arcadia_Tests_getCaseName());
}

static int
caseNameProbe
  (
    void
  )
{
  ++callCount;
  if (!Arcadia_Tests_getCaseName() || strcmp(Arcadia_Tests_getCaseName(), "Inner")) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&checkInnerCase)) return EXIT_FAILURE;
  return Arcadia_Tests_getCaseName() && !strcmp(Arcadia_Tests_getCaseName(), "Inner") ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
caseContext
  (
    void
  )
{
  struct Observation observed;
  const char* parent = Arcadia_Tests_getCaseName();
  CHECK(parent && !strcmp(parent, "CaseContext"));
  CHECK(Arcadia_Tests_setCaseName("manual-outer") == parent);
  const Arcadia_Testing_Case cases[] = { { "Inner", caseNameProbe } };
  char* all[] = { "probe" };
  CHECK(invoke(1, all, cases, 1, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && observed.count == 1);
  CHECK(!strcmp(Arcadia_Tests_getCaseName(), "manual-outer"));
  char* list[] = { "probe", "--list" };
  CHECK(invoke(2, list, cases, 1, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_SUCCESS && !observed.count && !strcmp(observed.output, "Inner\n"));
  CHECK(!strcmp(Arcadia_Tests_getCaseName(), "manual-outer"));
  char* unknown[] = { "probe", "--case", "Missing" };
  CHECK(invoke(3, unknown, cases, 1, &observed) == EXIT_SUCCESS);
  CHECK(observed.status == EXIT_FAILURE && !observed.count);
  CHECK(!strcmp(Arcadia_Tests_getCaseName(), "manual-outer"));
  CHECK(!strcmp(Arcadia_Tests_setCaseName(parent), "manual-outer"));
  return EXIT_SUCCESS;
}

enum AssertionKind {
  Assertion_Natural, Assertion_Integer, Assertion_Real, Assertion_Size, Assertion_Status,
  Assertion_String, Assertion_Bytes, Assertion_LongBytes, Assertion_NullString,
  Assertion_NullBuffer, Assertion_ByteLength, Assertion_NaN,
};

static enum AssertionKind assertionKind;
static unsigned expectedEvaluations, actualEvaluations, threadEvaluations;
static unsigned expectedSizeEvaluations, actualSizeEvaluations;
static int assertionLine;
static Arcadia_Status assertionStatus;
static int assertionJumped;

static void
assertionMismatch
  (
    Arcadia_Thread* thread
  )
{
  const unsigned char expected[] = { 'A', 0, 'B' };
  const unsigned char actual[] = { 'A', 0, 'C' };
  unsigned char longExpected[80], longActual[80];
  memset(longExpected, 'x', sizeof(longExpected));
  memcpy(longActual, longExpected, sizeof(longActual));
  longActual[70] = 'z';
  switch (assertionKind) {
    case Assertion_Natural:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualNatural64((++threadEvaluations, thread), (++expectedEvaluations, UINT64_MAX), (++actualEvaluations, UINT64_MAX - 1));
      break;
    case Assertion_Integer:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualInteger64((++threadEvaluations, thread), (++expectedEvaluations, INT64_MIN), (++actualEvaluations, INT64_MAX));
      break;
    case Assertion_Real:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualReal64((++threadEvaluations, thread), (++expectedEvaluations, 1.5), (++actualEvaluations, -2.25));
      break;
    case Assertion_Size:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualSize((++threadEvaluations, thread), (++expectedEvaluations, SIZE_MAX), (++actualEvaluations, 0));
      break;
    case Assertion_Status:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualStatus((++threadEvaluations, thread), (++expectedEvaluations, Arcadia_Status_SemanticalError), (++actualEvaluations, Arcadia_Status_SyntacticalError));
      break;
    case Assertion_String:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualCxxString((++threadEvaluations, thread), (++expectedEvaluations, "one\n"), (++actualEvaluations, "two\t"));
      break;
    case Assertion_Bytes:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualBytes((++threadEvaluations, thread), (++expectedEvaluations, expected), (++expectedSizeEvaluations, sizeof(expected)), (++actualEvaluations, actual), (++actualSizeEvaluations, sizeof(actual)));
      break;
    case Assertion_LongBytes:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualBytes((++threadEvaluations, thread), (++expectedEvaluations, longExpected), (++expectedSizeEvaluations, sizeof(longExpected)), (++actualEvaluations, longActual), (++actualSizeEvaluations, sizeof(longActual)));
      break;
    case Assertion_NullString:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualCxxString((++threadEvaluations, thread), (++expectedEvaluations, NULL), (++actualEvaluations, ""));
      break;
    case Assertion_NullBuffer:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualBytes((++threadEvaluations, thread), (++expectedEvaluations, NULL), (++expectedSizeEvaluations, 3), (++actualEvaluations, actual), (++actualSizeEvaluations, sizeof(actual)));
      break;
    case Assertion_ByteLength:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualBytes((++threadEvaluations, thread), (++expectedEvaluations, "ab"), (++expectedSizeEvaluations, 2), (++actualEvaluations, "abc"), (++actualSizeEvaluations, 3));
      break;
    case Assertion_NaN:
      assertionLine = __LINE__ + 1;
      Arcadia_Tests_assertEqualReal64((++threadEvaluations, thread), (++expectedEvaluations, NAN), (++actualEvaluations, NAN));
      break;
  }
}

static int
assertionProbe
  (
    void
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) return EXIT_FAILURE;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  assertionJumped = 0;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) assertionMismatch(thread);
  else assertionJumped = 1;
  Arcadia_Thread_popJumpTarget(thread);
  assertionStatus = Arcadia_Thread_getStatus(thread);
  Arcadia_Process_relinquish(process);
  return assertionStatus ? EXIT_FAILURE : EXIT_SUCCESS;
}

static void
assertionSuccess
  (
    Arcadia_Thread* thread
  )
{
  const unsigned char bytes[] = { 0, 1, 255 };
  Arcadia_Tests_assertEqualNatural64(thread, UINT64_MAX, UINT64_MAX);
  Arcadia_Tests_assertEqualInteger64(thread, INT64_MIN, INT64_MIN);
  Arcadia_Tests_assertEqualReal64(thread, INFINITY, INFINITY);
  Arcadia_Tests_assertEqualReal64(thread, -0.0, 0.0);
  Arcadia_Tests_assertEqualSize(thread, SIZE_MAX, SIZE_MAX);
  Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ValueRaised, Arcadia_Status_ValueRaised);
  Arcadia_Tests_assertEqualCxxString(thread, "same", "same");
  Arcadia_Tests_assertEqualCxxString(thread, NULL, NULL);
  Arcadia_Tests_assertEqualBytes(thread, bytes, sizeof(bytes), bytes, sizeof(bytes));
  Arcadia_Tests_assertEqualBytes(thread, NULL, 0, bytes, 0);
  if (1) Arcadia_Tests_assertEqualNatural64(thread, 1, 1);
  else Arcadia_Tests_assertEqualNatural64(thread, 0, 1);
}

static int
valueAssertions
  (
    void
  )
{
  CHECK(Arcadia_Tests_safeExecute(&assertionSuccess));
  struct Expectation { enum AssertionKind kind; const char* first; const char* second; int bytes; } expectations[] = {
    { Assertion_Natural, "expected=18446744073709551615", "actual=18446744073709551614", 0 },
    { Assertion_Integer, "expected=-9223372036854775808", "actual=9223372036854775807", 0 },
    { Assertion_Real, "expected=1.5", "actual=-2.25", 0 },
    { Assertion_Size, "Size equality", "actual=0", 0 },
    { Assertion_Status, "expected=23", "actual=24", 0 },
    { Assertion_String, "expected=\"one\\n\"", "actual=\"two\\t\"", 0 },
    { Assertion_Bytes, "expected=\"A\\x00B\"", "actual-byte=0x43", 1 },
    { Assertion_LongBytes, "first-difference=70 expected-byte=0x78", "actual-byte=0x7a", 1 },
    { Assertion_NullString, "expected=<null>", "actual=\"\"", 0 },
    { Assertion_NullBuffer, "expected=<null> (length=3)", "actual=\"A\\x00C\"", 1 },
    { Assertion_ByteLength, "expected-byte=<end>", "actual-byte=0x63", 1 },
    { Assertion_NaN, "Real64 equality", "actual=", 0 },
  };
  const Arcadia_Testing_Case cases[] = { { "AssertionProbe", assertionProbe } };
  char* argv[] = { "probe" };
  const char* parent = Arcadia_Tests_getCaseName();
  for (size_t i = 0; i < sizeof(expectations) / sizeof(expectations[0]); ++i) {
    struct Observation observed;
    assertionKind = expectations[i].kind;
    expectedEvaluations = actualEvaluations = threadEvaluations = 0;
    expectedSizeEvaluations = actualSizeEvaluations = 0;
    assertionStatus = Arcadia_Status_Success;
    CHECK(invoke(1, argv, cases, 1, &observed) == EXIT_SUCCESS);
    CHECK(observed.status == EXIT_FAILURE && assertionStatus == Arcadia_Status_TestFailed && assertionJumped);
    CHECK(expectedEvaluations == 1 && actualEvaluations == 1 && threadEvaluations == 1);
    CHECK(expectedSizeEvaluations == (unsigned)expectations[i].bytes && actualSizeEvaluations == (unsigned)expectations[i].bytes);
    char location[32];
    snprintf(location, sizeof(location), ":%d:", assertionLine);
    CHECK(strstr(observed.error, __FILE__) && strstr(observed.error, location));
    CHECK(strstr(observed.error, "case `AssertionProbe`"));
    CHECK(strstr(observed.error, expectations[i].first) && strstr(observed.error, expectations[i].second));
    CHECK(Arcadia_Tests_getCaseName() == parent);
    if (assertionKind == Assertion_Size) {
      char maximum[64];
      snprintf(maximum, sizeof(maximum), "expected=%zu", (size_t)SIZE_MAX);
      CHECK(strstr(observed.error, maximum));
    }
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  const Arcadia_Testing_Case cases[] = {
    { "Execution", execution }, { "Selection", selection }, { "Arguments", arguments },
    { "Descriptors", descriptors }, { "Discovery", discovery }, { "Failures", failures },
    { "CaseContext", caseContext }, { "ValueAssertions", valueAssertions },
    { "Skips", skips },
  };
  const char* previous = Arcadia_Tests_getCaseName();
  int result = Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
  CHECK(Arcadia_Tests_getCaseName() == previous);
  return result;
}
