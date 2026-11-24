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

// This is a test-only native-socket fixture. The production Arcadia.Network
// TCP transport does not exist yet. No game state, callbacks, or worker threads.
#include "Arcadia/Network/Include.h"
#include "Arcadia/Testing/Include.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
  typedef SOCKET Loopback_Socket;
  typedef int Loopback_AddressLength;
  #define LOOPBACK_INVALID INVALID_SOCKET
  #define LOOPBACK_SHUT_WRITE SD_SEND
  #define LOOPBACK_SEND_FLAGS 0
#else
  #include <errno.h>
  #include <fcntl.h>
  #include <netinet/in.h>
  #include <sys/select.h>
  #include <sys/socket.h>
  #include <unistd.h>
  typedef int Loopback_Socket;
  typedef socklen_t Loopback_AddressLength;
  #define LOOPBACK_INVALID (-1)
  #define LOOPBACK_SHUT_WRITE SHUT_WR
  #if defined(__APPLE__)
    #define LOOPBACK_SEND_FLAGS 0
  #else
    #define LOOPBACK_SEND_FLAGS MSG_NOSIGNAL
  #endif
#endif

#define LOOPBACK_MAX_FRAME (256u * 1024u)
#define LOOPBACK_BUFFER (4096u)
#define REQUIRE(x) do { if (!(x)) { Loopback_Fixture_destroy(&fixture); return EXIT_FAILURE; } } while (0)

typedef struct Loopback_Peer Loopback_Peer;
typedef struct Loopback_Fixture Loopback_Fixture;

struct Loopback_Peer {
  Loopback_Socket socket;
  Arcadia_Network_FrameDecoder decoder;
  uint8_t buffer[LOOPBACK_BUFFER];
  size_t cursor;
  size_t count;
};

struct Loopback_Fixture {
  Loopback_Socket listener;
  Loopback_Peer client[2];
  Loopback_Peer server[2];
  struct sockaddr_in address;
  int socketLibraryReady;
};

static int
Loopback_socketError
  (
    void
  )
{
#if defined(_WIN32)
  return WSAGetLastError();
#else
  return errno;
#endif
}

static int
Loopback_wouldBlock
  (
    int error
  )
{
#if defined(_WIN32)
  return error == WSAEWOULDBLOCK || error == WSAEINPROGRESS;
#else
  return error == EWOULDBLOCK || error == EAGAIN || error == EINPROGRESS;
#endif
}

static void
Loopback_closeSocket
  (
    Loopback_Socket* socket
  )
{
  if (*socket != LOOPBACK_INVALID) {
#if defined(_WIN32)
    closesocket(*socket);
#else
    close(*socket);
#endif
    *socket = LOOPBACK_INVALID;
  }
}

static int
Loopback_nonBlocking
  (
    Loopback_Socket socket
  )
{
#if defined(_WIN32)
  u_long enabled = 1;
  return ioctlsocket(socket, FIONBIO, &enabled) == 0;
#else
  int flags = fcntl(socket, F_GETFL, 0);
  if (flags < 0 || fcntl(socket, F_SETFL, flags | O_NONBLOCK) != 0) {
    return 0;
  }
#if defined(__APPLE__)
  int yes = 1;
  return setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(yes)) == 0;
#else
  return 1;
#endif
#endif
}

// All socket waits are bounded: no blocking accept/read/write in any test.
static int
Loopback_wait
  (
    Loopback_Socket socket,
    int writable
  )
{
  fd_set set;
#if !defined(_WIN32)
  if (socket >= FD_SETSIZE) {
    return 0;
  }
#endif
  FD_ZERO(&set);
  FD_SET(socket, &set);
  struct timeval timeout = { 2, 0 };
  int result = select((int)(socket + 1), writable ? NULL : &set,
                      writable ? &set : NULL, NULL, &timeout);
  return result == 1 && FD_ISSET(socket, &set);
}

static void
Loopback_Peer_destroy
  (
    Loopback_Peer* peer
  )
{
  Arcadia_Network_FrameDecoder_uninitialize(&peer->decoder);
  Loopback_closeSocket(&peer->socket);
  peer->cursor = 0;
  peer->count = 0;
}

static void
Loopback_Fixture_destroy
  (
    Loopback_Fixture* fixture
  )
{
  for (size_t i = 0; i < 2; i++) {
    Loopback_Peer_destroy(&fixture->client[i]);
    Loopback_Peer_destroy(&fixture->server[i]);
  }
  Loopback_closeSocket(&fixture->listener);
#if defined(_WIN32)
  if (fixture->socketLibraryReady) {
    WSACleanup();
  }
#endif
  fixture->socketLibraryReady = 0;
}

static int
Loopback_accept
  (
    Loopback_Fixture* fixture,
    Loopback_Peer* server
  )
{
  for (unsigned attempt = 0; attempt < 8; attempt++) {
    Loopback_Socket accepted = accept(fixture->listener, NULL, NULL);
    if (accepted != LOOPBACK_INVALID) {
      server->socket = accepted;
      return Loopback_nonBlocking(accepted) &&
             Arcadia_Network_Result_Success ==
               Arcadia_Network_FrameDecoder_initialize(&server->decoder, LOOPBACK_MAX_FRAME);
    }
    if (!Loopback_wouldBlock(Loopback_socketError()) || !Loopback_wait(fixture->listener, 0)) {
      return 0;
    }
  }
  return 0;
}

static int
Loopback_connectPair
  (
    Loopback_Fixture* fixture,
    size_t index
  )
{
  Loopback_Peer* client = &fixture->client[index];
  Loopback_Peer* server = &fixture->server[index];
  client->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client->socket == LOOPBACK_INVALID || !Loopback_nonBlocking(client->socket)) {
    return 0;
  }
  if (connect(client->socket, (struct sockaddr*)&fixture->address,
              (Loopback_AddressLength)sizeof(fixture->address)) != 0) {
    if (!Loopback_wouldBlock(Loopback_socketError()) || !Loopback_wait(client->socket, 1)) {
      return 0;
    }
    int error = 0;
    Loopback_AddressLength length = (Loopback_AddressLength)sizeof(error);
#if defined(_WIN32)
    if (getsockopt(client->socket, SOL_SOCKET, SO_ERROR, (char*)&error, &length) != 0 || error) {
#else
    if (getsockopt(client->socket, SOL_SOCKET, SO_ERROR, &error, &length) != 0 || error) {
#endif
      return 0;
    }
  }
  return Arcadia_Network_Result_Success ==
           Arcadia_Network_FrameDecoder_initialize(&client->decoder, LOOPBACK_MAX_FRAME) &&
         Loopback_accept(fixture, server);
}

static int
Loopback_Fixture_create
  (
    Loopback_Fixture* fixture
  )
{
  memset(fixture, 0, sizeof(*fixture));
  fixture->listener = LOOPBACK_INVALID;
  for (size_t i = 0; i < 2; i++) {
    fixture->client[i].socket = LOOPBACK_INVALID;
    fixture->server[i].socket = LOOPBACK_INVALID;
  }
#if defined(_WIN32)
  WSADATA startup;
  if (WSAStartup(MAKEWORD(2, 2), &startup)) {
    return 0;
  }
#endif
  fixture->socketLibraryReady = 1;
  fixture->listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fixture->listener == LOOPBACK_INVALID || !Loopback_nonBlocking(fixture->listener)) {
    return 0;
  }
  memset(&fixture->address, 0, sizeof(fixture->address));
  fixture->address.sin_family = AF_INET;
  fixture->address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  fixture->address.sin_port = htons(0); // OS-assigned port; no test port collisions.
  if (bind(fixture->listener, (struct sockaddr*)&fixture->address,
           (Loopback_AddressLength)sizeof(fixture->address)) ||
      listen(fixture->listener, 4)) {
    return 0;
  }
  Loopback_AddressLength length = (Loopback_AddressLength)sizeof(fixture->address);
  if (getsockname(fixture->listener, (struct sockaddr*)&fixture->address, &length) ||
      !Loopback_connectPair(fixture, 0) || !Loopback_connectPair(fixture, 1)) {
    return 0;
  }
  return 1;
}

// Return 1 on success, 0 on timeout/error. Native writes may be short.
static int
Loopback_sendAll
  (
    Loopback_Socket socket,
    const uint8_t* bytes,
    size_t length
  )
{
  size_t position = 0;
  while (position < length) {
    size_t available = length - position;
    if (available > 4096) {
      available = 4096;
    }
#if defined(_WIN32)
    int count = send(socket, (const char*)bytes + position, (int)available, 0);
#else
    int count = send(socket, bytes + position, available, LOOPBACK_SEND_FLAGS);
#endif
    if (count > 0) {
      position += (size_t)count;
    } else if (count < 0 && Loopback_wouldBlock(Loopback_socketError())) {
      if (!Loopback_wait(socket, 1)) {
        return 0;
      }
    } else {
      return 0;
    }
  }
  return 1;
}

static int
Loopback_sendFrame
  (
    Loopback_Peer* peer,
    const uint8_t* data,
    size_t length
  )
{
  if (length > LOOPBACK_MAX_FRAME) {
    return 0;
  }
  uint8_t* frame = malloc(length + Arcadia_Network_Frame_HeaderSize);
  if (!frame) {
    return 0;
  }
  size_t count = 0;
  int ok = Arcadia_Network_Result_Success ==
           Arcadia_Network_Frame_encode(data, length, frame,
                                        length + Arcadia_Network_Frame_HeaderSize, &count) &&
           Loopback_sendAll(peer->socket, frame, count);
  free(frame);
  return ok;
}

// 2 = frame complete; 1 = needs more data; 0 = would block;
// -1 = I/O error; -2 = peer EOF; -3 = malformed frame.
static int
Loopback_readStep
  (
    Loopback_Peer* peer
  )
{
  if (peer->cursor == peer->count) {
#if defined(_WIN32)
    int count = recv(peer->socket, (char*)peer->buffer, (int)sizeof(peer->buffer), 0);
#else
    int count = recv(peer->socket, peer->buffer, sizeof(peer->buffer), 0);
#endif
    if (count == 0) {
      return -2;
    }
    if (count < 0) {
      return Loopback_wouldBlock(Loopback_socketError()) ? 0 : -1;
    }
    peer->cursor = 0;
    peer->count = (size_t)count;
  }
  size_t consumed = 0;
  Arcadia_Network_Result status =
    Arcadia_Network_FrameDecoder_feed(&peer->decoder, peer->buffer + peer->cursor,
                                      peer->count - peer->cursor, &consumed);
  peer->cursor += consumed;
  if (status == Arcadia_Network_Result_Ready) {
    return 2;
  }
  if (status == Arcadia_Network_Result_Malformed) {
    return -3;
  }
  return status == Arcadia_Network_Result_Success ? 1 : -1;
}

static int
Loopback_expectFrame
  (
    Loopback_Peer* peer,
    const uint8_t* expected,
    size_t expectedLength
  )
{
  for (unsigned attempt = 0; attempt < 200; attempt++) {
    int step = Loopback_readStep(peer);
    if (step == 2) {
      const uint8_t* payload = NULL;
      size_t length = 0;
      int ok = Arcadia_Network_Result_Success ==
                 Arcadia_Network_FrameDecoder_peek(&peer->decoder, &payload, &length) &&
               length == expectedLength &&
               (!length || !memcmp(payload, expected, length));
      return ok && Arcadia_Network_Result_Success ==
                     Arcadia_Network_FrameDecoder_consume(&peer->decoder);
    }
    if (step < 0 || (step == 0 && !Loopback_wait(peer->socket, 0))) {
      return 0;
    }
  }
  return 0;
}

static int
Loopback_testTwoClientsRelay
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  Arcadia_Network_EventQueue* queue = NULL;
  const uint8_t first[] = { 1, 2, 0, 3 };
  const uint8_t second[] = { 9, 8, 7 };
  int ok = Arcadia_Network_Result_Success ==
             Arcadia_Network_EventQueue_create(&queue, 4, 64);
  for (size_t direction = 0; ok && direction < 2; direction++) {
    const uint8_t* data = direction ? second : first;
    size_t length = direction ? sizeof(second) : sizeof(first);
    size_t other = 1 - direction;
    ok = Loopback_sendFrame(&fixture.client[direction], data, length) &&
         Loopback_expectFrame(&fixture.server[direction], data, length) &&
         Arcadia_Network_Result_Success == Arcadia_Network_EventQueue_post(
           queue, (uint64_t)(direction + 1), Arcadia_Network_EventType_Data,
           0, data, length);
    Arcadia_Network_Event event = { 0 };
    if (ok) {
      ok = Arcadia_Network_Result_Success == Arcadia_Network_EventQueue_pop(queue, &event) &&
           event.connectionId == direction + 1 && event.dataLength == length &&
           Loopback_sendFrame(&fixture.server[other], event.data, event.dataLength) &&
           Loopback_expectFrame(&fixture.client[other], data, length);
    }
    Arcadia_Network_Event_dispose(&event);
  }
  Arcadia_Network_EventQueue_destroy(queue);
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testFragmentedFrame
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  uint8_t message[] = { 4, 0, 7, 21, 1, 99 };
  uint8_t frame[sizeof(message) + 4];
  size_t written = 0;
  int ok = Arcadia_Network_Result_Success ==
    Arcadia_Network_Frame_encode(message, sizeof(message), frame, sizeof(frame), &written);
  for (size_t i = 0; ok && i < written; i++) {
    ok = Loopback_sendAll(fixture.client[0].socket, frame + i, 1);
  }
  ok = ok && Loopback_expectFrame(&fixture.server[0], message, sizeof(message));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testEmptyFrame
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  int ok = Loopback_sendFrame(&fixture.client[0], NULL, 0) &&
           Loopback_expectFrame(&fixture.server[0], NULL, 0);
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testLargeBinaryFrame
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  const size_t length = 128u * 1024u;
  uint8_t* input = malloc(length);
  uint8_t* frame = malloc(length + 4);
  int ok = input != NULL && frame != NULL;
  if (ok) {
    for (size_t i = 0; i < length; i++) {
      input[i] = (uint8_t)(i * 37u + 19u);
    }
  }
  size_t framedLength = 0;
  if (ok) {
    ok = Arcadia_Network_Result_Success ==
      Arcadia_Network_Frame_encode(input, length, frame, length + 4, &framedLength);
  }
  size_t position = 0;
  int complete = 0;
  for (unsigned round = 0; ok && !complete && round < 10000; round++) {
    if (position < framedLength) {
      size_t chunk = framedLength - position;
      if (chunk > 512) {
        chunk = 512;
      }
#if defined(_WIN32)
      int sent = send(fixture.client[0].socket, (const char*)frame + position, (int)chunk, 0);
#else
      int sent = send(fixture.client[0].socket, frame + position, chunk, LOOPBACK_SEND_FLAGS);
#endif
      if (sent > 0) {
        position += (size_t)sent;
      } else if (sent < 0 && !Loopback_wouldBlock(Loopback_socketError())) {
        ok = 0;
      } else if (sent == 0) {
        ok = 0;
      }
    }
    int step = Loopback_readStep(&fixture.server[0]);
    if (step == 2) {
      const uint8_t* output = NULL;
      size_t outputLength = 0;
      complete = Arcadia_Network_Result_Success ==
                   Arcadia_Network_FrameDecoder_peek(&fixture.server[0].decoder,
                                                     &output, &outputLength) &&
                 outputLength == length && !memcmp(output, input, length);
      ok = ok && complete && position == framedLength;
    } else if (step < 0) {
      ok = 0;
    }
  }
  free(frame);
  free(input);
  Loopback_Fixture_destroy(&fixture);
  return ok && complete ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testCoalescedFrames
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  const uint8_t a[] = { 1, 3, 5 }, b[] = { 2, 4 };
  uint8_t combined[4 + sizeof(a) + 4 + sizeof(b)];
  size_t n = 0, m = 0;
  int ok = Arcadia_Network_Result_Success ==
             Arcadia_Network_Frame_encode(a, sizeof(a), combined, sizeof(combined), &n) &&
           Arcadia_Network_Result_Success ==
             Arcadia_Network_Frame_encode(b, sizeof(b), combined + n,
                                          sizeof(combined) - n, &m) &&
           Loopback_sendAll(fixture.client[0].socket, combined, n + m) &&
           Loopback_expectFrame(&fixture.server[0], a, sizeof(a)) &&
           Loopback_expectFrame(&fixture.server[0], b, sizeof(b));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testSimultaneousClients
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  uint8_t a[] = { 0x11, 0x22 }, b[] = { 0x33, 0x44 };
  int ok = Loopback_sendFrame(&fixture.client[0], a, sizeof(a)) &&
           Loopback_sendFrame(&fixture.client[1], b, sizeof(b)) &&
           Loopback_expectFrame(&fixture.server[1], b, sizeof(b)) &&
           Loopback_expectFrame(&fixture.server[0], a, sizeof(a)) &&
           Loopback_sendFrame(&fixture.server[0], b, sizeof(b)) &&
           Loopback_sendFrame(&fixture.server[1], a, sizeof(a)) &&
           Loopback_expectFrame(&fixture.client[0], b, sizeof(b)) &&
           Loopback_expectFrame(&fixture.client[1], a, sizeof(a));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testDisconnectReconnect
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  Loopback_Peer_destroy(&fixture.client[0]);
  int eof = 0;
  if (Loopback_wait(fixture.server[0].socket, 0)) {
    uint8_t byte;
#if defined(_WIN32)
    eof = recv(fixture.server[0].socket, (char*)&byte, 1, 0) == 0;
#else
    eof = recv(fixture.server[0].socket, &byte, 1, 0) == 0;
#endif
  }
  Loopback_Peer_destroy(&fixture.server[0]);
  uint8_t message[] = { 9, 0, 1 };
  int ok = eof && Loopback_connectPair(&fixture, 0) &&
           Loopback_sendFrame(&fixture.client[0], message, sizeof(message)) &&
           Loopback_expectFrame(&fixture.server[0], message, sizeof(message)) &&
           Loopback_sendFrame(&fixture.client[1], message, sizeof(message)) &&
           Loopback_expectFrame(&fixture.server[1], message, sizeof(message));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testMalformedIsolation
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  uint8_t invalid[] = { 0, 16, 0, 0 };
  int ok = Loopback_sendAll(fixture.client[0].socket, invalid, sizeof(invalid));
  int result = 0;
  for (unsigned i = 0; ok && i < 12 && result != -3; i++) {
    result = Loopback_readStep(&fixture.server[0]);
    if (result == 0) {
      ok = Loopback_wait(fixture.server[0].socket, 0);
    }
  }
  uint8_t normal[] = { 44, 0, 55 };
  ok = ok && result == -3 &&
       Loopback_sendFrame(&fixture.client[1], normal, sizeof(normal)) &&
       Loopback_expectFrame(&fixture.server[1], normal, sizeof(normal));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testGracefulHalfClose
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  uint8_t data[] = { 8, 8, 8 };
  int ok = Loopback_sendFrame(&fixture.client[0], data, sizeof(data)) &&
           shutdown(fixture.client[0].socket, LOOPBACK_SHUT_WRITE) == 0 &&
           Loopback_expectFrame(&fixture.server[0], data, sizeof(data));
  if (ok && Loopback_wait(fixture.server[0].socket, 0)) {
    uint8_t byte;
#if defined(_WIN32)
    ok = recv(fixture.server[0].socket, (char*)&byte, 1, 0) == 0;
#else
    ok = recv(fixture.server[0].socket, &byte, 1, 0) == 0;
#endif
  } else {
    ok = 0;
  }
  ok = ok && Loopback_sendFrame(&fixture.client[1], data, sizeof(data)) &&
       Loopback_expectFrame(&fixture.server[1], data, sizeof(data));
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testRepeatedSetupTeardown
  (
    void
  )
{
  for (unsigned i = 0; i < 12; i++) {
    Loopback_Fixture fixture;
    if (!Loopback_Fixture_create(&fixture)) {
      Loopback_Fixture_destroy(&fixture);
      return EXIT_FAILURE;
    }
    uint8_t data[] = { (uint8_t)i };
    int ok = Loopback_sendFrame(&fixture.client[1], data, 1) &&
             Loopback_expectFrame(&fixture.server[1], data, 1);
    Loopback_Fixture_destroy(&fixture);
    if (!ok) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

static int
Loopback_testQueueBackpressure
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  Arcadia_Network_EventQueue* queue = NULL;
  uint8_t data[] = { 1, 2, 3 };
  int ok = Arcadia_Network_Result_Success ==
             Arcadia_Network_EventQueue_create(&queue, 1, 3) &&
           Loopback_sendFrame(&fixture.client[0], data, sizeof(data)) &&
           Loopback_expectFrame(&fixture.server[0], data, sizeof(data)) &&
           Arcadia_Network_Result_Success ==
             Arcadia_Network_EventQueue_post(queue, 1, Arcadia_Network_EventType_Data, 0, data, 3) &&
           Arcadia_Network_Result_Full ==
             Arcadia_Network_EventQueue_post(queue, 2, Arcadia_Network_EventType_Data, 0, data, 3);
  Arcadia_Network_Event event = { 0 };
  if (ok) {
    ok = Arcadia_Network_Result_Success == Arcadia_Network_EventQueue_pop(queue, &event) &&
         Loopback_sendFrame(&fixture.server[1], event.data, event.dataLength) &&
         Loopback_expectFrame(&fixture.client[1], data, sizeof(data));
  }
  Arcadia_Network_Event_dispose(&event);
  Arcadia_Network_EventQueue_destroy(queue);
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
Loopback_testIdleHasNoData
  (
    void
  )
{
  Loopback_Fixture fixture;
  REQUIRE(Loopback_Fixture_create(&fixture));
  int ok = Loopback_readStep(&fixture.server[0]) == 0 &&
           Loopback_readStep(&fixture.server[1]) == 0;
  Loopback_Fixture_destroy(&fixture);
  return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  const Arcadia_Testing_Case cases[] = {
    { "TwoClientsRelay", Loopback_testTwoClientsRelay },
    { "FragmentedFrame", Loopback_testFragmentedFrame },
    { "EmptyFrame", Loopback_testEmptyFrame },
    { "LargeBinaryFrame", Loopback_testLargeBinaryFrame },
    { "CoalescedFrames", Loopback_testCoalescedFrames },
    { "SimultaneousClients", Loopback_testSimultaneousClients },
    { "DisconnectReconnect", Loopback_testDisconnectReconnect },
    { "MalformedIsolation", Loopback_testMalformedIsolation },
    { "GracefulHalfClose", Loopback_testGracefulHalfClose },
    { "RepeatedSetupTeardown", Loopback_testRepeatedSetupTeardown },
    { "QueueBackpressure", Loopback_testQueueBackpressure },
    { "IdleHasNoData", Loopback_testIdleHasNoData }
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
