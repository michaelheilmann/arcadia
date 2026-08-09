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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arcadia/FontIO/Include.h"

static const char* g_fontFilePath = NULL;

static Arcadia_BooleanValue
fileExists
  (
    char const* path
  )
{
  FILE* file = fopen(path, "rb");
  if (!file) {
    return Arcadia_BooleanValue_False;
  }
  fclose(file);
  return Arcadia_BooleanValue_True;
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  static const Arcadia_Natural32Value g_codePoints[] = { 'A', 'B', 'C', 'a', 'b', 'g', '0', '1' };
  static const Arcadia_SizeValue g_numberOfCodePoints = sizeof(g_codePoints) / sizeof(Arcadia_Natural32Value);
  static const Arcadia_Integer32Value g_pixelSize = 16;

  Arcadia_String* path = Arcadia_String_createFromCxxString(thread, g_fontFilePath);
  Arcadia_FontIO_AtlasBitmapFont* atlasBitmapFont = Arcadia_FontIO_AtlasBitmapFont_create(thread, path, g_pixelSize);

  // (1) Get the information of the glyphs. This renders and packs them into the atlas.
  Arcadia_FontIO_AtlasGlyphInformation glyphInformations[8];
  for (Arcadia_SizeValue i = 0; i < g_numberOfCodePoints; ++i) {
    Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, atlasBitmapFont, g_codePoints[i], &glyphInformations[i]);
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].glyphInformation.width > 0);
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].glyphInformation.height > 0);
  }

  // (2) The atlas must be large enough to contain all glyph images ...
  Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, atlasBitmapFont);
  Arcadia_Tests_assertTrue(thread, NULL != atlas);
  for (Arcadia_SizeValue i = 0; i < g_numberOfCodePoints; ++i) {
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].x >= 0);
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].y >= 0);
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].x + glyphInformations[i].glyphInformation.width <= Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas));
    Arcadia_Tests_assertTrue(thread, glyphInformations[i].y + glyphInformations[i].glyphInformation.height <= Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas));
  }

  // (3) ... and the glyph images must not overlap.
  {
    static const Arcadia_Natural8Value g_padding = 1;
    for (Arcadia_SizeValue i = 0; i < g_numberOfCodePoints; ++i) {
      for (Arcadia_SizeValue j = i + 1; j < g_numberOfCodePoints; ++j) {
        Arcadia_BooleanValue overlaps =
          glyphInformations[i].x + glyphInformations[i].glyphInformation.width + g_padding > glyphInformations[j].x &&
          glyphInformations[j].x + glyphInformations[j].glyphInformation.width + g_padding > glyphInformations[i].x &&
          glyphInformations[i].y + glyphInformations[i].glyphInformation.height + g_padding > glyphInformations[j].y &&
          glyphInformations[j].y + glyphInformations[j].glyphInformation.height + g_padding > glyphInformations[i].y;
        Arcadia_Tests_assertTrue(thread, !overlaps);
      }
    }
  }

  // (4) Each glyph image within the atlas must contain at least one covered pixel.
  {
    Arcadia_SizeValue lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, atlas);
    Arcadia_Natural8Value* bytes = atlas->bytes;
    for (Arcadia_SizeValue i = 0; i < g_numberOfCodePoints; ++i) {
      Arcadia_BooleanValue covered = Arcadia_BooleanValue_False;
      for (Arcadia_Integer32Value y = 0; y < glyphInformations[i].glyphInformation.height && !covered; ++y) {
        for (Arcadia_Integer32Value x = 0; x < glyphInformations[i].glyphInformation.width && !covered; ++x) {
          if (bytes[(size_t)(glyphInformations[i].y + y) * lineStride + (size_t)(glyphInformations[i].x + x) * 4 + 3] > 0) {
            covered = Arcadia_BooleanValue_True;
          }
        }
      }
      Arcadia_Tests_assertTrue(thread, covered == Arcadia_BooleanValue_True);
    }
  }

  // (5) Requesting an already packed glyph must return the same position.
  {
    Arcadia_FontIO_AtlasGlyphInformation glyphInformation;
    Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, atlasBitmapFont, g_codePoints[0], &glyphInformation);
    Arcadia_Tests_assertTrue(thread, glyphInformation.x == glyphInformations[0].x);
    Arcadia_Tests_assertTrue(thread, glyphInformation.y == glyphInformations[0].y);
  }

  // (6) A code point without a glyph must fail.
  {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_FontIO_AtlasGlyphInformation glyphInformation;
      Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, atlasBitmapFont, 0x10FFFF, &glyphInformation);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_ArgumentValueInvalid) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      }
    }
  }
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (argc > 1 && argv[1]) {
    g_fontFilePath = argv[1];
  } else {
    // Fall back to well-known system fonts.
    static char const* g_fallbacks[] = {
      "C:/Windows/Fonts/arial.ttf",
      "C:/Windows/Fonts/segoeui.ttf",
      "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    };
    for (size_t i = 0; i < sizeof(g_fallbacks) / sizeof(char const*); ++i) {
      if (fileExists(g_fallbacks[i])) {
        g_fontFilePath = g_fallbacks[i];
        break;
      }
    }
  }
  if (!g_fontFilePath) {
    fprintf(stderr, "warning: no font file found, test skipped\n");
    return EXIT_SUCCESS;
  }
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
