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

#include "Arcadia.Tools.GenerateRyuLookupTables/generateReal32Table.h"

#include "Arcadia/Ring2/Include.h"
#include "Arcadia.Tools.GenerateRyuLookupTables/Common.h"

#define POS_TABLE_SIZE 47
#define INV_TABLE_SIZE 55

#define POW5_BITCOUNT 61 /* max 63 */
#define POW5_INV_BITCOUNT 59 /* max 63 */

static Arcadia_BigInteger* ONE = NULL;
static Arcadia_BigInteger* MASK64 = NULL;
static Arcadia_BigInteger* TWO = NULL;

static void
onNotifyDestroy
  (
    void* argument1,
    void* argument2
  )
{
  Arcadia_BigInteger** variable = (Arcadia_BigInteger**)argument2;
  *variable = NULL;
}

void
Arcadia_Tools_RyuLookupTableGenerator_generateReal32Table
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* target
  )
{
  //
  Arcadia_BigInteger* a = NULL;
  // 1
  a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, a, 1);
  if (Arcadia_ARMS_addNotifyDestroy(a, NULL, &ONE, (Arcadia_ARMS_NotifyDestroyCallback*)&onNotifyDestroy)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  ONE = a;
  a = NULL;

  // 2
  a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, a, 2);
  if (Arcadia_ARMS_addNotifyDestroy(a, NULL, &TWO, (Arcadia_ARMS_NotifyDestroyCallback*)&onNotifyDestroy)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  TWO = a;
  a = NULL;

  Arcadia_BigInteger* pow5 = Arcadia_BigInteger_create(thread);
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL32_POW5_INV_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_INV_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL32_POW5_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"static const Arcadia_Natural64Value REAL32_POW5_INV_SPLIT");
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromSize(thread, INV_TABLE_SIZE)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"] = {\n");
  // This table basically contains the lower half1
  for (Arcadia_SizeValue i = 0; i < INV_TABLE_SIZE; ++i) {
    Arcadia_BigInteger_setPowerOfFiveNatural64(thread, pow5, i); // Should be: setPowerOfFiveSize.
    Arcadia_Integer64Value pow5Len = (Arcadia_Integer64Value)Arcadia_BigInteger_getBitLength(thread, pow5);
    Arcadia_Integer64Value j = pow5Len - 1 + POW5_INV_BITCOUNT;
    Arcadia_BigInteger* result = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_copy(thread, result, ONE);
    Arcadia_BigInteger_shiftLeftInteger64(thread, result, j);
    Arcadia_BigInteger_divide(thread, result, pow5);
    Arcadia_BigInteger_add(thread, result, ONE);
    Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, result);
    Arcadia_RuntimeUTF8String* vs = Arcadia_RuntimeUTF8String_createFromNatural64(thread, v);
    Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"  ");
    if (Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, vs) < 19) {
      for (Arcadia_SizeValue j = 0, m = 19 - Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, vs); j < m; ++j) {
        Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ");
      }
    }
    Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(vs));
    if (i < INV_TABLE_SIZE - 1) {
      Arcadia_StringBuilder_insertBackCxxString(thread, target, u8",");
    }
    if (i % 4 == 3) {
      Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
    }
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n};\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"static const Arcadia_Natural64Value REAL32_POW5_SPLIT");
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromSize(thread, POS_TABLE_SIZE)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"] = {\n");
  for (Arcadia_SizeValue i = 0; i < POS_TABLE_SIZE; ++i) {
    Arcadia_BigInteger_setPowerOfFiveNatural64(thread, pow5, i); // Should be: setPowerOfFiveSize.
    Arcadia_Integer64Value pow5Len = (Arcadia_Integer64Value)Arcadia_BigInteger_getBitLength(thread, pow5);
    Arcadia_Integer64Value j = pow5Len - POW5_BITCOUNT;
    Arcadia_BigInteger* result = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_copy(thread, result, pow5);
    Arcadia_BigInteger_shiftRightInteger64(thread, result, j);
    Arcadia_Natural64Value v =Arcadia_BigInteger_toNatural64(thread, result);
    Arcadia_RuntimeUTF8String* vs = Arcadia_RuntimeUTF8String_createFromNatural64(thread, v);
    Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"  ");
    if (Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, vs) < 19) {
      for (Arcadia_SizeValue j = 0, m = 19 - Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, vs); j < m; ++j) {
        Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ");
      }
    }
    Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(vs));
    if (i < POS_TABLE_SIZE - 1) {
      Arcadia_StringBuilder_insertBackCxxString(thread, target, u8",");
    }
    if (i % 4 == 3) {
      Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
    }
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n};\n");
}
