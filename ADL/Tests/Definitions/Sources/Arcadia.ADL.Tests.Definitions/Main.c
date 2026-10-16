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
#include "Arcadia.ADL.Tests.Definitions/ClassModels.h"

#include "Arcadia.ADL.Tests.Definitions/Audials/SampleBuffer.h"
#include "Arcadia.ADL.Tests.Definitions/Audials/SoundSourceNodes.h"
#include "Arcadia.ADL.Tests.Definitions/UI/ContainerComposition.h"

#include "Arcadia.ADL.Tests.Definitions/Visuals/BoxMeshGenerator.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Color.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Material.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Mesh.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Model.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/PixelBuffer.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/PlaneMeshGenerator.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/SphereMeshGenerator.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Texture.h"

#include "Arcadia.ADL.Tests.Definitions/DSP/Constant.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/SawtoothWave.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/SineWave.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/WhiteNoise.h"

#include "Arcadia.ADL.Tests.Definitions/Graphs/Checkerboard.h"
#include "Arcadia.ADL.Tests.Definitions/Graphs/Sine.h"
#include "Arcadia.ADL.Tests.Definitions/Graphs/ReferenceEdges.h"
#include "Arcadia.ADL.Tests.Definitions/Graphs/Dual.h"

static int
runUIContainerComposition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_UI_ContainerCompositionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAudialsSampleBufferDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Audials_SampleBufferDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAudialsSoundSourceNodes
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Audials_SoundSourceNodesTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsColorDefinition1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ColorDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsColorDefinition2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ColorDefinitionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPixelBufferDefinition1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPixelBufferDefinition2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPixelBufferDefinition3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsTextureDefinition1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsTextureEnums
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureEnums) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsTextureDefinition2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureDefinitionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsTextureDefinition3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureDefinitionTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsTextureDefinition4
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureDefinitionTest4) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialEnums
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialEnums) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition4
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest4) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition5
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest5) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMaterialDefinition6
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest6) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsMeshDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MeshDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsBoxMeshGenerator1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsBoxMeshGenerator2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsBoxMeshGenerator3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsSphereMeshGenerator1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_SphereMeshGeneratorTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsSphereMeshGenerator2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_SphereMeshGeneratorTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsSphereMeshGenerator3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_SphereMeshGeneratorTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPlaneMeshGenerator1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PlaneMeshGeneratorTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPlaneMeshGenerator2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PlaneMeshGeneratorTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsPlaneMeshGenerator3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PlaneMeshGeneratorTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsModelDefinition1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ModelDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsModelDefinition2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ModelDefinitionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVisualsModelDefinition3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ModelDefinitionTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDSPConstantDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_ConstantDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDSPSawtoothWaveDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_SawtoothWaveDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDSPSineWaveDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_SineWaveDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDSPWhiteNoiseDefinition
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_WhiteNoiseDefinitionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsCheckerboard1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsCheckerboard2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsCheckerboard3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsCheckerboard4
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest4) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsCheckerboard5
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest5) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine1
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine2
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine3
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine4
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest4) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine5
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest5) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine6
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest6) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine7
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest7) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runGraphsSine8
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Definitions_Graphs_SineTest8) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "UIContainerComposition", &runUIContainerComposition },
    { "AudialsSampleBufferDefinition", &runAudialsSampleBufferDefinition },
    { "AudialsSoundSourceNodes", &runAudialsSoundSourceNodes },
    { "DSPOperatorValidation", &Arcadia_ADL_Tests_Audials_dspOperatorValidation },
    { "VisualsColorDefinition1", &runVisualsColorDefinition1 },
    { "ColorModel", &Arcadia_ADL_Tests_colorModel },
    { "VisualsColorDefinition2", &runVisualsColorDefinition2 },
    { "VisualsColorAlpha", &Arcadia_ADL_Tests_Visuals_colorAlpha },
    { "VisualsColorAlphaInvalid", &Arcadia_ADL_Tests_Visuals_colorAlphaInvalid },
    { "VisualsColorAlphaGraph", &Arcadia_ADL_Tests_Visuals_colorAlphaGraph },
    { "VisualsColorAlphaAssets", &Arcadia_ADL_Tests_Visuals_colorAlphaAssets },
    { "VisualsPixelBufferDefinition1", &runVisualsPixelBufferDefinition1 },
    { "VisualsPixelBufferDefinition2", &runVisualsPixelBufferDefinition2 },
    { "VisualsPixelBufferDefinition3", &runVisualsPixelBufferDefinition3 },
    { "VisualsTextureDefinition1", &runVisualsTextureDefinition1 },
    { "VisualsTextureEnums", &runVisualsTextureEnums },
    { "VisualsTextureDefinition2", &runVisualsTextureDefinition2 },
    { "VisualsTextureDefinition3", &runVisualsTextureDefinition3 },
    { "VisualsTextureDefinition4", &runVisualsTextureDefinition4 },
    { "VisualsMaterialDefinition1", &runVisualsMaterialDefinition1 },
    { "VisualsMaterialEnums", &runVisualsMaterialEnums },
    { "VisualsMaterialDefinition2", &runVisualsMaterialDefinition2 },
    { "VisualsMaterialDefinition3", &runVisualsMaterialDefinition3 },
    { "VisualsMaterialDefinition4", &runVisualsMaterialDefinition4 },
    { "VisualsMaterialDefinition5", &runVisualsMaterialDefinition5 },
    { "VisualsMaterialDefinition6", &runVisualsMaterialDefinition6 },
    { "VisualsMeshDefinition", &runVisualsMeshDefinition },
    { "VisualsBoxMeshGenerator1", &runVisualsBoxMeshGenerator1 },
    { "VisualsBoxMeshGenerator2", &runVisualsBoxMeshGenerator2 },
    { "VisualsBoxMeshGenerator3", &runVisualsBoxMeshGenerator3 },
    { "VisualsSphereMeshGenerator1", &runVisualsSphereMeshGenerator1 },
    { "VisualsSphereMeshGenerator2", &runVisualsSphereMeshGenerator2 },
    { "VisualsSphereMeshGenerator3", &runVisualsSphereMeshGenerator3 },
    { "VisualsPlaneMeshGenerator1", &runVisualsPlaneMeshGenerator1 },
    { "VisualsPlaneMeshGenerator2", &runVisualsPlaneMeshGenerator2 },
    { "VisualsPlaneMeshGenerator3", &runVisualsPlaneMeshGenerator3 },
    { "VisualsModelDefinition1", &runVisualsModelDefinition1 },
    { "VisualsModelDefinition2", &runVisualsModelDefinition2 },
    { "VisualsModelDefinition3", &runVisualsModelDefinition3 },
    { "DSPConstantDefinition", &runDSPConstantDefinition },
    { "DSPSawtoothWaveDefinition", &runDSPSawtoothWaveDefinition },
    { "DSPSineWaveDefinition", &runDSPSineWaveDefinition },
    { "DSPWhiteNoiseDefinition", &runDSPWhiteNoiseDefinition },
    { "WhiteNoiseModel", &Arcadia_ADL_Tests_whiteNoiseModel },
    { "ConstantModel", &Arcadia_ADL_Tests_constantModel },
    { "SineWaveModel", &Arcadia_ADL_Tests_sineWaveModel },
    { "SawtoothWaveModel", &Arcadia_ADL_Tests_sawtoothWaveModel },
    { "GraphsCheckerboard1", &runGraphsCheckerboard1 },
    { "GraphsCheckerboard2", &runGraphsCheckerboard2 },
    { "GraphsCheckerboard3", &runGraphsCheckerboard3 },
    { "GraphsCheckerboard4", &runGraphsCheckerboard4 },
    { "GraphsCheckerboard5", &runGraphsCheckerboard5 },
    { "GraphsSine1", &runGraphsSine1 },
    { "GraphsSine2", &runGraphsSine2 },
    { "GraphsSine3", &runGraphsSine3 },
    { "GraphsSine4", &runGraphsSine4 },
    { "GraphsSine5", &runGraphsSine5 },
    { "GraphsSine6", &runGraphsSine6 },
    { "GraphsSine7", &runGraphsSine7 },
    { "GraphsSine8", &runGraphsSine8 },
    { "GraphsUnresolvedOutput", &Arcadia_ADL_Tests_Graphs_unresolvedOutput },
    { "GraphsUnresolvedFrequency", &Arcadia_ADL_Tests_Graphs_unresolvedFrequency },
    { "GraphsUnresolvedColor", &Arcadia_ADL_Tests_Graphs_unresolvedColor },
    { "GraphsUnresolvedOperation", &Arcadia_ADL_Tests_Graphs_unresolvedOperation },
    { "GraphsWrongColorFamily", &Arcadia_ADL_Tests_Graphs_wrongColorFamily },
    { "GraphsWrongOperationFamily", &Arcadia_ADL_Tests_Graphs_wrongOperationFamily },
    { "GraphsOutputReference", &Arcadia_ADL_Tests_Graphs_outputReference },
    { "GraphsSelfCycle", &Arcadia_ADL_Tests_Graphs_selfCycle },
    { "GraphsCrossCycle", &Arcadia_ADL_Tests_Graphs_crossCycle },
    { "GraphsRetainedEdges", &Arcadia_ADL_Tests_Graphs_retainedEdges },
    { "GraphsDualExpansion", &Arcadia_ADL_Tests_Graphs_dualExpansion },
    { "GraphsDualDeclarationOrder", &Arcadia_ADL_Tests_Graphs_dualDeclarationOrder },
    { "GraphsDualWrongFamily", &Arcadia_ADL_Tests_Graphs_dualWrongFamily },
    { "GraphsDualMissingInput", &Arcadia_ADL_Tests_Graphs_dualMissingInput },
    { "GraphsDualInputShape", &Arcadia_ADL_Tests_Graphs_dualInputShape },
    { "GraphsDualCycles", &Arcadia_ADL_Tests_Graphs_dualCycles },
    { "GraphsDualNestedMix", &Arcadia_ADL_Tests_Graphs_dualNestedMix },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
