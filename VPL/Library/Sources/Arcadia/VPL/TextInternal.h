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

#if !defined(ARCADIA_VPL_TEXTINTERNAL_H_INCLUDED)
#define ARCADIA_VPL_TEXTINTERNAL_H_INCLUDED
#include "Arcadia/VPL/Text.h"
#include "Arcadia/VPL/IR/Include.h"
#include "Arcadia/VPL/Diagnostics/VerificationDiagnostic.h"

#define VPL_TEXT_NAME (0)
#define VPL_TEXT_NUMBER (1)
#define VPL_TEXT_STRING (2)
#define VPL_TEXT_FORM (3)

Arcadia_declareObjectType(u8"Arcadia.VPL.Text.Node", Arcadia_VPL_Text_Node, u8"Arcadia.Object");

struct Arcadia_VPL_Text_NodeDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Text_Node {
  Arcadia_Object _parent;
  Arcadia_Natural8Value kind;
  Arcadia_String* text;
  Arcadia_List* items;
  Arcadia_Object* binding;
  Arcadia_Natural8Value state;
  Arcadia_SizeValue offset;
  Arcadia_SizeValue ordinal;
};

Arcadia_declareObjectType(u8"Arcadia.VPL.Text.Context", Arcadia_VPL_Text_Context, u8"Arcadia.Object");

struct Arcadia_VPL_Text_ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Text_Context {
  Arcadia_Object _parent;
  Arcadia_RuntimeByteArray* source;
  Arcadia_String* sourceName;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_List* roots;
  Arcadia_Map* locations;
  Arcadia_Map* types;
  Arcadia_Map* uniforms;
  Arcadia_Map* functions;
  Arcadia_Map* registers;
  Arcadia_Map* parameters;
  Arcadia_Map* interface;
  Arcadia_Map* blocks;
  Arcadia_StringBuilder* output;
  Arcadia_List* typeOrder;
  Arcadia_SizeValue position;
};

Arcadia_VPL_Text_Context*
Arcadia_VPL_Text_createContext
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

Arcadia_Object*
Arcadia_VPL_Text_own
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_Object* object
  );

Arcadia_VPL_Text_Node*
Arcadia_VPL_Text_readForm
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_SizeValue depth
  );

void
Arcadia_VPL_Text_trivia
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context
  );

void
Arcadia_VPL_Text_fail
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_SizeValue offset,
    Arcadia_Status status,
    const char* message
  );

void
Arcadia_VPL_Text_locate
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* context,
    Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic,
    Arcadia_SizeValue offset
  );

void
Arcadia_VPL_Text_append
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* builder,
    const void* bytes,
    Arcadia_SizeValue count
  );

#endif // ARCADIA_VPL_TEXTINTERNAL_H_INCLUDED
