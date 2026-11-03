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

#include "Arcadia/Engine/Include.h"
#include "Contracts.c.g"

#if Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty != 1
  #error("resource constants must remain usable in preprocessor expressions")
#endif
#if Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty != 1 \
 || Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty != 2 \
 || Arcadia_Engine_Visuals_EnterPassResource_CanvasSizeDirty != 4 \
 || Arcadia_Engine_Visuals_EnterPassResource_ClearColorDirty != 8 \
 || Arcadia_Engine_Visuals_EnterPassResource_ClearDepthDirty != 16 \
 || Arcadia_Engine_Visuals_EnterPassResource_RelativeViewportRectangleDirty != 32
  #error("existing resource flag values must be preserved")
#endif
#if Arcadia_Engine_Visuals_EnterPassResource_DepthTestDirty != 64 || Arcadia_Engine_Visuals_EnterPassResource_AllDirty != 127
  #error("depth-test flag must be included in the all-dirty mask")
#endif

static int
runTests
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(testResourceModels) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ResourceModels", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
