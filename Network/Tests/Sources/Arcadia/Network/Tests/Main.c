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

#include "Arcadia/Network/Include.h"
#include "Arcadia/Testing/Include.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #include <pthread.h>
#endif

#define CHECK(x) do { if (!(x)) { return EXIT_FAILURE; } } while (0)
#define OK Arcadia_Network_Result_Success

static int
frameHeader
  (
    void
  )
{
  uint8_t header[4];
  CHECK(OK == Arcadia_Network_Frame_writeHeader(header, 0x01020304u));
  CHECK(header[0] == 1 && header[1] == 2 && header[2] == 3 && header[3] == 4);
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_Frame_writeHeader(NULL, 1));
#if SIZE_MAX > UINT32_MAX
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_Frame_writeHeader(header, (size_t)UINT32_MAX + 1));
#endif
  return EXIT_SUCCESS;
}

static int
frameEncode
  (
    void
  )
{
  uint8_t b[] = { 0, 1, 255, 0, 42 };
  uint8_t frame[9];
  size_t written = 99;
  CHECK(OK == Arcadia_Network_Frame_encode(b, sizeof(b), frame, sizeof(frame), &written));
  CHECK(written == 9);
  CHECK(frame[0] == 0 && frame[1] == 0 && frame[2] == 0 && frame[3] == 5);
  CHECK(!memcmp(frame + 4, b, 5));
  return EXIT_SUCCESS;
}

static int
frameEmpty
  (
    void
  )
{
  uint8_t frame[4] = { 255, 255, 255, 255 };
  size_t written = 1;
  CHECK(OK == Arcadia_Network_Frame_encode(NULL, 0, frame, 4, &written));
  CHECK(written == 4);
  CHECK(frame[0] == 0 && frame[1] == 0 && frame[2] == 0 && frame[3] == 0);
  return EXIT_SUCCESS;
}

static int
frameRejectShortOutput
  (
    void
  )
{
  uint8_t frame[4] = { 1, 2, 3, 4 }, data[2] = { 4, 5 };
  size_t written = 99;
  CHECK(Arcadia_Network_Result_InvalidArgument ==
        Arcadia_Network_Frame_encode(data, 2, frame, sizeof(frame), &written));
  CHECK(written == 0);
  CHECK(frame[0] == 1 && frame[1] == 2 && frame[2] == 3 && frame[3] == 4);
  return EXIT_SUCCESS;
}

static int
decodeFragmentation
  (
    void
  )
{
  uint8_t frame[] = { 0, 0, 0, 3, 11, 22, 33 };
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 8));
  for (size_t i = 0; i < sizeof(frame); ++i) {
    size_t consumed = 99;
    Arcadia_Network_Result status = Arcadia_Network_FrameDecoder_feed(&d, &frame[i], 1, &consumed);
    CHECK(consumed == 1);
    CHECK(status == (i == 6 ? Arcadia_Network_Result_Ready : OK));
  }
  const uint8_t* payload;
  size_t count;
  CHECK(OK == Arcadia_Network_FrameDecoder_peek(&d, &payload, &count));
  CHECK(count == 3 && payload[0] == 11 && payload[2] == 33);
  CHECK(OK == Arcadia_Network_FrameDecoder_consume(&d));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
decodeCoalesced
  (
    void
  )
{
  uint8_t frames[] = { 0, 0, 0, 2, 1, 2, 0, 0, 0, 1, 7 };
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 5));
  size_t first;
  CHECK(Arcadia_Network_Result_Ready == Arcadia_Network_FrameDecoder_feed(&d, frames, sizeof(frames), &first));
  CHECK(first == 6);
  const uint8_t* payload;
  size_t count;
  CHECK(OK == Arcadia_Network_FrameDecoder_peek(&d, &payload, &count));
  CHECK(count == 2 && payload[0] == 1 && payload[1] == 2);
  CHECK(OK == Arcadia_Network_FrameDecoder_consume(&d));
  size_t second;
  CHECK(Arcadia_Network_Result_Ready ==
        Arcadia_Network_FrameDecoder_feed(&d, frames + first, sizeof(frames) - first, &second));
  CHECK(second == 5);
  CHECK(OK == Arcadia_Network_FrameDecoder_peek(&d, &payload, &count));
  CHECK(count == 1 && payload[0] == 7);
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
decodeEmpty
  (
    void
  )
{
  uint8_t header[] = { 0, 0, 0, 0 };
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 4));
  size_t consumed;
  CHECK(Arcadia_Network_Result_Ready == Arcadia_Network_FrameDecoder_feed(&d, header, 4, &consumed));
  CHECK(consumed == 4);
  const uint8_t* payload;
  size_t count;
  CHECK(OK == Arcadia_Network_FrameDecoder_peek(&d, &payload, &count));
  CHECK(payload == NULL && count == 0);
  CHECK(OK == Arcadia_Network_FrameDecoder_consume(&d));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
decodeOversized
  (
    void
  )
{
  uint8_t header[] = { 0, 0, 0, 5 };
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 4));
  size_t consumed;
  CHECK(Arcadia_Network_Result_Malformed == Arcadia_Network_FrameDecoder_feed(&d, header, 4, &consumed));
  CHECK(consumed == 4);
  CHECK(Arcadia_Network_Result_InvalidState == Arcadia_Network_FrameDecoder_feed(&d, NULL, 0, &consumed));
  Arcadia_Network_FrameDecoder_reset(&d);
  header[3] = 0;
  CHECK(Arcadia_Network_Result_Ready == Arcadia_Network_FrameDecoder_feed(&d, header, 4, &consumed));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
decodeNoFeedBeforeConsume
  (
    void
  )
{
  uint8_t header[] = { 0, 0, 0, 0 };
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 4));
  size_t consumed;
  CHECK(Arcadia_Network_Result_Ready == Arcadia_Network_FrameDecoder_feed(&d, header, 4, &consumed));
  CHECK(Arcadia_Network_Result_InvalidState == Arcadia_Network_FrameDecoder_feed(&d, header, 4, &consumed));
  CHECK(consumed == 0);
  CHECK(OK == Arcadia_Network_FrameDecoder_consume(&d));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
decodeBadArguments
  (
    void
  )
{
  Arcadia_Network_FrameDecoder d = { 0 };
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_FrameDecoder_initialize(&d, 0));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_FrameDecoder_initialize(&d, (size_t)16 * 1024 * 1024 + 1));
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 8));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_FrameDecoder_initialize(&d, 8));
  size_t consumed;
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_FrameDecoder_feed(&d, NULL, 2, &consumed));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_FrameDecoder_feed(&d, NULL, 0, NULL));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  return EXIT_SUCCESS;
}

static int
byteQueueFifo
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  uint8_t data[] = { 1, 2, 3, 4, 5 };
  uint8_t output[5];
  size_t count;
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 5));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, data, 5));
  CHECK(OK == Arcadia_Network_ByteQueue_read(&q, output, 5, &count));
  CHECK(count == 5 && !memcmp(data, output, 5));
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
byteQueueWrap
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  uint8_t data[] = { 1, 2, 3, 4, 5 }, next[] = { 6, 7, 8 };
  uint8_t output[6];
  size_t count;
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 5));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, data, 5));
  CHECK(OK == Arcadia_Network_ByteQueue_consume(&q, 3));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, next, 3));
  CHECK(OK == Arcadia_Network_ByteQueue_read(&q, output, 5, &count));
  uint8_t expected[] = { 4, 5, 6, 7, 8 };
  CHECK(count == 5 && !memcmp(output, expected, 5));
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
byteQueueCapacity
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  uint8_t data[] = { 3, 4, 5 };
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 3));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, data, 3));
  CHECK(Arcadia_Network_Result_Full == Arcadia_Network_ByteQueue_append(&q, data, 1));
  CHECK(q.count == 3);
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_ByteQueue_consume(&q, 4));
  CHECK(q.count == 3);
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
byteQueuePeek
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  uint8_t data[] = { 9, 8, 7, 6 }, more[] = { 5, 4, 3 };
  const uint8_t* pointer;
  size_t count;
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 4));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, data, 4));
  CHECK(OK == Arcadia_Network_ByteQueue_consume(&q, 3));
  CHECK(OK == Arcadia_Network_ByteQueue_append(&q, more, 3));
  CHECK(OK == Arcadia_Network_ByteQueue_peek(&q, &pointer, &count));
  CHECK(count == 1 && pointer[0] == 6);
  CHECK(OK == Arcadia_Network_ByteQueue_consume(&q, 1));
  CHECK(OK == Arcadia_Network_ByteQueue_peek(&q, &pointer, &count));
  CHECK(count == 3 && !memcmp(pointer, more, 3));
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
byteQueueBadArguments
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_ByteQueue_initialize(&q, 0));
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 1));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_ByteQueue_append(&q, NULL, 1));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_ByteQueue_initialize(&q, 1));
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
byteQueueStress
  (
    void
  )
{
  Arcadia_Network_ByteQueue q = { 0 };
  CHECK(OK == Arcadia_Network_ByteQueue_initialize(&q, 7));
  for (unsigned i = 0; i < 10000; i++) {
    uint8_t expected = (uint8_t)i;
    uint8_t actual = 0;
    size_t count;
    CHECK(OK == Arcadia_Network_ByteQueue_append(&q, &expected, 1));
    CHECK(OK == Arcadia_Network_ByteQueue_read(&q, &actual, 1, &count));
    CHECK(count == 1 && expected == actual);
  }
  Arcadia_Network_ByteQueue_uninitialize(&q);
  return EXIT_SUCCESS;
}

static int
eventQueueFifo
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  uint8_t data[] = { 3, 4, 5 };
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 3, 16));
  for (uint64_t i = 1; i <= 3; i++) {
    CHECK(OK == Arcadia_Network_EventQueue_post(q, i, Arcadia_Network_EventType_Data, 0, data, 3));
  }
  for (uint64_t i = 1; i <= 3; i++) {
    CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
    CHECK(event.connectionId == i && event.dataLength == 3);
    CHECK(!memcmp(event.data, data, 3));
    Arcadia_Network_Event_dispose(&event);
  }
  CHECK(Arcadia_Network_Result_Empty == Arcadia_Network_EventQueue_pop(q, &event));
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
eventQueueCopiesPayload
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  uint8_t bytes[] = { 1, 2, 3 };
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 2, 4));
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 12, Arcadia_Network_EventType_Data, 0, bytes, 3));
  bytes[0] = 99;
  CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
  CHECK(event.data[0] == 1 && event.data[1] == 2);
  Arcadia_Network_EventQueue_destroy(q);
  CHECK(event.data[0] == 1);
  Arcadia_Network_Event_dispose(&event);
  return EXIT_SUCCESS;
}

static int
eventQueueCountLimit
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 1, 0));
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 1, Arcadia_Network_EventType_Connected, 0, NULL, 0));
  CHECK(Arcadia_Network_Result_Full ==
        Arcadia_Network_EventQueue_post(q, 2, Arcadia_Network_EventType_Connected, 0, NULL, 0));
  CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
  Arcadia_Network_Event_dispose(&event);
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 3, Arcadia_Network_EventType_Connected, 0, NULL, 0));
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
eventQueuePayloadLimit
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  uint8_t bytes[] = { 5, 6, 7 };
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 3, 4));
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 1, Arcadia_Network_EventType_Data, 0, bytes, 3));
  CHECK(Arcadia_Network_Result_Full == Arcadia_Network_EventQueue_post(q, 2, Arcadia_Network_EventType_Data, 0, bytes, 3));
  size_t count, payload;
  CHECK(OK == Arcadia_Network_EventQueue_getCounts(q, &count, &payload));
  CHECK(count == 1 && payload == 3);
  CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
  Arcadia_Network_Event_dispose(&event);
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 2, Arcadia_Network_EventType_Data, 0, bytes, 3));
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
eventQueueCloseDrain
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 2, 0));
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 7, Arcadia_Network_EventType_Connected, 0, NULL, 0));
  Arcadia_Network_EventQueue_close(q);
  CHECK(Arcadia_Network_Result_Closed == Arcadia_Network_EventQueue_post(q, 8, Arcadia_Network_EventType_Connected, 0, NULL, 0));
  CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
  CHECK(event.connectionId == 7);
  Arcadia_Network_Event_dispose(&event);
  CHECK(Arcadia_Network_Result_Closed == Arcadia_Network_EventQueue_pop(q, &event));
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
eventQueueWrap
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 3, 0));
  for (uint64_t i = 0; i < 100; i++) {
    CHECK(OK == Arcadia_Network_EventQueue_post(q, i, Arcadia_Network_EventType_Connected, 0, NULL, 0));
    CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
    CHECK(event.connectionId == i);
    Arcadia_Network_Event_dispose(&event);
  }
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
eventQueueBadArguments
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_EventQueue_create(&q, 0, 0));
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 1, 1));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_EventQueue_post(q, 1, Arcadia_Network_EventType_Data, 0, NULL, 1));
  CHECK(Arcadia_Network_Result_InvalidArgument == Arcadia_Network_EventQueue_post(q, 1, (Arcadia_Network_EventType)0, 0, NULL, 0));
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

typedef struct Producer {
  Arcadia_Network_EventQueue* queue;
  uint64_t base;
  int success;
} Producer;

#if defined(_WIN32)
static DWORD WINAPI
producerRun
  (
    LPVOID argument
  )
#else
static void*
producerRun
  (
    void* argument
  )
#endif
{
  Producer* p = argument;
  p->success = 1;
  for (uint64_t i = 0; i < 200; i++) {
    uint8_t data[] = { (uint8_t)i, (uint8_t)(i >> 1) };
    if (OK != Arcadia_Network_EventQueue_post(p->queue, p->base + i,
                                               Arcadia_Network_EventType_Data, 0, data, sizeof(data))) {
      p->success = 0;
      break;
    }
  }
#if defined(_WIN32)
  return 0;
#else
  return NULL;
#endif
}

static int
eventQueueConcurrentProducers
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 500, 1000));
  Producer a = { q, 1000, 0 }, b = { q, 2000, 0 };
#if defined(_WIN32)
  HANDLE ta = CreateThread(NULL, 0, producerRun, &a, 0, NULL);
  HANDLE tb = CreateThread(NULL, 0, producerRun, &b, 0, NULL);
  CHECK(ta && tb);
  CHECK(WaitForSingleObject(ta, INFINITE) == WAIT_OBJECT_0);
  CHECK(WaitForSingleObject(tb, INFINITE) == WAIT_OBJECT_0);
  CloseHandle(ta);
  CloseHandle(tb);
#else
  pthread_t ta, tb;
  CHECK(!pthread_create(&ta, NULL, producerRun, &a));
  CHECK(!pthread_create(&tb, NULL, producerRun, &b));
  CHECK(!pthread_join(ta, NULL));
  CHECK(!pthread_join(tb, NULL));
#endif
  CHECK(a.success && b.success);
  uint8_t seenA[200] = { 0 }, seenB[200] = { 0 };
  for (size_t i = 0; i < 400; i++) {
    Arcadia_Network_Event event = { 0 };
    CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
    CHECK(event.dataLength == 2);
    if (event.connectionId >= 1000 && event.connectionId < 1200) {
      size_t index = (size_t)(event.connectionId - 1000);
      CHECK(!seenA[index]);
      seenA[index] = 1;
      CHECK(event.data[0] == (uint8_t)index);
    } else if (event.connectionId >= 2000 && event.connectionId < 2200) {
      size_t index = (size_t)(event.connectionId - 2000);
      CHECK(!seenB[index]);
      seenB[index] = 1;
      CHECK(event.data[0] == (uint8_t)index);
    } else {
      return EXIT_FAILURE;
    }
    Arcadia_Network_Event_dispose(&event);
  }
  for (size_t i = 0; i < 200; i++) {
    CHECK(seenA[i] && seenB[i]);
  }
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

static int
frameQueueRoundtrip
  (
    void
  )
{
  Arcadia_Network_EventQueue* q = NULL;
  Arcadia_Network_FrameDecoder d = { 0 };
  uint8_t raw[] = { 7, 0, 8, 9, 0 };
  uint8_t encoded[9];
  size_t encodedLength, consumed;
  CHECK(OK == Arcadia_Network_EventQueue_create(&q, 4, 64));
  CHECK(OK == Arcadia_Network_FrameDecoder_initialize(&d, 20));
  CHECK(OK == Arcadia_Network_Frame_encode(raw, sizeof(raw), encoded, sizeof(encoded), &encodedLength));
  CHECK(Arcadia_Network_Result_Ready ==
        Arcadia_Network_FrameDecoder_feed(&d, encoded, encodedLength, &consumed));
  const uint8_t* payload;
  size_t count;
  CHECK(OK == Arcadia_Network_FrameDecoder_peek(&d, &payload, &count));
  CHECK(OK == Arcadia_Network_EventQueue_post(q, 77, Arcadia_Network_EventType_Data, 0, payload, count));
  CHECK(OK == Arcadia_Network_FrameDecoder_consume(&d));
  Arcadia_Network_FrameDecoder_uninitialize(&d);
  Arcadia_Network_Event event = { 0 };
  CHECK(OK == Arcadia_Network_EventQueue_pop(q, &event));
  CHECK(event.connectionId == 77 && event.dataLength == sizeof(raw) && !memcmp(raw, event.data, sizeof(raw)));
  Arcadia_Network_Event_dispose(&event);
  Arcadia_Network_EventQueue_destroy(q);
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "FrameHeader", frameHeader },
    { "FrameEncode", frameEncode },
    { "FrameEmpty", frameEmpty },
    { "FrameRejectShortOutput", frameRejectShortOutput },
    { "DecoderFragmentation", decodeFragmentation },
    { "DecoderCoalesced", decodeCoalesced },
    { "DecoderEmpty", decodeEmpty },
    { "DecoderOversized", decodeOversized },
    { "DecoderNoFeedBeforeConsume", decodeNoFeedBeforeConsume },
    { "DecoderBadArguments", decodeBadArguments },
    { "ByteQueueFifo", byteQueueFifo },
    { "ByteQueueWrap", byteQueueWrap },
    { "ByteQueueCapacity", byteQueueCapacity },
    { "ByteQueuePeek", byteQueuePeek },
    { "ByteQueueBadArguments", byteQueueBadArguments },
    { "ByteQueueStress", byteQueueStress },
    { "EventQueueFifo", eventQueueFifo },
    { "EventQueueCopiesPayload", eventQueueCopiesPayload },
    { "EventQueueCountLimit", eventQueueCountLimit },
    { "EventQueuePayloadLimit", eventQueuePayloadLimit },
    { "EventQueueCloseDrain", eventQueueCloseDrain },
    { "EventQueueWrap", eventQueueWrap },
    { "EventQueueBadArguments", eventQueueBadArguments },
    { "EventQueueConcurrentProducers", eventQueueConcurrentProducers },
    { "FrameQueueRoundtrip", frameQueueRoundtrip },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
