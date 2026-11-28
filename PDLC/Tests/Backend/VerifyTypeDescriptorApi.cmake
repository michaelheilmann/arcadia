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

# Inspect freshly generated backend-test sources, not checked-in production
# outputs. This catches generator regressions during the normal Backend build.
if(NOT DEFINED SOURCE_ROOT OR NOT IS_DIRECTORY "${SOURCE_ROOT}")
  message(FATAL_ERROR "SOURCE_ROOT must name an existing generated sources directory")
endif()

file(GLOB_RECURSE generatedSources LIST_DIRECTORIES FALSE
  "${SOURCE_ROOT}/*.c.g")
if(NOT generatedSources)
  message(FATAL_ERROR "No generated C bodies found beneath ${SOURCE_ROOT}")
endif()

# Match complete C identifiers so canonical Descriptor-suffixed names do not
# accidentally match the retired Type/TypeValue prefixes.
set(retiredIdentifiers
  Arcadia_Type
  Arcadia_TypeValue
  Arcadia_Type_Operations
  Arcadia_Type_Operations_Initializer
  Arcadia_Type_getOperations
  Arcadia_Type_getName
  Arcadia_Type_getHash
  Arcadia_Type_getKind
  Arcadia_Type_isDescendantType
  Arcadia_Value_getType
  Arcadia_Value_getTypeValue
  Arcadia_Value_isTypeValue
  Arcadia_Value_makeTypeValue
  Arcadia_Value_setTypeValue
  Arcadia_ValueTag_Type
  Arcadia_getType
  _Arcadia_Type_getType
)

foreach(source IN LISTS generatedSources)
  file(READ "${source}" contents)
  foreach(identifier IN LISTS retiredIdentifiers)
    string(REGEX MATCH "(^|[^A-Za-z0-9_])${identifier}([^A-Za-z0-9_]|$)"
      legacyMatch "${contents}")
    if(legacyMatch)
      message(FATAL_ERROR
        "PDLC generated retired descriptor API ${identifier} in ${source}")
    endif()
  endforeach()
endforeach()

# A regenerated eligible class must publish one combined field/method metadata
# set and retain the C ABI of its ordinary typed methods.
set(sizedSource "${SOURCE_ROOT}/Test/Sized.c.g")
if(NOT EXISTS "${sizedSource}")
  message(FATAL_ERROR "Missing generated source for Test.Sized")
endif()
file(READ "${sizedSource}" sizedContents)
set(descriptorFragments
  "publishMembersImpl"
  ".publishMembers = &publishMembersImpl"
  "Arcadia_FieldSpecification fields[1]"
  "Arcadia_MethodSpecification methods[3]"
  "offsetof(Test_Sized, length)"
  "fields[0].storageSize"
  "methods[0].invokeSizeValue0 = &methodAdapter_0"
  "methods[2].invokeSizeValue1 = &methodAdapter_2"
  "Arcadia_TypeDescriptor_publishMembers(thread, owner, fields, 1, methods, 3)"
)
foreach(fragment IN LISTS descriptorFragments)
  string(FIND "${sizedContents}" "${fragment}" index)
  if(index EQUAL -1)
    message(FATAL_ERROR "Missing generated descriptor fragment '${fragment}' in Test.Sized")
  endif()
endforeach()
