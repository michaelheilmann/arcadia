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

#include "Arcadia/PixelBufferIO/ImageManager.h"

#include "Arcadia/PixelBufferIO/ImageWriter.h"

#include "Arcadia/PixelBufferIO/ICOImageWriter.h"
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #include "Arcadia/PixelBufferIO/Windows/BMPImageWriter.h"
  #include "Arcadia/PixelBufferIO/Windows/PNGImageWriter.h"
  #include "Arcadia/PixelBufferIO/Windows/TIFImageWriter.h"
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include "Arcadia/PixelBufferIO/Linux/BMPImageWriter.h"
  #include "Arcadia/PixelBufferIO/Linux/PNGImageWriter.h"
#else
  #error("environment not (yet) supported")
#endif

static Arcadia_Imaging_ImageManager* g_instance = NULL;

static void
Arcadia_Imaging_ImageManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self
  );

static void
Arcadia_Imaging_ImageManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManagerDispatch* self
  );

static Arcadia_Imaging_ImageManager*
Arcadia_Imaging_ImageManager_create
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_Imaging_ImageManager_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self
  );

static void
Arcadia_Imaging_ImageManager_destroyCallback
  (
    void* observer,
    void* observed
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ImageManager_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Imaging_ImageManager_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ImageManager_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ImageManager", Arcadia_Imaging_ImageManager,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Imaging_ImageManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ImageManager);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->writers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Imaging_ImageWriter* writer = NULL;

  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_ICOImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_Windows_BMPImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_Windows_PNGImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_Windows_TIFImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_Linux_PNGImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
  writer = (Arcadia_Imaging_ImageWriter*)Arcadia_Imaging_Linux_BMPImageWriter_create(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->writers, writer);
#else
  #error("environment not (yet) supported")
#endif
  Arcadia_LeaveConstructor(Arcadia_Imaging_ImageManager);
}

static void
Arcadia_Imaging_ImageManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManagerDispatch* self
  )
{/*Intentionally empty.*/}

static Arcadia_Imaging_ImageManager*
Arcadia_Imaging_ImageManager_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ImageManager);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_ImageManager);
}

static void
Arcadia_Imaging_ImageManager_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self
  )
{
  if (self->writers) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->writers);
  }
}

static void
Arcadia_Imaging_ImageManager_destroyCallback
  (
    void* observer,
    void* observed
  )
{
  g_instance = NULL;
}

Arcadia_Imaging_ImageManager*
Arcadia_Imaging_ImageManager_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_Imaging_ImageManager* instance = Arcadia_Imaging_ImageManager_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) &Arcadia_Imaging_ImageManager_destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}

static Arcadia_BooleanValue
filter
  (
    Arcadia_Thread* thread,
    Arcadia_Value context,
    Arcadia_Value value
  )
{
  Arcadia_Imaging_ImageWriter* writer = Arcadia_Value_getObjectReferenceValue(&value);
  Arcadia_List* supportedExtensions = (Arcadia_List*)Arcadia_Imaging_ImageWriter_getSupportedTypes(thread, writer);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)supportedExtensions); i < n; ++i) {
    Arcadia_Value a = Arcadia_List_getAt(thread, supportedExtensions, i);
    Arcadia_Value b = context;
    if (Arcadia_Value_isEqualTo(thread, &a, &b)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

Arcadia_List*
Arcadia_Imaging_ImageManager_getWriters
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageManager* self,
    Arcadia_String* extension
  )
{
  Arcadia_List* list = Arcadia_List_filter(thread, self->writers, Arcadia_Value_makeObjectReferenceValue(extension), &filter);
  return list;
}
