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

#include "Arcadia/Engine/Tests/VisualResources/Cases.h"
#include "Arcadia/Testing/Include.h"

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Icon", &Arcadia_Engine_Tests_VisualResources_icon },
    { "ColorSource", &Arcadia_Engine_Tests_VisualResources_colorSource },
    { "MaterialResource", &Arcadia_Engine_Tests_VisualResources_materialResource },
    { "ThinResources", &Arcadia_Engine_Tests_VisualResources_thinResources },
    { "ResourceLifecycle", &Arcadia_Engine_Tests_VisualResources_resourceLifecycle },
    { "ModelConstruction", &Arcadia_Engine_Tests_VisualResources_modelConstruction },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
