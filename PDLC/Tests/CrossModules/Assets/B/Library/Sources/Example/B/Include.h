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

#ifndef EXAMPLE_B_INCLUDE_H_INCLUDED
#define EXAMPLE_B_INCLUDE_H_INCLUDED
#pragma push_macro("EXAMPLE_B_PRIVATE")
#undef EXAMPLE_B_PRIVATE
#define EXAMPLE_B_PRIVATE 1
#include "Example/B.h"
#include "Example/B/Derived.h"
#include "Example/B/Holder.h"
#include "Example/B/Child.h"
#include "Example/B/answer.h"
#include "Example/B/defaultFlavor.h"
#include "Example/B/answerFromA.h"
#pragma pop_macro("EXAMPLE_B_PRIVATE")
#endif
