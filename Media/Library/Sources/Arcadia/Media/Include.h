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

#if !defined(ARCADIA_MEDIA_INCLUDE_H_INCLUDED)
#define ARCADIA_MEDIA_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_MEDIA_MODULE")
#define ARCADIA_MEDIA_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_MEDIA_EXPORT")
#define ARCADIA_MEDIA_EXPORT (1)

#include "Arcadia/Media/PixelBuffer.h"
#include "Arcadia/Media/PixelBufferOperations/CheckerboardFill.h"
#include "Arcadia/Media/PixelBufferOperations/Fill.h"
#include "Arcadia/Media/PixelBufferOperation.h"
#include "Arcadia/Media/PixelFormat.h"

#include "Arcadia/Media/SampleBuffer.h"
#include "Arcadia/Media/DSP/SineWave.h"
#include "Arcadia/Media/DSP/WhiteNoise.h"
#include "Arcadia/Media/SampleFormat.h"

#include "Arcadia/Media/VertexBuffer.h"

#include "Arcadia/Media/VertexDescriptor.h"
#include "Arcadia/Media/VertexDescriptorBuilder.h"
#include "Arcadia/Media/VertexElementDescriptor.h"
#include "Arcadia/Media/VertexElementSemantics.h"
#include "Arcadia/Media/VertexElementSyntactics.h"

#undef ARCADIA_MEDIA_EXPORT
#pragma pop_macro("ARCADIA_MEDIA_EXPORT")

#undef ARCADIA_MEDIA_MODULE
#pragma pop_macro("ARCADIA_MEDIA_MODULE")

#endif // ARCADIA_MEDIA_INCLUDE_H_INCLUDED
