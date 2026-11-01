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

#include "Arcadia/VPL/Include.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define GL_VERTEX_SHADER 0x8b31
#define GL_FRAGMENT_SHADER 0x8b30
#define GL_COMPILE_STATUS 0x8b81
#define GL_LINK_STATUS 0x8b82
#define GL_ARRAY_BUFFER 0x8892
#define GL_UNIFORM_BUFFER 0x8a11
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8a40
#define GL_STATIC_DRAW 0x88e4
#define GL_FRAMEBUFFER 0x8d40
#define GL_COLOR_ATTACHMENT0 0x8ce0
#define GL_FRAMEBUFFER_COMPLETE 0x8cd5
#define GL_RGBA8 0x8058
#define GL_TEXTURE0 0x84c0

#define FUNCTION(Return, Name, Arguments) typedef Return (APIENTRY *P_##Name) Arguments; static P_##Name p_##Name;
FUNCTION(GLuint, glCreateShader, (GLenum))
FUNCTION(void, glShaderSource, (GLuint, GLsizei, const char* const*, const GLint*))
FUNCTION(void, glCompileShader, (GLuint))
FUNCTION(void, glGetShaderiv, (GLuint, GLenum, GLint*))
FUNCTION(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei*, char*))
FUNCTION(void, glDeleteShader, (GLuint))
FUNCTION(GLuint, glCreateProgram, (void))
FUNCTION(void, glAttachShader, (GLuint, GLuint))
FUNCTION(void, glLinkProgram, (GLuint))
FUNCTION(void, glGetProgramiv, (GLuint, GLenum, GLint*))
FUNCTION(void, glGetProgramInfoLog, (GLuint, GLsizei, GLsizei*, char*))
FUNCTION(void, glDeleteProgram, (GLuint))
FUNCTION(void, glUseProgram, (GLuint))
FUNCTION(GLuint, glGetUniformBlockIndex, (GLuint, const char*))
FUNCTION(void, glGetActiveUniformBlockiv, (GLuint, GLuint, GLenum, GLint*))
FUNCTION(GLint, glGetUniformLocation, (GLuint, const char*))
FUNCTION(GLint, glGetFragDataLocation, (GLuint, const char*))
FUNCTION(GLint, glGetAttribLocation, (GLuint, const char*))
FUNCTION(void, glUniform1i, (GLint, GLint))
FUNCTION(void, glGenBuffers, (GLsizei, GLuint*))
FUNCTION(void, glBindBuffer, (GLenum, GLuint))
FUNCTION(void, glBufferData, (GLenum, ptrdiff_t, const void*, GLenum))
FUNCTION(void, glBindBufferBase, (GLenum, GLuint, GLuint))
FUNCTION(void, glDeleteBuffers, (GLsizei, const GLuint*))
FUNCTION(void, glGenVertexArrays, (GLsizei, GLuint*))
FUNCTION(void, glBindVertexArray, (GLuint))
FUNCTION(void, glDeleteVertexArrays, (GLsizei, const GLuint*))
FUNCTION(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void*))
FUNCTION(void, glEnableVertexAttribArray, (GLuint))
FUNCTION(void, glGenFramebuffers, (GLsizei, GLuint*))
FUNCTION(void, glBindFramebuffer, (GLenum, GLuint))
FUNCTION(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint))
FUNCTION(GLenum, glCheckFramebufferStatus, (GLenum))
FUNCTION(void, glDeleteFramebuffers, (GLsizei, const GLuint*))
FUNCTION(void, glActiveTexture, (GLenum))
#undef FUNCTION

static HWND window;
static HDC dc;
static HGLRC context;

static PROC
address
  (
    const char* name
  )
{
  PROC value = wglGetProcAddress(name);
  return value == (PROC)1 || value == (PROC)2 || value == (PROC)3 || value == (PROC)-1 ? NULL : value;
}

static void
cleanup
  (
    void
  )
{
  wglMakeCurrent(NULL, NULL);
  if (context) wglDeleteContext(context);
  if (dc && window) ReleaseDC(window, dc);
  if (window) DestroyWindow(window);
  context = NULL; dc = NULL; window = NULL;
}

static int
setup
  (
    void
  )
{
  WNDCLASSA wc;
  memset(&wc, 0, sizeof(wc));
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = DefWindowProcA;
  wc.hInstance = GetModuleHandleA(NULL);
  wc.lpszClassName = "ArcadiaVPLShaderTest";
  if (!RegisterClassA(&wc)) return 0;
  window = CreateWindowA(wc.lpszClassName, "", WS_OVERLAPPEDWINDOW, 0, 0, 16, 16, NULL, NULL, wc.hInstance, NULL);
  if (!window) return 0;
  dc = GetDC(window);
  PIXELFORMATDESCRIPTOR format;
  memset(&format, 0, sizeof(format));
  format.nSize = sizeof(format);
  format.nVersion = 1;
  format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  format.iPixelType = PFD_TYPE_RGBA;
  format.cColorBits = 32;
  int selected = ChoosePixelFormat(dc, &format);
  if (!selected || !SetPixelFormat(dc, selected, &format)) return 0;
  context = wglCreateContext(dc);
  if (!context || !wglMakeCurrent(dc, context)) return 0;
  typedef HGLRC (WINAPI *CreateContext)(HDC, HGLRC, const int*);
  CreateContext create = (CreateContext)address("wglCreateContextAttribsARB");
  const int attributes[] = { 0x2091, 4, 0x2092, 2, 0x9126, 1, 0 };
  HGLRC modern = create ? create(dc, NULL, attributes) : NULL;
  if (!modern) return 0;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(context);
  context = modern;
  if (!wglMakeCurrent(dc, context)) return 0;
#define LOAD(Name) p_##Name = (P_##Name)address(#Name); if (!p_##Name) return 0;
  LOAD(glCreateShader) LOAD(glShaderSource) LOAD(glCompileShader) LOAD(glGetShaderiv) LOAD(glGetShaderInfoLog) LOAD(glDeleteShader)
  LOAD(glCreateProgram) LOAD(glAttachShader) LOAD(glLinkProgram) LOAD(glGetProgramiv) LOAD(glGetProgramInfoLog) LOAD(glDeleteProgram)
  LOAD(glUseProgram) LOAD(glGetUniformBlockIndex) LOAD(glGetActiveUniformBlockiv) LOAD(glGetUniformLocation) LOAD(glGetFragDataLocation) LOAD(glGetAttribLocation) LOAD(glUniform1i)
  LOAD(glGenBuffers) LOAD(glBindBuffer) LOAD(glBufferData) LOAD(glBindBufferBase) LOAD(glDeleteBuffers)
  LOAD(glGenVertexArrays) LOAD(glBindVertexArray) LOAD(glDeleteVertexArrays) LOAD(glVertexAttribPointer) LOAD(glEnableVertexAttribArray)
  LOAD(glGenFramebuffers) LOAD(glBindFramebuffer) LOAD(glFramebufferTexture2D) LOAD(glCheckFramebufferStatus) LOAD(glDeleteFramebuffers) LOAD(glActiveTexture)
#undef LOAD
  printf("OpenGL renderer: %s; version: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
  return 1;
}

static GLuint
shader
  (
    Arcadia_Thread* thread,
    GLenum kind,
    Arcadia_String* code
  )
{
  GLuint id = p_glCreateShader(kind);
  const char* bytes = (const char*)Arcadia_String_getBytes(thread, code);
  GLint count = (GLint)Arcadia_String_getNumberOfBytes(thread, code);
  p_glShaderSource(id, 1, &bytes, &count);
  p_glCompileShader(id);
  GLint success = 0;
  p_glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[8192];
    p_glGetShaderInfoLog(id, sizeof(log), NULL, log);
    fprintf(stderr, "Shader compilation failed:\n%s\n%.*s\n", log, count, bytes);
  }
  Arcadia_Tests_assertTrue(thread, success);
  return id;
}

static void
render
  (
    Arcadia_Thread* thread,
    GLuint program,
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact,
    const unsigned char expected[4],
    int textureCase
  )
{
  float vertices[] = {
    -1, -1, 0, .5f, .25f, 1, 1, .5f, .5f,
     3, -1, 0, .5f, .25f, 1, 1, .5f, .5f,
    -1,  3, 0, .5f, .25f, 1, 1, .5f, .5f
  };
  if (textureCase >= 3) {
    vertices[7] = 0; vertices[8] = 0;
    vertices[16] = 16; vertices[17] = 0;
    vertices[25] = 0; vertices[26] = 16;
  }
  float viewer[32] = { 0 }, model[16] = { 0 }, mesh[] = { .25f, .5f, .75f, 1 };
  for (int i = 0; i < 4; ++i) { viewer[5 * i] = 1; viewer[16 + 5 * i] = 1; model[5 * i] = 1; }
  GLuint buffers[4], vao, framebuffer, textures[2];
  p_glGenBuffers(4, buffers);
  p_glGenVertexArrays(1, &vao);
  p_glBindVertexArray(vao);
  p_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  p_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  p_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)0);
  p_glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(3 * sizeof(float)));
  p_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(7 * sizeof(float)));
  for (int i = 0; i < 3; ++i) p_glEnableVertexAttribArray(i);
  const void* constants[] = { viewer, model, mesh };
  const ptrdiff_t sizes[] = { sizeof(viewer), sizeof(model), sizeof(mesh) };
  Arcadia_SizeValue uniformCount = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)artifact->uniforms);
  for (int i = 0; i < 3 && uniformCount >= 4; ++i) {
    p_glBindBuffer(GL_UNIFORM_BUFFER, buffers[i + 1]);
    p_glBufferData(GL_UNIFORM_BUFFER, sizes[i], constants[i], GL_STATIC_DRAW);
    p_glBindBufferBase(GL_UNIFORM_BUFFER, i, buffers[i + 1]);
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, artifact->uniforms, i);
    GLuint index = p_glGetUniformBlockIndex(program, (const char*)Arcadia_String_getBytes(thread, binding->glName));
    if (index != (GLuint)-1) {
      GLint size = 0;
      p_glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
      Arcadia_Tests_assertTrue(thread, size == sizes[i]);
    }
  }
  glGenTextures(2, textures);
  p_glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  const unsigned char pixel[] = { 128, 64, 192, 255 };
  const unsigned char pattern[] = { 255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255 };
  const unsigned char red[] = { 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255 };
  const unsigned char green[] = { 0, 255, 0, 255 };
  if (!textureCase) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
  else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureCase >= 3 ? red : pattern);
  if (textureCase >= 3) {
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, green);
    glTexParameteri(GL_TEXTURE_2D, 0x813d /* GL_TEXTURE_MAX_LEVEL */, 1);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureCase >= 3 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureCase == 2 ? 0x812f /* GL_CLAMP_TO_EDGE */ : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureCase == 2 ? 0x812f : GL_REPEAT);
  p_glUseProgram(program);
  for (Arcadia_SizeValue i = 0; i < uniformCount; ++i) {
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, artifact->uniforms, i);
    if (binding->type->kind != Arcadia_VPL_IR_TypeKind_Sampler) continue;
    GLint location = p_glGetUniformLocation(program, (const char*)Arcadia_String_getBytes(thread, binding->glName));
    if (location >= 0) p_glUniform1i(location, 0);
  }
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  p_glGenFramebuffers(1, &framebuffer);
  p_glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  p_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);
  Arcadia_Tests_assertTrue(thread, p_glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glViewport(0, 0, 8, 8);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  unsigned char actual[4];
  glReadPixels(4, 4, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, actual);
  for (int i = 0; i < 4; ++i) {
    int target = expected[i];
    int delta = (int)actual[i] - target;
    if (delta < -2 || delta > 2) fprintf(stderr, "textureCase=%d channel %d actual=%u expected=%d\n", textureCase, i, actual[i], target);
    Arcadia_Tests_assertTrue(thread, delta >= -2 && delta <= 2);
  }
  Arcadia_Tests_assertTrue(thread, glGetError() == GL_NO_ERROR);
  p_glBindFramebuffer(GL_FRAMEBUFFER, 0);
  p_glDeleteFramebuffers(1, &framebuffer);
  glDeleteTextures(2, textures);
  p_glDeleteBuffers(4, buffers);
  p_glDeleteVertexArrays(1, &vao);
}

static Arcadia_Object*
root
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  return object;
}

static Arcadia_VPL_VerifiedModule*
loopProgram
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* original,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  Arcadia_VPL_IR_Module* module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, original, diagnostics));
  Arcadia_VPL_IR_Function* function = module->fragment->entryFunction;
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, function->blocks, 0);
  Arcadia_VPL_IR_Type* integer = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Integer));
  Arcadia_VPL_IR_Type* boolean = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Boolean));
  Arcadia_VPL_IR_Type* real = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating));
  Arcadia_VPL_IR_RegisterValue* counter = (Arcadia_VPL_IR_RegisterValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, 4, integer));
  Arcadia_VPL_IR_RegisterValue* condition = (Arcadia_VPL_IR_RegisterValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, 5, boolean));
  Arcadia_List_insertFront(thread, entry->instructions, Arcadia_Value_makeObjectReferenceValue(root(thread, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, counter, integer))));
  Arcadia_List_insertFront(thread, entry->instructions, Arcadia_Value_makeObjectReferenceValue(root(thread, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, condition, boolean))));
  Arcadia_VPL_IR_Value* zero = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, integer, Arcadia_Value_makeInteger32Value(0)));
  Arcadia_VPL_IR_Value* one = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, integer, Arcadia_Value_makeInteger32Value(1)));
  Arcadia_VPL_IR_Value* two = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, integer, Arcadia_Value_makeInteger32Value(2)));
  Arcadia_VPL_IR_Value* half = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, real, Arcadia_Value_makeReal32Value(.5f)));
  Arcadia_VPL_IR_Block* test = (Arcadia_VPL_IR_Block*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Block_create(thread, (Arcadia_List*)Arcadia_ArrayList_create(thread), NULL));
  Arcadia_VPL_IR_Block* body = (Arcadia_VPL_IR_Block*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Block_create(thread, (Arcadia_List*)Arcadia_ArrayList_create(thread), NULL));
  Arcadia_VPL_IR_Block* exit = (Arcadia_VPL_IR_Block*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Block_create(thread, (Arcadia_List*)Arcadia_ArrayList_create(thread), NULL));
  Arcadia_List_insertBackObjectReferenceValue(thread, function->blocks, (Arcadia_Object*)test);
  Arcadia_List_insertBackObjectReferenceValue(thread, function->blocks, (Arcadia_Object*)body);
  Arcadia_List_insertBackObjectReferenceValue(thread, function->blocks, (Arcadia_Object*)exit);
  Arcadia_List_insertBackObjectReferenceValue(thread, entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_CopyInstruction_create(thread, counter, zero));
  entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, test);
  Arcadia_List_insertBackObjectReferenceValue(thread, test->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, condition, Arcadia_VPL_IR_BinaryOperator_Less, (Arcadia_VPL_IR_Value*)counter, two));
  test->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ConditionalBranchTerminator_create(thread, (Arcadia_VPL_IR_Value*)condition, body, exit);
  Arcadia_VPL_IR_StageIO* output = (Arcadia_VPL_IR_StageIO*)Arcadia_List_getObjectReferenceValueAt(thread, module->fragment->interface, 2);
  Arcadia_VPL_IR_RegisterValue* color = (Arcadia_VPL_IR_RegisterValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, 0, output->type));
  Arcadia_VPL_IR_ReferenceValue* ref = (Arcadia_VPL_IR_ReferenceValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ReferenceValue_createWithStorage(thread, Arcadia_VPL_IR_StorageKind_StageIO, 2, output->type));
  Arcadia_List_insertBackObjectReferenceValue(thread, body->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, color, ref));
  Arcadia_List_insertBackObjectReferenceValue(thread, body->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, color, Arcadia_VPL_IR_BinaryOperator_Multiply, (Arcadia_VPL_IR_Value*)color, half));
  Arcadia_List_insertBackObjectReferenceValue(thread, body->instructions, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, ref, (Arcadia_VPL_IR_Value*)color));
  Arcadia_List_insertBackObjectReferenceValue(thread, body->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, counter, Arcadia_VPL_IR_BinaryOperator_Add, (Arcadia_VPL_IR_Value*)counter, one));
  body->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, test);
  exit->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  return Arcadia_VPL_VerifiedModule_create(thread, module, diagnostics);
}

static void
testPrograms
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 8; ++test) {
    int variant = test % 4;
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_VPL_Context* diagnostics = Arcadia_VPL_Context_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
    Arcadia_VPL_VerifiedModule* module = Arcadia_VPL_Programs_create(thread, (Arcadia_Natural8Value)variant);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)module);
    if (test >= 4) module = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)loopProgram(thread, module, Arcadia_VPL_Context_getDiagnostics(thread, diagnostics)));
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact = Arcadia_VPL_Backends_GLSL_compile(thread, module, Arcadia_VPL_Context_getDiagnostics(thread, diagnostics), 420);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)artifact);
    GLuint vertex = shader(thread, GL_VERTEX_SHADER, artifact->vertexCode);
    GLuint fragment = shader(thread, GL_FRAGMENT_SHADER, artifact->fragmentCode);
    GLuint program = p_glCreateProgram();
    p_glAttachShader(program, vertex); p_glAttachShader(program, fragment); p_glLinkProgram(program);
    GLint success = 0;
    p_glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) { char log[8192]; p_glGetProgramInfoLog(program, sizeof(log), NULL, log); fprintf(stderr, "Program link failed: %s\n", log); }
    Arcadia_Tests_assertTrue(thread, success);
    Arcadia_Tests_assertTrue(thread, p_glGetFragDataLocation(program, (const char*)Arcadia_String_getBytes(thread, artifact->fragmentColorOutput)) == artifact->fragmentColorLocation);
    Arcadia_VPL_Backends_GLSL_Binding* position = (Arcadia_VPL_Backends_GLSL_Binding*)Arcadia_List_getObjectReferenceValueAt(thread, artifact->vertexInterface, 0);
    Arcadia_Tests_assertTrue(thread, p_glGetAttribLocation(program, (const char*)Arcadia_String_getBytes(thread, position->glName)) == 0);
    const unsigned char reference[][4] = { { 64, 128, 191, 255 }, { 128, 64, 255, 255 }, { 128, 64, 192, 255 }, { 64, 16, 192, 255 } };
    unsigned char expected[4];
    for (int i = 0; i < 4; ++i) expected[i] = test >= 4 ? (reference[variant][i] + 2) / 4 : reference[variant][i];
    render(thread, program, artifact, expected, 0);
    p_glDeleteProgram(program); p_glDeleteShader(vertex); p_glDeleteShader(fragment);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testAggregateControlFlow
  (
    Arcadia_Thread* thread
  )
{
  static const char source[] =
    "(module 1 (types (type Z void) (type I int) (type B bool) (type V vector float 4)"
    " (type V3 vector float 3) (type F float) (type W array I 2)"
    " (type R record \"Packet\" (field \"color\" V) (field \"weights\" W)) (type A array R 2))"
    " (uniforms (array packets \"packets\" R 2) (record block \"block\" R))"
    " (vertex entry (interface (in pos \"position\" V3 0 none)"
    " (out position \"clip_position\" V -1 position) (out data \"data\" R 0 none))"
    " (functions (function entry \"entry\" Z (parameters)"
    " (registers (register r0 V3) (register r1 V) (register r2 W) (register r3 R))"
    " (block first (load r0 (stage pos)) (construct r1 r0 (float 1))"
    " (construct r2 (int 1) (int 2)) (construct r3 r1 r2)"
    " (store (stage position) r1) (store (stage data) r3) (return)))))"
    " (fragment entry (interface (in data \"data\" R 0 none) (out color \"color\" V 0 fragment_color))"
    " (functions (function entry \"entry\" Z (parameters)"
    " (registers (register r0 R) (register r1 W) (register r2 I) (register r3 V)"
    " (register r4 A) (register r5 B) (register r6 V3))"
    " (block first (load r0 (uniform block)) (load r4 (uniform packets))"
    " (load r0 (stage data)) (member r1 r0 \"weights\") (index r2 r1 (int 0))"
    " (member r3 r0 \"color\") (swizzle r6 r3 \"zyx\") (construct r3 r6 (float 1))"
    " (store (member_ref (index_ref (local r4) (int 0)) \"color\") r3)"
    " (load r0 (index_ref (local r4) (int 0))) (call r2 helper r2)"
    " (equal r5 r2 (int 0)) (branch_if r5 discard_block done))"
    " (block discard_block (call r2 kill) (copy r2 r2) (branch done))"
    " (block done (store (stage color) r3) (return)))"
    " (function helper \"helper\" I (parameters (parameter value I)) (registers (register r0 I))"
    " (block first (load r0 (parameter value)) (return r0)))"
    " (function kill \"kill\" I (parameters) (registers) (block first (discard))))))";
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_VPL_IR_Module* module = NULL;
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_read(thread, source, sizeof(source) - 1, diagnostics, NULL));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Languages_Diagnostics_emit(thread, diagnostics);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_VPL_VerifiedModule* published = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, diagnostics));
  Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact = (Arcadia_VPL_Backends_GLSL_ProgramArtifact*)root(thread, (Arcadia_Object*)Arcadia_VPL_Backends_GLSL_compile(thread, published, diagnostics, 420));
  GLuint vertex = shader(thread, GL_VERTEX_SHADER, artifact->vertexCode);
  GLuint fragment = shader(thread, GL_FRAGMENT_SHADER, artifact->fragmentCode);
  GLuint program = p_glCreateProgram();
  p_glAttachShader(program, vertex); p_glAttachShader(program, fragment); p_glLinkProgram(program);
  GLint success = 0;
  p_glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) { char log[8192]; p_glGetProgramInfoLog(program, sizeof(log), NULL, log); fprintf(stderr, "Aggregate link failed: %s\n", log); }
  Arcadia_Tests_assertTrue(thread, success);
  p_glDeleteProgram(program); p_glDeleteShader(vertex); p_glDeleteShader(fragment);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
renderSource
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue size,
    const unsigned char expected[4],
    int textureCase
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_VPL_IR_Module* module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_Text_read(thread, bytes, size, diagnostics, NULL));
  Arcadia_VPL_VerifiedModule* snapshot = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_create(thread, module, diagnostics));
  Arcadia_VPL_Backends_GLSL_ProgramArtifact* artifact = (Arcadia_VPL_Backends_GLSL_ProgramArtifact*)root(thread, (Arcadia_Object*)Arcadia_VPL_Backends_GLSL_compile(thread, snapshot, diagnostics, 420));
  GLuint vertex = shader(thread, GL_VERTEX_SHADER, artifact->vertexCode);
  GLuint fragment = shader(thread, GL_FRAGMENT_SHADER, artifact->fragmentCode);
  GLuint program = p_glCreateProgram();
  p_glAttachShader(program, vertex); p_glAttachShader(program, fragment); p_glLinkProgram(program);
  GLint success = 0;
  p_glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) { char log[8192]; p_glGetProgramInfoLog(program, sizeof(log), NULL, log); fprintf(stderr, "Conformance link failed: %s\n", log); }
  Arcadia_Tests_assertTrue(thread, success);
  render(thread, program, artifact, expected, textureCase);
  p_glDeleteProgram(program); p_glDeleteShader(vertex); p_glDeleteShader(fragment);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testNativeNumerics
  (
    Arcadia_Thread* thread
  )
{
  static const char source[] =
    "(module 1 (types (type Z void) (type I int) (type B bool) (type F float)"
    " (type V3 vector float 3) (type V4 vector float 4)) (uniforms)"
    " (vertex main (interface (in attr \"attribute\" V3 0 none) (out pos \"position\" V4 -1 position))"
    " (functions (function main \"vertex\" Z (parameters) (registers (register r0 V3) (register r1 V4))"
    " (block entry (load r0 (stage attr)) (construct r1 r0 (float 1)) (store (stage pos) r1) (return)))))"
    " (fragment main (interface (out col \"color\" V4 0 fragment_color))"
    " (functions (function main \"fragment\" Z (parameters)"
    " (registers (register r0 V4) (register r1 I) (register r2 B) (register r3 F))"
    " (block entry (copy r1 (int 2147483647)) (add r1 r1 (int 1))"
    " (equal r2 r1 (int -2147483648)) (branch_if r2 subtraction fail))"
    " (block subtraction (subtract r1 r1 (int 1)) (equal r2 r1 (int 2147483647))"
    " (branch_if r2 multiplication fail))"
    " (block multiplication (copy r1 (int 1073741824)) (multiply r1 r1 (int 4))"
    " (equal r2 r1 (int 0)) (branch_if r2 division fail))"
    " (block division (divide r1 (int -7) (int 2)) (equal r2 r1 (int -3))"
    " (branch_if r2 conversion fail))"
    " (block conversion (copy r1 (int -5)) (construct r3 r1) (equal r2 r3 (float -5))"
    " (branch_if r2 zeros fail))"
    " (block zeros (equal r2 (float -0.0) (float 0.0)) (branch_if r2 finite fail))"
    " (block finite (add r3 (float 0.5) (float 0.25)) (equal r2 r3 (float 0.75))"
    " (branch_if r2 pass fail))"
    " (block pass (construct r0 (float 0) (float 1) (float 0) (float 1)) (branch done))"
    " (block fail (construct r0 (float 1) (float 0) (float 0) (float 1)) (branch done))"
    " (block done (store (stage col) r0) (return))))))";
  const unsigned char expected[] = { 0, 255, 0, 255 };
  renderSource(thread, source, sizeof(source) - 1, expected, 0);
}

static void
testSamplingContract
  (
    Arcadia_Thread* thread
  )
{
  static const char prefix[] =
    "(module 1 (types (type Z void) (type V3 vector float 3) (type V4 vector float 4)"
    " (type V2 vector float 2) (type S sampler2D)) (uniforms (scalar tex \"texture\" S))"
    " (vertex main (interface (in attr \"attribute\" V3 0 none) (in uv \"attribute_uv\" V2 2 none)"
    " (out pos \"position\" V4 -1 position) (out vary \"vary\" V2 0 texcoord) (out sampled \"sampled\" V4 1 color))"
    " (functions (function main \"vertex\" Z (parameters)"
    " (registers (register r0 V3) (register r1 V4) (register r2 S) (register r3 V2) (register r4 V4))"
    " (block entry (load r0 (stage attr)) (construct r1 r0 (float 1)) (store (stage pos) r1)"
    " (load r3 (stage uv)) (store (stage vary) r3) (load r2 (uniform tex))"
    " (texture r4 r2 r3) (store (stage sampled) r4) (return)))))"
    " (fragment main (interface (in uv \"vary\" V2 0 texcoord) (in sampled \"sampled\" V4 1 color)"
    " (out col \"color\" V4 0 fragment_color)) (functions (function main \"fragment\" Z"
    " (parameters) (registers (register r0 V4) (register r1 V2) (register r2 S)) (block entry";
  const char* operations[] = {
    " (construct r1 (float 1.25) (float .25)) (load r2 (uniform tex)) (texture r0 r2 r1)",
    " (construct r1 (float 1.25) (float .25)) (load r2 (uniform tex)) (texture r0 r2 r1)",
    " (load r1 (stage uv)) (load r2 (uniform tex)) (texture r0 r2 r1)",
    " (load r0 (stage sampled))"
  };
  const unsigned char expected[][4] = { { 255, 0, 0, 255 }, { 0, 255, 0, 255 }, { 0, 255, 0, 255 }, { 255, 0, 0, 255 } };
  for (int i = 0; i < 4; ++i) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_StringBuilder* source = (Arcadia_StringBuilder*)root(thread, (Arcadia_Object*)Arcadia_StringBuilder_create(thread));
    const char* parts[] = { prefix, operations[i], " (store (stage col) r0) (return))))))" };
    for (int j = 0; j < 3; ++j) {
      Arcadia_String* text = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, parts[j]));
      Arcadia_StringBuilder_insertBack(thread, source, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text));
    }
    renderSource(thread, Arcadia_StringBuilder_getBytes(thread, source), Arcadia_StringBuilder_getNumberOfBytes(thread, source), expected[i], i + 1);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testHandwrittenExamples
  (
    Arcadia_Thread* thread
  )
{
  const char* files[] = { VPL_TEXT_SPEC_DIRECTORY "/control-flow.vpl", VPL_TEXT_SPEC_DIRECTORY "/aggregate-storage.vpl" };
  const unsigned char expected[] = { 0, 255, 0, 255 };
  for (int i = 0; i < 2; ++i) {
    char bytes[8192];
    FILE* file = fopen(files[i], "rb");
    Arcadia_Tests_assertTrue(thread, file != NULL);
    size_t n = fread(bytes, 1, sizeof(bytes), file);
    int failed = ferror(file), extra = fgetc(file);
    fclose(file);
    Arcadia_Tests_assertTrue(thread, !failed && extra == EOF);
    renderSource(thread, bytes, n, expected, 0);
  }
}

static int
runPrograms
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testPrograms) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAggregateControlFlow
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAggregateControlFlow) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNativeNumerics
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNativeNumerics) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSamplingContract
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testSamplingContract) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runHandwrittenExamples
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testHandwrittenExamples) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Programs", &runPrograms },
    { "AggregateControlFlow", &runAggregateControlFlow },
    { "NativeNumerics", &runNativeNumerics },
    { "SamplingContract", &runSamplingContract },
    { "HandwrittenExamples", &runHandwrittenExamples },
  };
  if (argc == 2 && !strcmp(argv[1], "--list")) {
    return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
  }
  if (!setup()) { cleanup(); puts("No OpenGL 4.2 core context available"); return 77; }
  int result = Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
  cleanup();
  return result;
}
