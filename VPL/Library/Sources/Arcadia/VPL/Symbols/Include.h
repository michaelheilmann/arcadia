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

#if !defined(ARCADIA_VPL_SYMBOLS_INCLUDE_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_INCLUDE_H_INCLUDED

#pragma push_macro("ARCADIA_VPL_PRIVATE")
#undef ARCADIA_VPL_PRIVATE
#define ARCADIA_VPL_PRIVATE (1)

#include "Arcadia/VPL/Symbols/BuiltinType.h"
#include "Arcadia/VPL/Symbols/ConstantRecord.h"
#include "Arcadia/VPL/Symbols/ConstantKind.h"
#include "Arcadia/VPL/Symbols/ConstantRecord.h"
#include "Arcadia/VPL/Symbols/ConstantScalar.h"
#include "Arcadia/VPL/Symbols/Procedure.h"
#include "Arcadia/VPL/Symbols/Program.h"
#include "Arcadia/VPL/Symbols/Stage.h"
#include "Arcadia/VPL/Symbols/Symbol.h"
#include "Arcadia/VPL/Symbols/Variable.h"
#include "Arcadia/VPL/Symbols/VariableScalar.h"

#undef ARCADIA_VPL_PRIVATE
#pragma pop_macro("ARCADIA_VPL_PRIVATE")

#endif // ARCADIA_VPL_SYMBOLS_INCLUDE_H_INCLUDED
