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

#include "Arcadia/Engine/Tests/Nodes/Cases.h"
#include "Arcadia/Testing/Include.h"

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Node", &Arcadia_Engine_Tests_Nodes_node },
    { "NodeFactory", &Arcadia_Engine_Tests_Nodes_nodeFactory },
    { "RenderNodes", &Arcadia_Engine_Tests_Nodes_renderNodes },
    { "Engine", &Arcadia_Engine_Tests_Nodes_engine },
    { "Scene", &Arcadia_Engine_Tests_Nodes_scene },
    { "Application", &Arcadia_Engine_Tests_Nodes_application },
    { "SceneManager", &Arcadia_Engine_Tests_Nodes_sceneManager },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
