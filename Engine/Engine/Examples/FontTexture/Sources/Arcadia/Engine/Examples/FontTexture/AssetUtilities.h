#if !defined(ARCADIA_ENGINE_DEMO_ASSETUTILITIES_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_ASSETUTILITIES_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/ADL/Include.h"

/// Add all files discovered in a search path to a list.
/// @param path The search  path.
/// @param list The list to add the files to.
static inline void
Arcadia_Engine_Demo_AssetUtilities_enumerateFiles
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* filePath,
    Arcadia_List* files
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);

  if (Arcadia_FilePath_isRelative(thread, filePath)) {
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_FilePath* newFilePath = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
    Arcadia_FilePath_append(thread, newFilePath, filePath);
    filePath = newFilePath;
  }

  Arcadia_DirectoryIterator* directoryIterator = Arcadia_FileSystem_createDirectoryIterator(thread, fileSystem, filePath);
  Arcadia_FilePath* dot = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8".")),
                  * dotdot = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8".."));
  while (Arcadia_DirectoryIterator_hasValue(thread, directoryIterator)) {
    Arcadia_FilePath* elementPath = Arcadia_DirectoryIterator_getValue(thread, directoryIterator);
    if (Arcadia_FilePath_isEqualTo(thread, elementPath, dot) || Arcadia_FilePath_isEqualTo(thread, elementPath, dotdot)) {
    } else {
      Arcadia_FilePath* temporary = Arcadia_FilePath_clone(thread, filePath);
      Arcadia_FilePath_append(thread, temporary, elementPath);
      if (Arcadia_FileSystem_directoryFileExists(thread, fileSystem, temporary)) {
        Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, temporary, files);
      } else {
        elementPath = temporary;
        Arcadia_List_insertBackObjectReferenceValue(thread, files, (Arcadia_Object*)elementPath);
      }
    }
    Arcadia_DirectoryIterator_nextValue(thread, directoryIterator);
  }
}

static inline Arcadia_ADL_ColorDefinition*
getColorDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* filePath,
    Arcadia_String* name
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, name);
  if (!definition) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, Arcadia_FilePath_parseGeneric(thread, filePath));
    definition = Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_ColorDefinition*)definition;
}

static inline Arcadia_ADL_MaterialDefinition*
getMaterialDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* filePath,
    Arcadia_String* name
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, name);
  if (!definition) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, Arcadia_FilePath_parseGeneric(thread, filePath));
    definition = Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_MaterialDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_MaterialDefinition*)definition;
}

static inline Arcadia_ADL_MeshDefinition*
getMeshDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* filePath,
    Arcadia_String* name
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, name);
  if (!definition) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, Arcadia_FilePath_parseGeneric(thread, filePath));
    definition = Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_MeshDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_MeshDefinition*)definition;
}

static inline Arcadia_ADL_SampleBufferDefinition*
getSampleBufferDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* filePath,
    Arcadia_String* name
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, name);
  if (!definition) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, Arcadia_FilePath_parseGeneric(thread, filePath));
    definition = Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_SampleBufferDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_SampleBufferDefinition*)definition;
}

static inline Arcadia_ADL_ModelDefinition*
getModelDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* filePath,
    Arcadia_String* name
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, name);
  if (!definition) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, Arcadia_FilePath_parseGeneric(thread, filePath));
    definition = Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_ModelDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ADL_ModelDefinition*)definition;
}

#endif // ARCADIA_ENGINE_DEMO_ASSETUTILITIES_H_INCLUDED
