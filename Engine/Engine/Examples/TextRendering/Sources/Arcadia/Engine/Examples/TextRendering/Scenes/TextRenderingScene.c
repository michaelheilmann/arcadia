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

#include "Arcadia/Engine/Examples/TextRendering/Scenes/TextRenderingScene.h"

#include "Arcadia/Engine/Examples/TextRendering/AssetUtilities.h"
#include "Arcadia/Engine/Text/Include.h"

#include "Arcadia/Collections/Include.h"
#include "Arcadia/FontIO/Include.h"

// The text to render, and the runs (pixel size and color) to render it in.
// The markup parser converts this string into text-run descriptors.
static char const textRunsMarkup[] =
  "[rect=canvas align=center size=48 color=Colors.White]Hello, World!\n"
  "[rect=canvas align=center size=32 color=Colors.Yellow]Hello, World!\n"
  "[rect=canvas align=center size=24 color=Colors.Cyan]Hello, World!\n"
  "[rect=canvas align=center size=16 color=Colors.Magenta]Hello, World!\n"
  "[size=16 color=Colors.White fixed=true]Esc: quit | L/C/R or Left/Up/Right: align\n"
  "[size=16 color=Colors.White fixed=true]{fps}";

// The vertical distance, in pixels, between two adjacent lines.
#define Arcadia_Engine_Demo_TextRenderingScene_LINE_GAP (16)
// The distance, in pixels, of the fixed runs (e.g. the FPS counter) from the
// left and top edges of the canvas.
#define Arcadia_Engine_Demo_TextRenderingScene_MARGIN (16)
// The fraction of the canvas width inset from each side for normal text runs.
#define Arcadia_Engine_Demo_TextRenderingScene_TEXT_AREA_INSET_DENOMINATOR (8)
// The maximum number of glyphs rendered by the FPS counter run ("FPS: 9999").
#define Arcadia_Engine_Demo_TextRenderingScene_FPS_MAXIMUM_GLYPHS Arcadia_Engine_FPSCounter_MaximumTextLength

static void
Arcadia_Engine_Demo_TextRenderingScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingSceneDispatch* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static void
setGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_Integer32Value penX,
    Arcadia_Integer32Value baselineY,
    Arcadia_BooleanValue hasClipRectangle,
    Arcadia_Integer32Value clipLeft,
    Arcadia_Integer32Value clipBottom,
    Arcadia_Integer32Value clipWidth,
    Arcadia_Integer32Value clipHeight
  );

static void
copyGlyphPlacements
  (
    Arcadia_Engine_Text_GlyphPlacement* target,
    Arcadia_FontIO_AtlasGlyphInformation const* source,
    Arcadia_SizeValue numberOfGlyphs
  );

static Arcadia_SizeValue
getNumberOfRuns
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static Arcadia_Engine_Examples_TextRendering_TextRun*
getRunAt
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex
  );

static Arcadia_List*
parseTextRunsMarkup
  (
    Arcadia_Thread* thread
  );

static Arcadia_Engine_Text_MarkupTextRun*
getMarkupRunAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* markupRuns,
    Arcadia_SizeValue runIndex
  );

static Arcadia_BooleanValue
isFpsToken
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_MarkupTextRun* run
  );

static Arcadia_String*
getColorAssetPath
  (
    Arcadia_Thread* thread,
    Arcadia_String* colorName
  );

static void
layoutGlyphs
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
setTextAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Text_TextAlignment alignment
  );

static void
setFontTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_FontIO_AtlasBitmapFont* font,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

static char const*
findFontPath
  (
    Arcadia_Thread* thread
  );

static void
setRunText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_TextRenderingScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_TextRenderingScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_TextRenderingScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.TextRenderingScene", Arcadia_Engine_Demo_TextRenderingScene,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         &_Arcadia_Engine_Demo_TextRenderingScene_typeOperations);

static void
Arcadia_Engine_Demo_TextRenderingScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_TextRenderingScene);
  {
    Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 3),
                  sceneManager = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &engine);
    Arcadia_ValueStack_pushValue(thread, &sceneManager);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->definitions = NULL;
  //
  self->cameraNode = NULL;
  self->enterPassNode = NULL;
  self->viewportNode = NULL;
  //
  self->runs = NULL;
  self->fontCache = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_FontCache_getType(thread));
  //
  self->textCanvasWidth = -1;
  self->textCanvasHeight = -1;
  //
  Arcadia_Engine_FPSCounter_initialize(thread, &self->fpsCounter);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_TextRenderingScene);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  Arcadia_Engine_FPSCounter_uninitialize(thread, &self->fpsCounter);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingSceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Demo_TextRenderingScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }

  if (self->cameraNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  }
  if (self->enterPassNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  }
  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  }
  if (self->runs) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->runs);
  }
  if (self->fontCache) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fontCache);
  }
}

static Arcadia_SizeValue
getNumberOfRuns
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  return self->runs ? Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->runs) : 0;
}

static Arcadia_Engine_Examples_TextRendering_TextRun*
getRunAt
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex
  )
{
  return (Arcadia_Engine_Examples_TextRendering_TextRun*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->runs, runIndex, _Arcadia_Engine_Examples_TextRendering_TextRun_getType(thread));
}

static Arcadia_List*
parseTextRunsMarkup
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Engine_Text_MarkupParser* parser = Arcadia_Engine_Text_MarkupParser_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)parser);
  Arcadia_String* input = Arcadia_String_createFromCxxString(thread, textRunsMarkup);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser,
                                    (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, input)));
  Arcadia_Value result = Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser);
  Arcadia_List* markupRuns = (Arcadia_List*)Arcadia_Value_getObjectReferenceValueChecked(thread, result, _Arcadia_ArrayList_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)markupRuns);
  Arcadia_ValueStack_popValues(thread, 2);
  return markupRuns;
}

static Arcadia_Engine_Text_MarkupTextRun*
getMarkupRunAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* markupRuns,
    Arcadia_SizeValue runIndex
  )
{
  return (Arcadia_Engine_Text_MarkupTextRun*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, markupRuns, runIndex, _Arcadia_Engine_Text_MarkupTextRun_getType(thread));
}

static Arcadia_BooleanValue
isFpsToken
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_MarkupTextRun* run
  )
{
  return run->dynamicToken && Arcadia_String_isEqualTo_pn(thread, run->dynamicToken, "fps", sizeof("fps") - 1);
}

static Arcadia_String*
getColorAssetPath
  (
    Arcadia_Thread* thread,
    Arcadia_String* colorName
  )
{
  if (Arcadia_String_isEqualTo_pn(thread, colorName, "Colors.White", sizeof("Colors.White") - 1)) {
    return Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/White.adl");
  }
  if (Arcadia_String_isEqualTo_pn(thread, colorName, "Colors.Yellow", sizeof("Colors.Yellow") - 1)) {
    return Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Yellow.adl");
  }
  if (Arcadia_String_isEqualTo_pn(thread, colorName, "Colors.Cyan", sizeof("Colors.Cyan") - 1)) {
    return Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Cyan.adl");
  }
  if (Arcadia_String_isEqualTo_pn(thread, colorName, "Colors.Magenta", sizeof("Colors.Magenta") - 1)) {
    return Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Magenta.adl");
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

// Write the glyph quads of one run into the run's mesh vertex buffer.
// The penX is the x coordinate of the leftmost glyph edge, the baselineY the y
// coordinate of the baseline. Only positions, vertex colors, and texture
// coordinates are written; the mesh definition's initial data is overwritten.
static void
setGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_Integer32Value penX,
    Arcadia_Integer32Value baselineY,
    Arcadia_BooleanValue hasClipRectangle,
    Arcadia_Integer32Value clipLeft,
    Arcadia_Integer32Value clipBottom,
    Arcadia_Integer32Value clipWidth,
    Arcadia_Integer32Value clipHeight
  )
{
  Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, runIndex);
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, run->modelNode->mesh);
  Arcadia_Engine_Text_GlyphPlacement* glyphs = Arcadia_Memory_allocateUnmanaged(thread, run->numberOfGlyphs * sizeof(Arcadia_Engine_Text_GlyphPlacement));
  copyGlyphPlacements(glyphs, run->glyphs, run->numberOfGlyphs);
  Arcadia_Engine_Text_writeGlyphQuads(thread, glyphs, run->numberOfGlyphs, penX, baselineY,
                                      hasClipRectangle, clipLeft, clipBottom, clipWidth, clipHeight,
                                      (Arcadia_Real32Value)run->fontAtlasWidth, (Arcadia_Real32Value)run->fontAtlasHeight,
                                      run->red, run->green, run->blue, run->alpha,
                                      (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer),
                                      Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 9);
  Arcadia_Memory_deallocateUnmanaged(thread, glyphs);
  run->modelNode->mesh->dirtyFlags |= 1;
}

static void
copyGlyphPlacements
  (
    Arcadia_Engine_Text_GlyphPlacement* target,
    Arcadia_FontIO_AtlasGlyphInformation const* source,
    Arcadia_SizeValue numberOfGlyphs
  )
{
  for (Arcadia_SizeValue i = 0; i < numberOfGlyphs; ++i) {
    target[i].width = source[i].glyphInformation.width;
    target[i].height = source[i].glyphInformation.height;
    target[i].bearingX = source[i].glyphInformation.bearingX;
    target[i].bearingY = source[i].glyphInformation.bearingY;
    target[i].advanceX = source[i].glyphInformation.advanceX;
    target[i].atlasLeft = source[i].x;
    target[i].atlasTop = source[i].y;
  }
}

// Compute the relative to the baseline bounding box of a run's text:
// the top-most edge has the largest bearingY, the bottom-most edge the
// smallest (bearingY - height).
static void
computeTextExtents
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_TextRendering_TextRun const* run,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* bottom,
    Arcadia_Integer32Value* firstBearingX,
    Arcadia_Integer32Value* totalAdvanceX
  )
{
  Arcadia_Engine_Text_GlyphPlacement* glyphs = Arcadia_Memory_allocateUnmanaged(thread, run->numberOfGlyphs * sizeof(Arcadia_Engine_Text_GlyphPlacement));
  copyGlyphPlacements(glyphs, run->glyphs, run->numberOfGlyphs);
  Arcadia_Engine_Text_Extents extents;
  Arcadia_Engine_Text_computeExtents(thread, glyphs, run->numberOfGlyphs, &extents);
  Arcadia_Memory_deallocateUnmanaged(thread, glyphs);
  *top = extents.top;
  *bottom = extents.bottom;
  *firstBearingX = extents.firstBearingX;
  *totalAdvanceX = extents.totalAdvanceX;
}

// Center the stack of text runs horizontally and vertically on the canvas; runs
// with a fixed placement are positioned elsewhere (see `updateVisuals`) and are
// excluded from the stack.
// The canvas is a right-handed coordinate system with origin at the lower-left
// corner: x grows to the right, y grows upwards. The projection is a
// pixel-aligned orthographic projection (one world unit is one canvas pixel).
// All glyph edges are therefore aligned to integer canvas coordinates so that,
// with the Nearest-filtered atlas, every glyph texel maps exactly onto one
// canvas pixel.
static void
layoutGlyphs
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_SizeValue const numberOfRuns = getNumberOfRuns(thread, self);
  Arcadia_Engine_Text_LineLayoutElement* lines = Arcadia_Memory_allocateUnmanaged(thread, numberOfRuns * sizeof(Arcadia_Engine_Text_LineLayoutElement));
  for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
    Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, i);
    lines[i].active = !run->fixedPlacement;
    if (run->fixedPlacement) {
      continue;
    }
    Arcadia_Integer32Value firstBearingX, totalAdvanceX;
    computeTextExtents(thread, run, &lines[i].textTop, &lines[i].textBottom, &firstBearingX, &totalAdvanceX);
    lines[i].textWidth = firstBearingX + totalAdvanceX;
    lines[i].alignment = run->alignment;
    Arcadia_Integer32Value rectangleLeft = run->hasRectangle ? run->rectangleLeft : 0;
    Arcadia_Integer32Value rectangleBottom = run->hasRectangle ? run->rectangleBottom : 0;
    Arcadia_Integer32Value rectangleWidth = (run->hasRectangle && 0 < run->rectangleWidth) ? run->rectangleWidth : width;
    Arcadia_Integer32Value rectangleHeight = (run->hasRectangle && 0 < run->rectangleHeight) ? run->rectangleHeight : height;
    if (!run->hasRectangle && width > 2 * Arcadia_Engine_Demo_TextRenderingScene_MARGIN) {
      Arcadia_Integer32Value inset = width / Arcadia_Engine_Demo_TextRenderingScene_TEXT_AREA_INSET_DENOMINATOR;
      if (inset < Arcadia_Engine_Demo_TextRenderingScene_MARGIN) {
        inset = Arcadia_Engine_Demo_TextRenderingScene_MARGIN;
      }
      rectangleLeft = inset;
      rectangleWidth = width - 2 * inset;
    }
    lines[i].rectangleLeft = rectangleLeft;
    lines[i].rectangleBottom = rectangleBottom;
    lines[i].rectangleWidth = rectangleWidth;
    lines[i].rectangleHeight = rectangleHeight;
  }
  Arcadia_Engine_Text_layoutLineStack(thread, lines, numberOfRuns, height, Arcadia_Engine_Demo_TextRenderingScene_LINE_GAP);

  for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
    if (!lines[i].active) {
      continue;
    }
    setGlyphQuads(thread, self, i, lines[i].penX, lines[i].baselineY, Arcadia_BooleanValue_True,
                  lines[i].rectangleLeft, lines[i].rectangleBottom, lines[i].rectangleWidth, lines[i].rectangleHeight);
  }
  Arcadia_Memory_deallocateUnmanaged(thread, lines);
}

static void
setTextAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Text_TextAlignment alignment
  )
{
  if (!self->runs) {
    return;
  }
  Arcadia_SizeValue const numberOfRuns = getNumberOfRuns(thread, self);
  for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
    Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, i);
    if (!run->fixedPlacement) {
      run->alignment = alignment;
    }
  }
  self->textCanvasWidth = -1;
  self->textCanvasHeight = -1;
}

// Install the cached font atlas as the ambient color texture of one run's model,
// and store the glyph information of the glyph-source code points. The
// `codePoints`/`numberOfCodePoints` arguments name the glyph source; the run's
// active text may be any subsequence of them (see `setRunText`).
static void
setFontTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_FontIO_AtlasBitmapFont* font,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, runIndex);
  // Install the atlas as the ambient color texture of the run's model.
  Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
  Arcadia_Engine_Visuals_TextureNode* textureNode = run->modelNode->material->ambientColorTexture;
  textureNode->pixelBuffer->pixelBuffer = atlas;
  textureNode->width = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
  textureNode->height = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
  textureNode->dirtyBits = 0xff;
  run->fontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
  run->fontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
  // Fetch the glyph information of all code points of the glyph source, and
  // record the code points themselves so that the active text can be looked up
  // against this superset.
  Arcadia_Natural32Value* allCodePoints = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_Natural32Value));
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    allCodePoints[i] = codePoints[i];
  }
  Arcadia_FontIO_AtlasGlyphInformation* allGlyphInformation = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, codePoints[i], &allGlyphInformation[i]);
  }
  run->allCodePoints = allCodePoints;
  run->allGlyphInformation = allGlyphInformation;
  run->allCodePointCount = numberOfCodePoints;
  Arcadia_FontIO_AtlasGlyphInformation* glyphs = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
  run->glyphs = glyphs;
  run->maximumNumberOfGlyphs = numberOfCodePoints;
  run->numberOfGlyphs = 0;
}

static char const*
findFontPath
  (
    Arcadia_Thread* thread
  )
{
  static char const* fallbackFontPaths[] = {
    "C:/Windows/Fonts/arial.ttf",
    "C:/Windows/Fonts/segoeui.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
  };
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  for (Arcadia_SizeValue i = 0; i < sizeof(fallbackFontPaths) / sizeof(fallbackFontPaths[0]); ++i) {
    if (Arcadia_FileSystem_regularFileExists(thread, fileSystem,
                                             Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, fallbackFontPaths[i])))) {
      return fallbackFontPaths[i];
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
  return NULL;
}

// Set the active text of one run from an array of code points. Every code point
// must be an element of the run's glyph source (see `allCodePoints`); the glyph
// information is copied from the glyph source into the run's active glyph
// buffer. This makes it possible to change the rendered text of a run without
// touching the font atlas.
static void
setRunText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_SizeValue runIndex,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, runIndex);
  if (numberOfCodePoints > run->maximumNumberOfGlyphs) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    // Look up the code point in the run's glyph source.
    Arcadia_SizeValue found = run->allCodePointCount;
    for (Arcadia_SizeValue j = 0; j < run->allCodePointCount; ++j) {
      if (run->allCodePoints[j] == codePoints[i]) {
        found = j;
        break;
      }
    }
    if (found == run->allCodePointCount) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    run->glyphs[i] = run->allGlyphInformation[found];
  }
  run->numberOfGlyphs = numberOfCodePoints;
}

static Arcadia_SizeValue
countCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  Arcadia_SizeValue numberOfCodePoints = 0;
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    ++numberOfCodePoints;
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
  return numberOfCodePoints;
}

static void
writeCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_Natural32Value* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    codePoints[i] = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    if (i + 1 < numberOfCodePoints) {
      Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    }
  }
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/TextRenderingScene")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/Colors")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }

    self->definitions = definitions;
  }

  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;

  if (!self->enterPassNode) {
    self->enterPassNode =
      Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }

  if (!self->viewportNode) {
    Arcadia_ADL_ColorDefinition* CLEARCOLORS[] =
    {
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/DarkGray.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.DarkGray")),
    };

    Arcadia_ADL_ColorDefinition* d = CLEARCOLORS[0];
    self->viewportNode =
      (Arcadia_Engine_Visuals_ViewportNode*)
      Arcadia_Engine_Visuals_NodeFactory_createViewportNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
    Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode, Arcadia_Math_Color4Real32_create4(thread, d->red / 255.f, d->green / 255.f, d->blue / 255.f, 1.f));
    Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode, 0.f, 0.f, 1.f, 1.f);
  }

  if (!self->cameraNode) {
    self->cameraNode =
      (Arcadia_Engine_Visuals_CameraNode*)
      Arcadia_Engine_Visuals_NodeFactory_createCameraNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }

  if (!self->runs) {
    // (1) Create the list of runs.
    Arcadia_List* markupRuns = parseTextRunsMarkup(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)markupRuns);
    Arcadia_SizeValue const numberOfRuns = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)markupRuns);
    Arcadia_List* runs = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->runs = runs;
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      Arcadia_Engine_Examples_TextRendering_TextRun* run = Arcadia_Engine_Examples_TextRendering_TextRun_create(thread);
      run->fixedPlacement = markupRun->fixedPlacement;
      run->dynamicFps = isFpsToken(thread, markupRun);
      run->alignment = markupRun->alignment;
      run->hasRectangle = markupRun->hasRectangle;
      run->rectangleLeft = markupRun->rectangleLeft;
      run->rectangleBottom = markupRun->rectangleBottom;
      run->rectangleWidth = markupRun->rectangleWidth;
      run->rectangleHeight = markupRun->rectangleHeight;
      run->lineBreakBefore = markupRun->lineBreakBefore;
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)run);
      Arcadia_List_insertBackObjectReferenceValue(thread, runs, (Arcadia_Object*)run);
      Arcadia_ValueStack_popValues(thread, 1);
    }

    // (2) Parse the glyph source of every run into an array of code points.
    // For a fixed run (e.g. the FPS counter) the glyph source is a superset of
    // the code points of the text that is rendered.
    Arcadia_Natural32Value** runCodePoints = Arcadia_Memory_allocateUnmanaged(thread, numberOfRuns * sizeof(Arcadia_Natural32Value*));
    Arcadia_SizeValue* runCodePointCounts = Arcadia_Memory_allocateUnmanaged(thread, numberOfRuns * sizeof(Arcadia_SizeValue));
    Arcadia_SizeValue maximumNumberOfGlyphs = 0;
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      if (markupRun->dynamicToken && !isFpsToken(thread, markupRun)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_String* glyphSource = isFpsToken(thread, markupRun) ? Arcadia_String_createFromCxxString(thread, Arcadia_Engine_FPSCounter_getGlyphSource()) : markupRun->glyphSource;
      Arcadia_SizeValue const numberOfCodePoints = countCodePoints(thread, glyphSource);
      runCodePoints[i] = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_Natural32Value));
      writeCodePoints(thread, glyphSource, runCodePoints[i], numberOfCodePoints);
      runCodePointCounts[i] = numberOfCodePoints;
      // The number of code points a run may render. Fixed runs are capped at
      // the glyph source subset actually used.
      Arcadia_SizeValue const activeMaximum = isFpsToken(thread, markupRun) ? Arcadia_Engine_FPSCounter_MaximumTextLength : numberOfCodePoints;
      if (activeMaximum > maximumNumberOfGlyphs) {
        maximumNumberOfGlyphs = activeMaximum;
      }
    }

    // (2.1) Create one font per unique font path / pixel size pair and warm it
    // with the glyph sources of all runs that use it.
    char const* fontPath = findFontPath(thread);
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      Arcadia_FontIO_AtlasBitmapFont* font = Arcadia_Engine_FontCache_getOrCreate(thread, self->fontCache, fontPath, markupRun->pixelSize);
      for (Arcadia_SizeValue j = 0; j < runCodePointCounts[i]; ++j) {
        Arcadia_FontIO_AtlasGlyphInformation glyphInformation;
        Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, runCodePoints[i][j], &glyphInformation);
      }
    }

    // (3) Build the mesh definition.
    Arcadia_SizeValue const numberOfVertices = 6 * maximumNumberOfGlyphs;
    Arcadia_SizeValue const numberOfPositionComponents = 3 * numberOfVertices;
    Arcadia_SizeValue const numberOfColorComponents = 4 * numberOfVertices;
    Arcadia_SizeValue const numberOfTextureCoordinatesComponents = 2 * numberOfVertices;
    Arcadia_Real32Value* positions = Arcadia_Memory_allocateUnmanaged(thread, numberOfPositionComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Real32Value* colors = Arcadia_Memory_allocateUnmanaged(thread, numberOfColorComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Real32Value* textureCoordinates = Arcadia_Memory_allocateUnmanaged(thread, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value));
    for (Arcadia_SizeValue i = 0; i < numberOfPositionComponents; ++i) {
      positions[i] = 0.f;
    }
    for (Arcadia_SizeValue i = 0; i < numberOfColorComponents; ++i) {
      colors[i] = 1.f;
    }
    for (Arcadia_SizeValue i = 0; i < numberOfTextureCoordinatesComponents; ++i) {
      textureCoordinates[i] = 0.f;
    }
    Arcadia_RuntimeByteArray* positionArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)positions, numberOfPositionComponents * sizeof(Arcadia_Real32Value));
    Arcadia_RuntimeByteArray* colorArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)colors, numberOfColorComponents * sizeof(Arcadia_Real32Value));
    Arcadia_RuntimeByteArray* textureCoordinatesArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)textureCoordinates, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Memory_deallocateUnmanaged(thread, positions);
    Arcadia_Memory_deallocateUnmanaged(thread, colors);
    Arcadia_Memory_deallocateUnmanaged(thread, textureCoordinates);

    Arcadia_ADL_MeshDefinition* meshDefinition = Arcadia_ADL_MeshDefinition_create(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "TextRenderingScene.Mesh"),
                                                                                   numberOfVertices, positionArray, colorArray, textureCoordinatesArray,
                                                                                   Arcadia_String_createFromCxxString(thread, "Colors.White"));
    // (4) Register the mesh into the definitions.
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)meshDefinition);
    Arcadia_Map_set(thread, self->definitions->definitions,
                    Arcadia_Value_makeObjectReferenceValue(((Arcadia_ADL_Definition*)meshDefinition)->name),
                    Arcadia_Value_makeObjectReferenceValue(meshDefinition), NULL, NULL);
    Arcadia_ValueStack_popValues(thread, 1);

    // (5) Create the model nodes.
    Arcadia_ADL_ModelDefinition* MODELS[] =
    {
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/TextRenderingScene/TextureColorModel.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "TextRenderingScene.TextureColorModel")),
    };
    Arcadia_ADL_ModelDefinition* modelDefinition = MODELS[0];
    if (NULL == modelDefinition) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)modelDefinition);
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, i);
      run->modelNode =
        (Arcadia_Engine_Visuals_ModelNode*)
        Arcadia_Engine_Visuals_NodeFactory_createModelNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
          modelDefinition
        );

      // The pixel size and the color of the run.
      Arcadia_ADL_ColorDefinition* colorDefinition = getColorDefinition(thread, self->definitions,
                                                                        getColorAssetPath(thread, markupRun->colorName),
                                                                        markupRun->colorName);
      run->red = colorDefinition->red / 255.f;
      run->green = colorDefinition->green / 255.f;
      run->blue = colorDefinition->blue / 255.f;
      run->alpha = 1.f;
    }

    // (6) Install the font atlas and fetch the glyph information for every run.
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, i);
      run->fontAtlasWidth = markupRun->pixelSize;
      setFontTexture(thread, self, i, Arcadia_Engine_FontCache_getOrCreate(thread, self->fontCache, fontPath, markupRun->pixelSize), runCodePoints[i], runCodePointCounts[i]);
    }
    // (7) Set the active text of every static run. Dynamic runs (e.g. the FPS
    // counter) are set from `updateVisuals`.
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Text_MarkupTextRun* markupRun = getMarkupRunAt(thread, markupRuns, i);
      if (!markupRun->dynamicToken) {
        setRunText(thread, self, i, runCodePoints[i], runCodePointCounts[i]);
      }
    }
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Memory_deallocateUnmanaged(thread, runCodePoints[i]);
    }
    Arcadia_Memory_deallocateUnmanaged(thread, runCodePoints);
    Arcadia_Memory_deallocateUnmanaged(thread, runCodePointCounts);
    Arcadia_ValueStack_popValues(thread, 1);
  }
}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);

  // We use a right-handed coordinate system.
  // -negative z-axis forward, positive z-axis backward
  // -negative y-axis down, positive y-axis up
  // -negative x-axis left, positive x-axis right
  // Viewer located at (0,0,+1).
  // The projection is a pixel-aligned orthographic projection:
  // one world unit corresponds to one pixel of the canvas.
  if (NULL != self->runs &&
      0 < getNumberOfRuns(thread, self) &&
      0 != getRunAt(thread, self, 0)->fontAtlasWidth && 0 != getRunAt(thread, self, 0)->fontAtlasHeight &&
      (self->textCanvasWidth != width || self->textCanvasHeight != height)) {
    layoutGlyphs(thread, self, width, height);
    self->textCanvasWidth = width;
    self->textCanvasHeight = height;
  }

  Arcadia_Engine_FPSCounter_update(&self->fpsCounter, tick);
  // Render fixed runs. The FPS counter is dynamic and stays at the top-left;
  // static fixed runs are anchored at the bottom-right.
  if (NULL != self->runs) {
    Arcadia_SizeValue const numberOfRuns = getNumberOfRuns(thread, self);
    for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
      Arcadia_Engine_Examples_TextRendering_TextRun* run = getRunAt(thread, self, i);
      if (!run->fixedPlacement) {
        continue;
      }
      if (run->dynamicFps) {
        char fpsText[Arcadia_Engine_Demo_TextRenderingScene_FPS_MAXIMUM_GLYPHS + 1];
        Arcadia_SizeValue const numberOfCharacters = Arcadia_Engine_FPSCounter_format(&self->fpsCounter, fpsText);
        Arcadia_Natural32Value fpsCodePoints[Arcadia_Engine_Demo_TextRenderingScene_FPS_MAXIMUM_GLYPHS + 1];
        for (Arcadia_SizeValue j = 0; j < numberOfCharacters; ++j) {
          fpsCodePoints[j] = (Arcadia_Natural32Value)(Arcadia_Natural8Value)fpsText[j];
        }
        setRunText(thread, self, i, fpsCodePoints, numberOfCharacters);
      }
      Arcadia_Integer32Value top, bottom, firstBearingX, totalAdvanceX;
      computeTextExtents(thread, run, &top, &bottom, &firstBearingX, &totalAdvanceX);
      Arcadia_Integer32Value textWidth = firstBearingX + totalAdvanceX;
      Arcadia_Integer32Value penX = run->dynamicFps ? Arcadia_Engine_Demo_TextRenderingScene_MARGIN
                                                    : width - Arcadia_Engine_Demo_TextRenderingScene_MARGIN - textWidth;
      Arcadia_Integer32Value baselineY = run->dynamicFps ? (height - Arcadia_Engine_Demo_TextRenderingScene_MARGIN) - top
                                                         : Arcadia_Engine_Demo_TextRenderingScene_MARGIN - bottom;
      if (run->hasRectangle && 0 < run->rectangleWidth && 0 < run->rectangleHeight) {
        switch (run->alignment) {
          case Arcadia_Engine_Text_TextAlignment_Left:
            penX = run->rectangleLeft;
            break;
          case Arcadia_Engine_Text_TextAlignment_Right:
            penX = run->rectangleLeft + run->rectangleWidth - textWidth;
            break;
          case Arcadia_Engine_Text_TextAlignment_Center:
          default:
            penX = run->rectangleLeft + (run->rectangleWidth - textWidth) / 2;
            break;
        }
        baselineY = run->rectangleBottom + (run->rectangleHeight - (top - bottom)) / 2 - bottom;
      }
      setGlyphQuads(thread, self, i, penX, baselineY,
                    run->hasRectangle && 0 < run->rectangleWidth && 0 < run->rectangleHeight,
                    run->rectangleLeft, run->rectangleBottom, run->rectangleWidth, run->rectangleHeight);
    }
  }

  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, 0.f, (Arcadia_Real32Value)width, 0.f, (Arcadia_Real32Value)height, -1.f, 1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);

  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, NULL);
  Arcadia_Engine_Visuals_BackendContext* backendContext = (Arcadia_Engine_Visuals_BackendContext*)((Arcadia_Engine_Scene*)self)->engine->visualsBackendContext;
  Arcadia_SizeValue const numberOfRuns = getNumberOfRuns(thread, self);
  Arcadia_Engine_Visuals_ModelNode** modelNodes = Arcadia_Memory_allocateUnmanaged(thread, (0 < numberOfRuns) ? numberOfRuns * sizeof(Arcadia_Engine_Visuals_ModelNode*) : sizeof(Arcadia_Engine_Visuals_ModelNode*));
  for (Arcadia_SizeValue i = 0; i < numberOfRuns; ++i) {
    modelNodes[i] = getRunAt(thread, self, i)->modelNode;
  }
  Arcadia_Engine_Visuals_renderSceneWithModelNodes(thread, self->enterPassNode, modelNodes, numberOfRuns, backendContext);
  Arcadia_Memory_deallocateUnmanaged(thread, modelNodes);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) != Arcadia_Engine_Input_KeyboardKeyAction_Released) {
    return;
  }
  Arcadia_Engine_Input_KeyboardKey const key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  if (key == Arcadia_Engine_Input_KeyboardKey_Escape) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* e = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread,  (Arcadia_Object*)e);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_L || key == Arcadia_Engine_Input_KeyboardKey_LeftArrow) {
    setTextAlignment(thread, self, Arcadia_Engine_Text_TextAlignment_Left);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_C || key == Arcadia_Engine_Input_KeyboardKey_UpArrow) {
    setTextAlignment(thread, self, Arcadia_Engine_Text_TextAlignment_Center);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_R || key == Arcadia_Engine_Input_KeyboardKey_RightArrow) {
    setTextAlignment(thread, self, Arcadia_Engine_Text_TextAlignment_Right);
  }
}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Demo_TextRenderingScene*
Arcadia_Engine_Demo_TextRenderingScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager,
    Arcadia_Engine_FontCache* fontCache
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_TextRenderingScene);
  if (engine) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (sceneManager) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sceneManager);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (fontCache) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fontCache);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Engine_Demo_TextRenderingScene);
}

#undef Arcadia_Engine_Demo_TextRenderingScene_LINE_GAP
#undef Arcadia_Engine_Demo_TextRenderingScene_MARGIN
#undef Arcadia_Engine_Demo_TextRenderingScene_TEXT_AREA_INSET_DENOMINATOR
#undef Arcadia_Engine_Demo_TextRenderingScene_FPS_MAXIMUM_GLYPHS
