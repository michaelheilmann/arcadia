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

#include "Arcadia.ADL.Tests.Definitions/Audials/SampleBuffer.h"

#include "Arcadia.ADL.Tests.Definitions/Visuals/Color.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Material.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Mesh.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Model.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/PixelBuffer.h"
#include "Arcadia.ADL.Tests.Definitions/Visuals/Texture.h"

#include "Arcadia.ADL.Tests.Definitions/DSP/Constant.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/SawtoothWave.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/SineWave.h"
#include "Arcadia.ADL.Tests.Definitions/DSP/WhiteNoise.h"

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Audials_SampleBufferDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ColorDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ColorDefinitionTest2)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_PixelBufferDefinitionTest3)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_TextureDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MaterialDefinitionTest3)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_MeshDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_Visuals_ModelDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_ConstantDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_SawtoothWaveDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_SineWaveDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_SineWaveDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&Arcadia_ADL_Tests_DSP_WhiteNoiseDefinitionTest1)) {
    return EXIT_FAILURE;
  }
  //
  return EXIT_SUCCESS;
}
