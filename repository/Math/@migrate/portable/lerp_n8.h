#if !defined(LERP_N8_H_INCLUDED)
#define LERP_N8_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

/// Linear interpolation between uint8_t values.
/// @param a The start value.
/// @param b The end value.
/// @param t The interpolation variable.
/// @return The interpolated value.
/// @remarks
/// - t = 0   => a
/// - t = 128 => a + (b - a) * 0.5
/// - t = 255 => b 
static inline uint8_t
lerp_n8
  (
    uint8_t a,
    uint8_t b,
    uint8_t t
  )
{
  return a + (((int16_t)b - (int16_t)a) * t + 127) / 255;
}

#endif // LERP_N8_H_INCLUDED
