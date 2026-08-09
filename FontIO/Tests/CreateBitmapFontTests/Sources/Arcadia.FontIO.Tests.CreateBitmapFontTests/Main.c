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
  static const Arcadia_Natural32Value g_codePoint = 0x41; // 'A'
  static const Arcadia_Integer32Value g_pixelSize = 16;

  Arcadia_String* path = Arcadia_String_createFromCxxString(thread, g_fontFilePath);
  Arcadia_FontIO_BitmapFont* bitmapFont = Arcadia_FontIO_BitmapFont_create(thread, path, g_pixelSize);

  // (1) Get the glyph information.
  Arcadia_FontIO_GlyphInformation glyphInformation;
  Arcadia_FontIO_BitmapFont_getGlyphInformation(thread, bitmapFont, g_codePoint, &glyphInformation);
  Arcadia_Tests_assertTrue(thread, glyphInformation.width > 0);
  Arcadia_Tests_assertTrue(thread, glyphInformation.height > 0);

  // (2) Get the glyph bitmap.
  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_FontIO_BitmapFont_getGlyphBitmap(thread, bitmapFont, g_codePoint);
  Arcadia_Tests_assertTrue(thread, NULL != pixelBuffer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, pixelBuffer) > 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getNumberOfRows(thread, pixelBuffer) > 0);

  // (3) The bitmap must contain at least one covered pixel.
  {
    Arcadia_SizeValue lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, pixelBuffer);
    Arcadia_Natural8Value* bytes = pixelBuffer->bytes;
    Arcadia_BooleanValue covered = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue y = 0; y < (Arcadia_SizeValue)Arcadia_Media_PixelBuffer_getNumberOfRows(thread, pixelBuffer); ++y) {
      for (Arcadia_SizeValue x = 0; x < (Arcadia_SizeValue)Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, pixelBuffer); ++x) {
        if (bytes[y * lineStride + x * 4 + 3] > 0) {
          covered = Arcadia_BooleanValue_True;
        }
      }
    }
    Arcadia_Tests_assertTrue(thread, covered == Arcadia_BooleanValue_True);
  }

  // (4) A code point without a glyph must fail.
  {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_FontIO_BitmapFont_getGlyphInformation(thread, bitmapFont, 0x10FFFF, &glyphInformation);
      Arcadia_FontIO_BitmapFont_getGlyphBitmap(thread, bitmapFont, 0x10FFFF);
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
