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

#if !defined(ARCADIA_VPL_IR_STAGEIO_H_INCLUDED)
#define ARCADIA_VPL_IR_STAGEIO_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/VPL/IR/Type.h"

// The directions of a stage input or output.
#define Arcadia_VPL_IR_StageIO_Direction_In (0)
#define Arcadia_VPL_IR_StageIO_Direction_Out (1)

// The semantics of a stage input or output.
#define Arcadia_VPL_IR_StageIO_Semantics_None (0)
#define Arcadia_VPL_IR_StageIO_Semantics_Position (1)
#define Arcadia_VPL_IR_StageIO_Semantics_Color (2)
#define Arcadia_VPL_IR_StageIO_Semantics_TexCoord (3)
#define Arcadia_VPL_IR_StageIO_Semantics_FragmentColor (4)

// An input or output of a stage interface.
Arcadia_declareObjectType(u8"Arcadia.VPL.IR.StageIO", Arcadia_VPL_IR_StageIO,
                          u8"Arcadia.Object");

struct Arcadia_VPL_IR_StageIODispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_StageIO {
  Arcadia_Object _parent;
  /// The direction, one of Arcadia_VPL_IR_StageIO_Direction_*.
  Arcadia_Natural8Value direction;
  /// The name of the input or output.
  Arcadia_String* name;
  /// The type of the input or output.
  Arcadia_VPL_IR_Type* type;
  /// The location of the input or output.
  Arcadia_Integer32Value location;
  /// The semantics of the input or output, one of Arcadia_VPL_IR_StageIO_Semantics_*.
  Arcadia_Natural8Value semantics;
};

Arcadia_VPL_IR_StageIO*
Arcadia_VPL_IR_StageIO_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value direction,
    Arcadia_String* name,
    Arcadia_VPL_IR_Type* type,
    Arcadia_Integer32Value location,
    Arcadia_Natural8Value semantics
  );

#endif // ARCADIA_VPL_IR_STAGEIO_H_INCLUDED