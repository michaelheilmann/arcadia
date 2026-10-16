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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Media/Include.h"
#include "Operations.h"
#include "FillModels.h"
#include "Formats.h"

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Operations", &Arcadia_Media_Tests_PixelBuffer_operations },
    { "Fills", &Arcadia_Media_Tests_PixelBuffer_fills },
    { "FormatMetadata", &Arcadia_Media_Tests_PixelBuffer_formatMetadata },
    { "FormatQueries", &Arcadia_Media_Tests_PixelBuffer_formatQueries },
    { "FormatConsumers", &Arcadia_Media_Tests_PixelBuffer_formatConsumers },
    { "Resize", &Arcadia_Media_Tests_PixelBuffer_resize },
    { "BulkResize", &Arcadia_Media_Tests_PixelBuffer_bulkResize },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
