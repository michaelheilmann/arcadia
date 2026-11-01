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

#include "Example/B/Include.h"

#if !defined(EXAMPLE_A_TYPES_FLAVOR_H_INCLUDED) || defined(EXAMPLE_A_FLAVOR_H_INCLUDED)
#error "Foreign mapped enum guards must follow the header path"
#endif

#if defined(EXAMPLE_A_PRIVATE) || defined(EXAMPLE_B_PRIVATE)
#error "Master headers must not expose their module-private switches"
#endif

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Value flavor = Example_B_defaultFlavor(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&flavor));
  Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&flavor);
  Arcadia_Tests_assertTrue(thread, enumeration.type == _Example_A_Flavor_getType(thread) && enumeration.value == Example_A_Flavor_High);
  Example_B* sameName = Example_B_create(thread, UINT64_MAX);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sameName);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Example_A_getStamp(thread, (Example_A*)sameName) == UINT64_MAX);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Example_A_answer(thread) == 41 && Example_B_answer(thread) == 42);
  Arcadia_Tests_assertTrue(thread, Example_B_answerFromA(thread) == 41);
  Example_B_Derived* derived = Example_B_Derived_create(thread, Arcadia_Natural64Value_Maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, derived);
  Example_B_Holder* holder = Example_B_Holder_create(thread, (void*)derived, Example_A_Flavor_High);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, holder);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, holder->flavor == Example_A_Flavor_High && Example_B_Holder_getStamp(thread, holder) == Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)holder->value, _Example_A_Base_getType(thread)));
  Arcadia_Tests_assertTrue(thread, _Example_B_Child_getType(thread) && _Example_A_Root_getType(thread));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static int
runTests
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "CrossModules", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
