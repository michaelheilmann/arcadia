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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/TextInternal.h"

static Arcadia_VPL_Text_Node*
newNode
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Text_Node);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_Text_Node);
}

static void
lexicalError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_SizeValue offset,
    const char* message
  )
{
  Arcadia_VPL_Text_fail(thread, c, offset, Arcadia_Status_LexicalError, message);
}

static void
validateUTF8
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c
  )
{
  const unsigned char* b = Arcadia_RuntimeByteArray_getBytes(thread, c->source);
  Arcadia_SizeValue n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, c->source);
  for (Arcadia_SizeValue i = 0; i < n;) {
    unsigned char first = b[i];
    if (first < 0x80) { ++i; continue; }
    Arcadia_SizeValue count = first >= 0xc2 && first <= 0xdf ? 2 : first >= 0xe0 && first <= 0xef ? 3 : first >= 0xf0 && first <= 0xf4 ? 4 : 0;
    if (!count || count > n - i) lexicalError(thread, c, i, "lexical error: invalid UTF-8\n");
    unsigned int cp = first & (count == 2 ? 0x1f : count == 3 ? 0x0f : 0x07);
    for (Arcadia_SizeValue j = 1; j < count; ++j) {
      if ((b[i + j] & 0xc0) != 0x80) lexicalError(thread, c, i + j, "lexical error: invalid UTF-8 continuation\n");
      cp = (cp << 6) | (b[i + j] & 0x3f);
    }
    if ((count == 3 && cp < 0x800) || (count == 4 && cp < 0x10000) || (cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff) lexicalError(thread, c, i, "lexical error: invalid Unicode scalar encoding\n");
    i += count;
  }
}

void
Arcadia_VPL_Text_trivia
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c
  )
{
  const unsigned char* b = Arcadia_RuntimeByteArray_getBytes(thread, c->source);
  Arcadia_SizeValue n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, c->source);
  for (;;) {
    if (c->position >= n) return;
    unsigned char ch = b[c->position];
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') { ++c->position; continue; }
    if (ch == '\\' && c->position + 1 < n && (b[c->position + 1] == '\r' || b[c->position + 1] == '\n')) { ++c->position; continue; }
    if (ch == '/' && c->position + 1 < n && b[c->position + 1] == '/') {
      c->position += 2;
      while (c->position < n && b[c->position] != '\r' && b[c->position] != '\n') ++c->position;
      continue;
    }
    if (ch == '/' && c->position + 1 < n && b[c->position + 1] == '*') {
      Arcadia_SizeValue start = c->position;
      c->position += 2;
      while (c->position + 1 < n && !(b[c->position] == '*' && b[c->position + 1] == '/')) ++c->position;
      if (c->position + 1 >= n) lexicalError(thread, c, start, "lexical error: unterminated comment\n");
      c->position += 2;
      continue;
    }
    return;
  }
}

static Arcadia_BooleanValue
alpha
  (
    unsigned char ch
  )
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static Arcadia_BooleanValue
digit
  (
    unsigned char ch
  )
{
  return ch >= '0' && ch <= '9';
}

static Arcadia_BooleanValue
numberSpelling
  (
    const unsigned char* b,
    Arcadia_SizeValue n
  )
{
  Arcadia_SizeValue i = 0, digits = 0;
  if (i < n && (b[i] == '+' || b[i] == '-')) ++i;
  while (i < n && digit(b[i])) { ++digits; ++i; }
  if (i < n && b[i] == '.') {
    ++i;
    while (i < n && digit(b[i])) { ++digits; ++i; }
  }
  if (!digits) return Arcadia_BooleanValue_False;
  if (i < n && (b[i] == 'e' || b[i] == 'E')) {
    ++i;
    if (i < n && (b[i] == '+' || b[i] == '-')) ++i;
    Arcadia_SizeValue start = i;
    while (i < n && digit(b[i])) ++i;
    if (i == start) return Arcadia_BooleanValue_False;
  }
  return i == n;
}

static void
appendCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_StringBuilder* builder,
    unsigned int cp,
    Arcadia_SizeValue offset
  )
{
  if (cp >= 0xd800 && cp <= 0xdfff) lexicalError(thread, c, offset, "lexical error: surrogate escape is not a Unicode scalar\n");
  unsigned char bytes[4];
  Arcadia_SizeValue count;
  if (cp < 0x80) { bytes[0] = (unsigned char)cp; count = 1; }
  else if (cp < 0x800) { bytes[0] = 0xc0 | (cp >> 6); bytes[1] = 0x80 | (cp & 0x3f); count = 2; }
  else { bytes[0] = 0xe0 | (cp >> 12); bytes[1] = 0x80 | ((cp >> 6) & 0x3f); bytes[2] = 0x80 | (cp & 0x3f); count = 3; }
  Arcadia_VPL_Text_append(thread, builder, bytes, count);
}

static Arcadia_String*
stringToken
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c
  )
{
  const unsigned char* b = Arcadia_RuntimeByteArray_getBytes(thread, c->source);
  Arcadia_SizeValue n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, c->source), start = c->position;
  unsigned char quote = b[c->position++];
  Arcadia_StringBuilder* builder = (Arcadia_StringBuilder*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
  while (c->position < n) {
    unsigned char ch = b[c->position++];
    if (ch == quote) return (Arcadia_String*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder)));
    if (ch < 0x20) lexicalError(thread, c, c->position - 1, "lexical error: raw control character in string\n");
    if (ch == '\\') {
      Arcadia_SizeValue escape = c->position - 1;
      if (c->position == n) lexicalError(thread, c, start, "lexical error: unterminated string\n");
      ch = b[c->position++];
      if (ch == 'u') {
        unsigned int cp = 0;
        for (int i = 0; i < 4; ++i) {
          if (c->position == n) lexicalError(thread, c, escape, "lexical error: incomplete Unicode escape\n");
          unsigned char h = b[c->position++];
          unsigned int v = digit(h) ? h - '0' : h >= 'a' && h <= 'f' ? h - 'a' + 10 : h >= 'A' && h <= 'F' ? h - 'A' + 10 : 16;
          if (v == 16) lexicalError(thread, c, c->position - 1, "lexical error: invalid hexadecimal escape digit\n");
          cp = cp * 16 + v;
        }
        appendCodePoint(thread, c, builder, cp, escape);
        continue;
      }
      if (ch == 'b') ch = 8;
      else if (ch == 'f') ch = 12;
      else if (ch == 'n') ch = 10;
      else if (ch == 'r') ch = 13;
      else if (ch == 't') ch = 9;
      else if (ch != '\\' && ch != '\'' && ch != '"') lexicalError(thread, c, escape, "lexical error: unknown string escape\n");
      appendCodePoint(thread, c, builder, ch, escape);
    } else {
      Arcadia_SizeValue count = ch < 0x80 ? 1 : ch < 0xe0 ? 2 : ch < 0xf0 ? 3 : 4;
      Arcadia_VPL_Text_append(thread, builder, b + c->position - 1, count);
      c->position += count - 1;
    }
  }
  lexicalError(thread, c, start, "lexical error: unterminated string\n");
  return NULL;
}

Arcadia_VPL_Text_Node*
Arcadia_VPL_Text_readForm
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_SizeValue depth
  )
{
  if (!depth) validateUTF8(thread, c);
  if (depth > 128) Arcadia_VPL_Text_fail(thread, c, c->position, Arcadia_Status_SyntacticalError, "syntax error: form nesting exceeds 128\n");
  Arcadia_VPL_Text_trivia(thread, c);
  const unsigned char* b = Arcadia_RuntimeByteArray_getBytes(thread, c->source);
  Arcadia_SizeValue n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, c->source);
  if (c->position == n) Arcadia_VPL_Text_fail(thread, c, n, Arcadia_Status_SyntacticalError, "syntax error: unexpected end of input\n");
  Arcadia_VPL_Text_Node* node = (Arcadia_VPL_Text_Node*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)newNode(thread));
  node->offset = c->position;
  unsigned char ch = b[c->position];
  if (ch == '(') {
    ++c->position;
    node->kind = VPL_TEXT_FORM;
    node->items = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (;;) {
      Arcadia_VPL_Text_trivia(thread, c);
      if (c->position == n) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SyntacticalError, "syntax error: unterminated form\n");
      if (b[c->position] == ')') { ++c->position; break; }
      Arcadia_List_insertBackObjectReferenceValue(thread, node->items, (Arcadia_Object*)Arcadia_VPL_Text_readForm(thread, c, depth + 1));
    }
  } else if (ch == '\'' || ch == '"') {
    node->kind = VPL_TEXT_STRING;
    node->text = stringToken(thread, c);
  } else {
    Arcadia_SizeValue start = c->position;
    while (c->position < n) {
      ch = b[c->position];
      if (ch == '(' || ch == ')' || ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '/' || ch == '\\') break;
      ++c->position;
    }
    Arcadia_SizeValue count = c->position - start;
    if (!count) lexicalError(thread, c, start, "lexical error: unexpected character\n");
    if (digit(b[start]) || b[start] == '+' || b[start] == '-' || b[start] == '.') {
      node->kind = VPL_TEXT_NUMBER;
      if (!numberSpelling(b + start, count)) lexicalError(thread, c, start, "lexical error: malformed number\n");
    } else {
      node->kind = VPL_TEXT_NAME;
      Arcadia_SizeValue i = 0;
      while (i < count && b[start + i] == '_') ++i;
      if (i == count || !alpha(b[start + i++])) lexicalError(thread, c, start, "lexical error: invalid name\n");
      for (; i < count; ++i) if (!alpha(b[start + i]) && !digit(b[start + i]) && b[start + i] != '_') lexicalError(thread, c, start + i, "lexical error: invalid name character\n");
    }
    node->text = (Arcadia_String*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_String_create(thread,
      Arcadia_Value_makeRuntimeByteArrayValue(Arcadia_RuntimeByteArray_create(thread, b + start, count))));
  }
  return node;
}
