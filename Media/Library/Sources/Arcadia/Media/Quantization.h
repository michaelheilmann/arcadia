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

#if !defined(ARCADIA_MEDIA_QUANTIZATION_H_INCLUDED)
#define ARCADIA_MEDIA_QUANTIZATION_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/SampleBuffer.h"
#include <assert.h>
#include <math.h>

/// @brief Quantize a Arcadia.Real32 value value to Arcadia.Integer16.
/// @param x The Arcadia.Real32 value.
/// @return The Arcadia.Integer16 value.
///
/// @remarks
/// x := x
///
/// raise #Arcadia_Status_ArgumentInvalid if x1 is no number (V1)
///
/// x2 := -1 if x1 is less than -1
/// x2 := +1 if x1 is greater than +1
/// x2 := x1 otherwise
///
/// Observation: less than -1 and greater than +1 also covers the cases when x is -infinity or +infinity, respectively.
///
/// x3 := x2 + 1
/// x4 := x3 / 2
///
/// Observation: x3 is now within the bounds of [0,1]
///
/// x5 := x4 * 65535 - 32768
///
/// x4 = 0 yields x5 := -32768 = Arcadia.Integer32.Minimum
/// x4 = 1 yields x5 := +37267 = Arcadia.Integer32.Maximum
///
/// y := x5
static inline Arcadia_Integer16Value
Arcadia_Media_quantizeInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x
  )
{
  if (isnan(x)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  if (x < -1.f) {
    x = -1.f;
  } else if (x > +1.f) {
    x = +1.f;
  }
  assert(!isinf(x));
  assert(!isnan(x));

  x = (x + 1.f) / 2.f;

  x = x * 65535.f - 32768.f;

  // Clamp again to ensure x stays within bounds despite potential floating-point imprecisions.
  if (x < Arcadia_Integer16Value_Minimum) {
    x = Arcadia_Integer16Value_Minimum;
  } else if (x > Arcadia_Integer16Value_Maximum) {
    x = Arcadia_Integer16Value_Maximum;
  } else {
    x = (Arcadia_Integer16Value)x;
  }
  return x;
}

/// @brief Quantize a Arcadia.Real32 value value to Arcadia.Integer8.
/// @param x The Arcadia.Real32 value.
/// @return The Arcadia.Integer8 value.
///
/// @remarks
/// x := x
///
/// raise #Arcadia_Status_ArgumentInvalid if x1 is no number (V1)
///
/// x2 := -1 if x1 is less than -1
/// x2 := +1 if x1 is greater than +1
/// x2 := x1 otherwise
///
/// Observation: less than -1 and greater than +1 also covers the cases when x is -infinity or +infinity, respectively.
///
/// x3 := x2 + 1
/// x4 := x3 / 2
///
/// Observation: x3 is now within the bounds of [0,1]
///
/// x5 := x4 * 255 - 128
///
/// x4 = 0 yields x5 := -128 = Arcadia.Integer8.Minimum
/// x4 = 1 yields x5 := +127 = Arcadia.Integer8.Maximum
///
/// y := x5
static inline Arcadia_Integer8Value
Arcadia_Media_quantizeInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x
  )
{
  if (isnan(x)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  if (x < -1.f) {
    x = -1.f;
  } else if (x > +1.f) {
    x = +1.f;
  }
  assert(!isinf(x));
  assert(!isnan(x));

  x = (x + 1.f) / 2.f;

  x = x * 255.f - 128.f;

  // Clamp again to ensure x stays within bounds despite potential floating-point imprecisions.
  if (x < Arcadia_Integer8Value_Minimum) {
    x = Arcadia_Integer8Value_Minimum;
  } else if (x > Arcadia_Integer8Value_Maximum) {
    x = Arcadia_Integer8Value_Maximum;
  } else {
    x = (Arcadia_Integer8Value)x;
  }
  return x;
}

/// @brief Quantize a Arcadia.Real32 value value to Arcadia.Natural16.
/// @param x The Arcadia.Real32 value.
/// @return The Arcadia.Natural16 value.
///
/// @remarks
/// x := x
///
/// raise #Arcadia_Status_ArgumentInvalid if x1 is no number (V1)
///
/// x2 := -1 if x1 is less than -1
/// x2 := +1 if x1 is greater than +1
/// x2 := x1 otherwise
///
/// Observation: less than -1 and greater than +1 also covers the cases when x is -infinity or +infinity, respectively.
///
/// x3 := x2 + 1
/// x4 := x3 / 2
///
/// Observation: x3 is now within the bounds of [0,1]
///
/// x5 := x4 * 65535
///
/// x4 = 0 yields x5 := 0 = Arcadia.Natural16.Minimum
/// x4 = 1 yields x5 := 65535 = Arcadia.Natural16.Maximum
///
/// y := x5
static inline Arcadia_Natural16Value
Arcadia_Media_quantizeNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x
  )
{
  if (isnan(x)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  if (x < -1.f) {
    x = -1.f;
  } else if (x > +1.f) {
    x = +1.f;
  }
  assert(!isinf(x));
  assert(!isnan(x));

  x = (x + 1.f) / 2.f;

  x = x * 65535.f;


  // Clamp again to ensure x stays within bounds despite potential floating-point imprecisions.
  if (x < Arcadia_Natural16Value_Minimum) {
    x = Arcadia_Natural16Value_Minimum;
  } else if (x > Arcadia_Natural16Value_Maximum) {
    x = Arcadia_Natural16Value_Maximum;
  } else {
    x = (Arcadia_Natural16Value)x;
  }
  return x;
}

/// @brief Quantize a Arcadia.Real32 value value to Arcadia.Natural8.
/// @param x The Arcadia.Real32 value.
/// @return The Arcadia.Natural8 value.
///
/// @remarks
/// x := x
///
/// raise #Arcadia_Status_ArgumentInvalid if x1 is no number (V1)
///
/// x2 := -1 if x1 is less than -1
/// x2 := +1 if x1 is greater than +1
/// x2 := x1 otherwise
///
/// Observation: less than -1 and greater than +1 also covers the cases when x is -infinity or +infinity, respectively.
///
/// x3 := x2 + 1
/// x4 := x3 / 2
///
/// Observation: x3 is now within the bounds of [0,1]
///
/// x5 := x4 * 255
///
/// x4 = 0 yields x5 := 0 = Arcadia.Natural8.Minimum
/// x4 = 1 yields x5 := 255 = Arcadia.Natural8.Maximum
///
/// y := x5
static Arcadia_Natural8Value
Arcadia_Media_quantizeNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value x
  )
{
  if (isnan(x)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  if (x < -1.f) {
    x = -1.f;
  } else if (x > +1.f) {
    x = +1.f;
  }
  assert(!isinf(x));
  assert(!isnan(x));

  x = (x + 1.f) / 2.f;

  x = x * 255.f;

  // Clamp again to ensure x stays within bounds despite potential floating-point imprecisions.
  if (x < Arcadia_Natural8Value_Minimum) {
    x = Arcadia_Natural8Value_Minimum;
  } else if (x > Arcadia_Natural8Value_Maximum) {
    x = Arcadia_Natural8Value_Maximum;
  } else {
    x = (Arcadia_Natural8Value)x;
  }
  return x;
}

#endif // ARCADIA_MEDIA_QUANTIZATION_H_INCLUDED
