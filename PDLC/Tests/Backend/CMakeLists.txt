# Arcadia
# Copyright (C) 2024-2026 Michael Heilmann
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.29)

set(this ${MyProjectName}.PDLC.Tests.Backend.Generator)
BeginProduct(${this} executable)
SetFolder(${this} PDLC)
OnSourceFile(${this} Arcadia/PDLC/Tests/Backend/Generator.c)
OnModuleDependency(${this} ${MyProjectName}.PDLC PRIVATE)
EndProduct(${this})

set(generator ${this})
set(generatedRoot "${CMAKE_CURRENT_BINARY_DIR}/Sources/Generated/$<CONFIG>")
set(generatedSources)
set(generatedHeaders)
set(generatedClasses
  ColoredChild
  AbstractBase AbstractMiddle AbstractConcrete AbstractInherited AbstractAgain AbstractFinal AbstractNativeBase AbstractNativeConcrete
  Base Derived Native Holder Colored Event EmptyEvent ConcreteEvent
  FieldNames MethodNames ValueBox DerivedBox OptionalContext DerivedContext
  ExternalDerived
  Sized
  NativeEnumHolder
  NativeEnumBox NativeEnumChild
  Nonvirtual
)
set(generatedProcedures
  strictEnumConstruct
  enumConstantRun nativeEnumConstant wrongEnumConstant
  bareReturn
  abstractMethodRun
  compute twice factorial fail exercise zero emptyCall collect builtin holderRun
  colorRun abstractRun escapedRun identity nativeIdentity relay makeBase voidArgument renamed
  nullableIdentity nullableOptional nullableCall nullableRun nullableNullReceiver nullableNullField
  externalIdentity externalRun
  sizeIdentity sizeFromAny
  readOnlyValue
  nativeHeaderRun
  nativeEnumFromAny nativeEnumConstruct nativeUnsignedEnumIdentity
)
foreach(name IN LISTS generatedClasses)
  list(APPEND generatedSources "${generatedRoot}/Library/Sources/Test/${name}.c.g")
  list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/Test/${name}.h")
endforeach()
foreach(name IN LISTS generatedProcedures)
  list(APPEND generatedSources "${generatedRoot}/Library/Sources/${name}.c.g")
  list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/${name}.h")
endforeach()
foreach(name nativeIdentity roundTrip)
  list(APPEND generatedSources "${generatedRoot}/Library/Sources/Test/Namespaced/${name}.c.g")
  list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/Test/Namespaced/${name}.h")
endforeach()
list(APPEND generatedSources "${generatedRoot}/Library/Sources/Test/Palette/Color.c")
list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/Test/Palette/Color.h")
list(APPEND generatedSources "${generatedRoot}/Library/Sources/Test/OtherColor.c")
list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/Test/OtherColor.h")
list(APPEND generatedSources "${generatedRoot}/Library/Sources/Test/Optional.c")
list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/Test/Optional.h")
list(APPEND generatedHeaders "${generatedRoot}/Library/Sources/NativeGenerated/Type.h")
add_custom_command(
  OUTPUT ${generatedSources} ${generatedHeaders}
  # Unlinked same-field fixture: these outputs must not become link dependencies
  # of Test.Sized's accesses to its own declared length field.
  BYPRODUCTS "${generatedRoot}/Library/Sources/Test/UnlinkedSized.c.g"
             "${generatedRoot}/Library/Sources/Test/UnlinkedSized.h"
  # This is disposable test staging. Start fresh so old flat procedure headers
  # cannot conceal an incorrect generated include or output path.
  COMMAND ${CMAKE_COMMAND} -E rm -rf "${generatedRoot}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${generatedRoot}/Library/Sources"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Module.pdl" "${generatedRoot}/Module.pdl"
  COMMAND $<TARGET_FILE:${generator}> "${generatedRoot}"
  COMMAND ${CMAKE_COMMAND} "-DSOURCE_ROOT=${generatedRoot}/Library/Sources" -DMODULE_PATH=Test/Namespaced -DPROCEDURE=roundTrip
          -P "${CMAKE_CURRENT_SOURCE_DIR}/../VerifyProcedurePaths.cmake"
  DEPENDS ${generator} "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Module.pdl" "${CMAKE_CURRENT_SOURCE_DIR}/../VerifyProcedurePaths.cmake"
  VERBATIM
)
Arcadia_markGeneratedTestSources(${generatedSources})

set(this ${MyProjectName}.PDLC.Tests.Backend)
BeginProduct(${this} test)
SetFolder(${this} PDLC)
OnSourceFile(${this} Arcadia/PDLC/Tests/Backend/Main.c)
OnSourceFile(${this} NativeResources/External.c)
OnHeaderFile(${this} NativeResources/External.h)
OnSourceFile(${this} NativeResources/Declared.c)
foreach(source ${generatedSources})
  list(APPEND ${this}.SourceFiles "${source}")
endforeach()
OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
EndProduct(${this})
target_include_directories(${this} PRIVATE "${generatedRoot}/Library/Sources")
