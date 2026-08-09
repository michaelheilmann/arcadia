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

#include "Arcadia/Visuals/Implementation/OpenGL4/GLX/WindowBackend.h"

#include "Arcadia/Visuals/Implementation/Linux/ErrorHandler.h"
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

static void
openContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
applyIcons
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackendDispatch* self
  );

static void
openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static Arcadia_BooleanValue
getFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
setFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_BooleanValue fullscreen
  );

static void
getRequiredBigIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
getRequiredSmallIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static Arcadia_Visuals_Linux_Icon*
getBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
setBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Visuals_Linux_Icon* icon
  );

static Arcadia_Visuals_Linux_Icon*
getSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
setSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Visuals_Linux_Icon* icon
  );

static Arcadia_String*
getTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
setTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_String* title
  );

static void
getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
beginRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
endRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  );

static void
getBorderInfo
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  );

static void
getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  );

static void
setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  );

static void
getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static void
setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.GLX.WindowBackend", Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend,
                         u8"Arcadia.Visuals.WindowBackend", Arcadia_Engine_Visuals_WindowBackend,
                         &_typeOperations);

static void
openContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  glXMakeCurrent(self->backendContext->display, self->window, self->backendContext->context);
  XSync(self->backendContext->display, False);
  if (Arcadia_Visuals_Implementation_Linux_hasX11Error(thread)) {
    Arcadia_Visuals_Implementation_Linux_clearX11Error(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
applyIcons
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (!self->window) {
    return;
  }
  if (!self->smallIcon && !self->bigIcon) {
    XDeleteProperty(self->backendContext->display, self->window, self->backendContext->_NET_WM_ICON);
  } else {
    size_t numberOfElements = 0;
    Arcadia_Visuals_Linux_Icon* icons[] = {
      self->smallIcon,
      self->bigIcon,
    };
    Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
    for (size_t i = 0; i < 2; ++i) {
      Arcadia_Visuals_Linux_Icon* icon = icons[i];
      if (icon) {
        numberOfElements += icon->width * icon->height + 2;
        Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, icon->bytes, icon->numberOfBytes);
      }
    }
    void const* p = Arcadia_ByteArrayBuilder_getBytes(thread, byteBuffer);
    int n = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, byteBuffer);
    XChangeProperty(self->backendContext->display,
                    self->window,
                    self->backendContext->_NET_WM_ICON,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    Arcadia_ByteArrayBuilder_getBytes(thread, byteBuffer),
                    numberOfElements);
  }
  XSync(self->backendContext->display, False);
}

static Arcadia_BooleanValue
filter
  (
    Arcadia_Thread* thread,
    Arcadia_Value context,
    Arcadia_Value value
  )
{
  Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_Value_getObjectReferenceValue(&context);
  Arcadia_WeakReference* weakReferenceElement = (Arcadia_WeakReference*)Arcadia_Value_getObjectReferenceValue(&value);
  Arcadia_Value valueElement = Arcadia_WeakReference_getValue(thread, weakReferenceElement);
  if (Arcadia_Value_isObjectReferenceValue(&valueElement)) {
    Arcadia_Engine_Visuals_Window* windowElement = (Arcadia_Engine_Visuals_Window*)Arcadia_Value_getObjectReferenceValue(&valueElement);
    if (window == windowElement) {
      return Arcadia_BooleanValue_False;
    }
  } else {
    return Arcadia_BooleanValue_False;
  }
  return Arcadia_BooleanValue_True;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  Arcadia_logf(Arcadia_LogFlags_Info, "%s:%d: Enter: Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_destruct\n", __FILE__, __LINE__);
  if (self->window) {
    XDestroyWindow(self->backendContext->display, self->window);
    self->window = None;
  }

  if (self->screen) {
  #if 0
    // Do NOT call XFree on self->screen (unlike some examples suggest).
    XFree(self->screen);
  #endif
    self->screen = NULL;
  }
  Arcadia_List_filter(thread, ((Arcadia_Engine_Visuals_BackendContext*)self->backendContext)->windows, Arcadia_Value_makeObjectReferenceValue(self), &filter);
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
  Arcadia_logf(Arcadia_LogFlags_Info, "%s:%d: Leave: Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_destruct\n", __FILE__, __LINE__);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (self->backendContext) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->bigIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->bigIcon);
  }
  if (self->smallIcon) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->smallIcon);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext_getType(thread));
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  self->smallIcon = NULL;
  self->bigIcon = NULL;
  self->screen = NULL;
  self->window = None;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackendDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->open = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & openImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->close = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & closeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getRequiredBigIconSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getRequiredBigIconSizeImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getRequiredSmallIconSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getRequiredSmallIconSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getBigIcon = (Arcadia_Engine_Visuals_Icon*(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getBigIconImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setBigIcon = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Engine_Visuals_Icon*)) & setBigIconImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getSmallIcon = (Arcadia_Engine_Visuals_Icon*(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getSmallIconImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setSmallIcon = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Engine_Visuals_Icon*)) & setSmallIconImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getTitle = (Arcadia_String*(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getTitleImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setTitle = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_String*)) & setTitleImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getCanvasSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getCanvasSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->beginRender = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*))&beginRenderImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->endRender = (void(*)(Arcadia_Thread*,Arcadia_Engine_Visuals_WindowBackend*))&endRenderImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getPosition = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getPositionImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setPosition = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & setPositionImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & getSizeImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setSize = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & setSizeImpl;

  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->getFullscreen = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*)) & getFullscreenImpl;
  ((Arcadia_Engine_Visuals_WindowBackendDispatch*)self)->setFullscreen = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_WindowBackend*, Arcadia_BooleanValue)) & setFullscreenImpl;
}

static void
openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (!self->screen) {
    self->screen = DefaultScreenOfDisplay(self->backendContext->display);
    if (!self->screen) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }

  Arcadia_JumpTarget jumpTarget;

  if (!self->window) {
    int screenId = DefaultScreen(self->backendContext->display);

    Window rootWindow = RootWindowOfScreen(self->screen);
    int windowX = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.left;
    int windowY = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.top;
    int windowW = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.width;
    int windowH = ((Arcadia_Engine_Visuals_WindowBackend*)self)->bounds.height;
    // Does not seem to have an effect when using GNOME.
    int windowBorderWidth = 0;
    int windowClass = InputOutput;
    int windowDepth = DefaultDepth(self->backendContext->display, screenId);

    int attributeValueMask = 0;
    XSetWindowAttributes windowAttributes = {};

  #if 0
    // Not really useful for us.
    windowAttributes.background_pixel = 0xffafe9af;
    attributeValueMask |= CWBackPixel;
  #endif

  #if 1
    // Does not seem to have an effect when using GNOME.
    windowAttributes.border_pixel = WhitePixel(self->backendContext->display, screenId);
    attributeValueMask |= CWBorderPixel;
  #endif

    windowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ExposureMask;
    attributeValueMask |= CWEventMask;

    windowAttributes.colormap = self->backendContext->colormap;
    attributeValueMask |= CWColormap;

    self->window = XCreateWindow(self->backendContext->display,
                                 rootWindow,
                                 windowX, windowY, windowW, windowH,
                                 windowBorderWidth,
                                 windowDepth,
                                 windowClass,
                                 self->backendContext->visualInfo->visual,
                                 attributeValueMask, &windowAttributes);
    if (!self->window) {

      XDestroyWindow(self->backendContext->display, self->window);
      self->window = None;

    #if 0
      // Do NOT call XFree on self->screen (unlike some examples suggest).
      XFree(self->screen);
    #endif
      self->screen = NULL;

      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  XMapWindow(self->backendContext->display, self->window);

  XSetWMProtocols(self->backendContext->display, self->window, &self->backendContext->WM_DELETE_WINDOW, 1);

  XSync(self->backendContext->display, False);
  if (Arcadia_Visuals_Implementation_Linux_hasX11Error(thread)) {
    Arcadia_Visuals_Implementation_Linux_clearX11Error(thread);

    XDestroyWindow(self->backendContext->display, self->window);
    self->window = None;

  #if 0
    // Do NOT call XFree on self->screen (unlike some examples suggest).
    XFree(self->screen);
  #endif
    self->screen = NULL;

    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    openContext(thread, self);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);

    XDestroyWindow(self->backendContext->display, self->window);
    self->window = None;

  #if 0
    // Do NOT call XFree on self->screen (unlike some examples suggest).
    XFree(self->screen);
  #endif
    self->screen = NULL;

    Arcadia_Thread_jump(thread);
  }

  XSync(self->backendContext->display, False);
}

static void
closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (self->window) {
    XDestroyWindow(self->backendContext->display, self->window);
    self->window = None;
  }

  if (self->screen) {
  #if 0
    // Do NOT call XFree on self->screen (unlike some examples suggest).
    XFree(self->screen);
  #endif
    self->screen = NULL;
  }
}

static Arcadia_BooleanValue
getFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{ return ((Arcadia_Engine_Visuals_Window*)self)->fullscreen; }

static void
setFullscreenImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_BooleanValue fullscreen
  )
{
  if (self->window) {
    if (((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen != fullscreen) {

      struct {
        unsigned long   flags;
        unsigned long   functions;
        unsigned long   decorations;
        long            inputMode;
        unsigned long   status;
      } hints = {
        .flags = 2, // indicate that we change the decorations
        .functions = 0, // ignored
        .decorations = fullscreen ? 0 : 1, // 0 means disables decorations, 1 enables decorations
        .inputMode = 0, // ignored
        .status = 0, // ingored
      };
      XSync(self->backendContext->display, False);
      XSync(self->backendContext->display, False);
      XChangeProperty(self->backendContext->display, self->window, self->backendContext->_MOTIF_WM_HINTS,
                                                   self->backendContext->_MOTIF_WM_HINTS,
                                                   32, PropModeReplace, (unsigned char *)&hints,
                                                   5);
      XSync(self->backendContext->display, False);
      ((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen = fullscreen;
      if (Arcadia_Visuals_Implementation_Linux_hasX11Error(thread)) {
        Arcadia_Visuals_Implementation_Linux_clearX11Error(thread);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    }
  } else {
    ((Arcadia_Engine_Visuals_WindowBackend*)self)->fullscreen = fullscreen;
  }
}

static void
getRequiredBigIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = 128;
  *height = 128;
}

static void
getRequiredSmallIconSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = 64;
  *height = 64;
}

static Arcadia_Visuals_Linux_Icon*
getBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{ return self->bigIcon; }

static void
setBigIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Visuals_Linux_Icon* icon
  )
{
  self->bigIcon = icon;
  applyIcons(thread, self);
}

static Arcadia_Visuals_Linux_Icon*
getSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{ return self->smallIcon; }

static void
setSmallIconImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Visuals_Linux_Icon* icon
  )
{
  self->smallIcon = icon;
  applyIcons(thread, self);
}

static Arcadia_String*
getTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{ return ((Arcadia_Engine_Visuals_Window*)self)->title; }

static void
setTitleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_String* title
  )
{
  if (!title) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_Value value;
  Arcadia_Value_setObjectReferenceValue(&value, (Arcadia_ObjectReferenceValue)title);
  Arcadia_StringBuilder_insertBack(thread, stringBuilder, value);
  Arcadia_Value_setObjectReferenceValue(&value, (Arcadia_ObjectReferenceValue)Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"", 1)));
  Arcadia_StringBuilder_insertBack(thread, stringBuilder, value);

  if (self->window) {
    XStoreName(self->backendContext->display, self->window,
               Arcadia_StringBuilder_getBytes(thread, stringBuilder));
  }
  ((Arcadia_Engine_Visuals_Window*)self)->title = title;
}

static void
getCanvasSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  if (!self->window) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  XWindowAttributes attr;
  if(!XGetWindowAttributes(self->backendContext->display, self->window, &attr)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  *width = attr.width;
  *height = attr.height;
}

static void
beginRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (!glXMakeCurrent(self->backendContext->display, self->window, self->backendContext->context)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
endRenderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self
  )
{
  if (self->backendContext->context == glXGetCurrentContext()) {
    glXSwapBuffers(self->backendContext->display, self->window);
  }
}

static void
getBorderInfo
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  )
{
  Atom type;
  int format;
  unsigned long nitems, bytes_after;
  unsigned char *property;
  Status status;
  status = XGetWindowProperty(self->backendContext->display, self->window, self->backendContext->_NET_FRAME_EXTENTS,
                              0, 16, 0, XA_CARDINAL, &type, &format, &nitems, &bytes_after, &property);
  if (status != Success) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  if (type == None || nitems != 4) {
    XFree(property);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  *left = (int)((long *)property)[0];
  *top = (int)((long *)property)[1];
  *right = (int)((long *)property)[2];
  *bottom = (int)((long *)property)[3];
  XFree(property);
}

static void
getPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top
  )
{
  XWindowAttributes attributes;
  XGetWindowAttributes(self->backendContext->display, self->window, &attributes);
  *left = attributes.x;
  *top = attributes.y;
}

static void
setPositionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top
  )
{
  XSync(self->backendContext->display, False);
  Arcadia_Integer32Value borderLeft, borderTop, borderRight, borderBottom;
  getBorderInfo(thread, self, &borderLeft, &borderTop, &borderRight, &borderBottom);
  XMoveWindow(self->backendContext->display, self->window, left + borderLeft, top + borderTop);
  XSync(self->backendContext->display, False);
  if (Arcadia_Visuals_Implementation_Linux_hasX11Error(thread)) {
    Arcadia_Visuals_Implementation_Linux_clearX11Error(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  XWindowAttributes attributes;
  XGetWindowAttributes(self->backendContext->display, self->window, &attributes);
  *width = attributes.width;
  *height = attributes.height;
}

static void
setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  XResizeWindow(self->backendContext->display, self->window, width, height);
  XSync(self->backendContext->display, False);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend*
Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend);
  if (backendContext) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_GLX_WindowBackend);
}
