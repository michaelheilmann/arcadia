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

#include "Arcadia.Tools.GenerateRyuLookupTables/generateReal64Table.h"

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia.Tools.GenerateRyuLookupTables/Common.h"

#define POS_TABLE_SIZE 326
// The C version has two code paths, one of which requires an additional entry here.
#define NEG_TABLE_SIZE 342

// We intentionally choose these to be larger than or equal to the float equivalent + 64.
// This is B1 + 1
#define POW5_BITCOUNT 125 // max 127
// This is B0 + 1
#define POW5_INV_BITCOUNT 125 // max 127

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

static Arcadia_Integer32Value
pow5Bits
  (
    Arcadia_Integer32Value e
  )
{ return ((e * 1217359) >> 19) + 1; }

static Arcadia_BigInteger *
multiplier
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value i
  )
{
  // 5^i
  Arcadia_BigInteger* pow = Arcadia_BigInteger_create(thread);
  if (i < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BigInteger_setPowerOfFiveNatural32(thread, pow, i);
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, pow));
  //Arcadia_printNewline(thread);
#endif
  // length of 5^i in binary = ceil(log_2(5^i))
  Arcadia_Integer32Value pow5len = (Arcadia_Integer32Value)Arcadia_BigInteger_getBitLength(thread, pow);
#if _DEBUG
  //Arcadia_printInteger32(thread, pow5len);
  //Arcadia_printNewline(thread);
#endif
  // [5^i / 2^j] = [5^i / 2^(ceil(log_2(5^i)) - FLOAT64_POW5_BITCOUNT)]
  // By construction, this will have exactly FLOAT64_POW5_BITCOUNT bits. Note that this can shift left if j is negative!
  Arcadia_BigInteger* pow5DivPow2 = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_copy(thread, pow5DivPow2, pow);
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, pow5DivPow2));
  //Arcadia_printNewline(thread);
#endif
  Arcadia_BigInteger_shiftRightInteger64(thread, pow5DivPow2, pow5len - POW5_BITCOUNT);
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, pow5DivPow2));
  //Arcadia_printNewline(thread);
#endif
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, pow5DivPow2));
  //Arcadia_printNewline(thread);
#endif
  Arcadia_SizeValue j = Arcadia_BigInteger_getBitLength(thread, pow5DivPow2);
  if (j != POW5_BITCOUNT) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return pow5DivPow2;
}

// precondition: i >= 0
// Compute floor(2^k / 5^q) + 1 where
// - q = i as described in section 3.4
// - B0 = POV5_INV_BITCOUNT - 1
static Arcadia_BigInteger*
invMultiplier
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value i
  )
{
  // 5^i
  Arcadia_BigInteger* powerOfFive = Arcadia_BigInteger_create(thread);
  if (i < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BigInteger_setPowerOfFiveNatural32(thread, powerOfFive, i);
  // length of 5^i in binary = ceil(log_2(5^i))
  Arcadia_Integer32Value pow5len = (Arcadia_Integer32Value)Arcadia_BigInteger_getBitLength(thread, powerOfFive);
  // We want floor(log_2(5^i)) here, which is ceil(log_2(5^i)) - 1 (no power of 5 is a power of 2).
  Arcadia_Integer32Value j = pow5len - 1 + POW5_INV_BITCOUNT;
  // [2^j / 5^i] + 1 = [2^(floor(log_2(5^i)) + POW5_INV_BITCOUNT) / 5^i] + 1
  // By construction, this will have approximately POW5_INV_BITCOUNT + 1 bits.
  Arcadia_BigInteger* powerOfTwo = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfTwoNatural32(thread, powerOfTwo, j);
  Arcadia_BigInteger_divide(thread, powerOfTwo, powerOfFive);
  Arcadia_BigInteger_add(thread, powerOfTwo, ONE);
  if (Arcadia_BigInteger_getBitLength(thread, powerOfTwo) > POW5_INV_BITCOUNT + 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return powerOfTwo;
}

static void
printTable
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_RuntimeUTF8String* name,
    Arcadia_List* table,
    Arcadia_SizeValue entriesPerLine
  )
{
  static const Arcadia_BooleanValue pad = Arcadia_BooleanValue_True;
  Arcadia_SizeValue length = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)table);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"static const Arcadia_Natural64Value ");
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(name));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[");
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromSize(thread, length)));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"][2] = {\n");
  for (Arcadia_SizeValue i = 0; i < length; ++i) {
    Arcadia_BigInteger *pow5 = Arcadia_List_getBigIntegerValueAt(thread, table, i);
    Arcadia_BigInteger* pow5High = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_copy(thread, pow5High, pow5);
    Arcadia_BigInteger_shiftRightInteger32(thread, pow5High, 64);
    Arcadia_BigInteger* pow5Low = Arcadia_List_getBigIntegerValueAt(thread, table, i);
    Arcadia_BigInteger_and(thread, pow5Low, MASK64);
    if (i % entriesPerLine == 0) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  ");
    } else {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ");
    }
    Arcadia_RuntimeUTF8String* string = NULL;
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"{ ");
    string = Arcadia_BigInteger_toDecimalString(thread, pow5Low);
    if (pad && Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string) < 20) { // fill to 20 code points of shorter
      for (Arcadia_SizeValue j = 0, m = 20 - Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string); j < m; ++j) {
        Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ");
      }
    } else if (Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string) > 20) {
#if 1
      Arcadia_Thread_setStatus(thread, Arcadia_ProcessStatus_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
#endif
    }
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"Arcadia_Natural64Value_Literal(");
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(string));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"), ");
    string = Arcadia_BigInteger_toDecimalString(thread, pow5High);
    if (pad && Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string) < 19) { // fill to 19 code points of shorter
      for (Arcadia_SizeValue j = 0, m = 18 - Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string); j < m; ++j) {
        Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ");
      }
    } else if (Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, string) > 19) {
#if 1
      Arcadia_Thread_setStatus(thread, Arcadia_ProcessStatus_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
#endif
    }
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"Arcadia_Natural64Value_Literal(");
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(string));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"), ");
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" }");
    if (i != length - 1) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8",");
    }
    if (i % entriesPerLine == entriesPerLine - 1) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
    }
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"};\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
}

static void
printOffsets
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_RuntimeUTF8String* name,
    Arcadia_List* table,
    Arcadia_SizeValue entriesPerLine
  )
{
  Arcadia_SizeValue length = (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)table) + 15) / 16;
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"static const Arcadia_Natural32Value ");
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(name));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[");
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromSize(thread,length)));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"] = {\n");
#if 1
  for (Arcadia_SizeValue i = 0; i < length; ++i) {
    Arcadia_Natural32Value value = 0;
    for (Arcadia_SizeValue j = 0; j < 16; ++j) {
      Arcadia_SizeValue offset = i * 16 + j < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)table) ? Arcadia_List_getNatural32ValueAt(thread, table, i * 16 + j) : 0;
      value |= offset << (j << 1);
    }
    if (i % entriesPerLine == 0) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"  ");
    } else {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ");
    }
    Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromNatural32(thread, value)));
    if (i != length - 1) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8",");
    }
    if ((i % entriesPerLine == entriesPerLine - 1) || (i == length - 1)) {
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
    }
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"};\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
#endif
}

static void
doPrintLargeTables
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* target
  )
{
  //
  Arcadia_List* largeTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_Natural32Value i = 0; i < POS_TABLE_SIZE; ++i) {
    Arcadia_List_insertBackBigIntegerValue(thread, largeTable, multiplier(thread, i));
  }
  //
  Arcadia_List* largeInvTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_Natural32Value i = 0; i < NEG_TABLE_SIZE; i++) {
    Arcadia_List_insertBackBigIntegerValue(thread, largeInvTable, invMultiplier(thread, i));
  }
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL64_POW5_INV_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_INV_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL64_POW5_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  printTable(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_INV_SPLIT", sizeof(u8"REAL64_POW5_INV_SPLIT") - 1), largeInvTable, 2);
  printTable(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_SPLIT", sizeof(u8"REAL64_POW5_SPLIT") - 1), largeTable, 2);
  Arcadia_printStringBuffer(thread, target);
}

static void
doPrintSmallTables
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* target
  )
{
  //
  int mulTableSize = 26;
  Arcadia_List* smallInvTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (int i = 0; i < (NEG_TABLE_SIZE + mulTableSize - 1) / mulTableSize + 1; i++) {
    Arcadia_List_insertBackBigIntegerValue(thread, smallInvTable, invMultiplier(thread, mulTableSize * i));
  }
  //
  Arcadia_List* smallTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_Natural32Value i = 0; i < (POS_TABLE_SIZE + mulTableSize - 1) / (mulTableSize + 1); ++i) {
    Arcadia_List_insertBackBigIntegerValue(thread, smallTable, multiplier(thread, mulTableSize * i));
  }
  //
  Arcadia_BigInteger* mul = Arcadia_BigInteger_create(thread); // pull outside of loop to avoid unnecessary allocations
  Arcadia_BigInteger* result = Arcadia_BigInteger_create(thread); // pull outside of loop to avoid unnecessary allocations
  //
  Arcadia_List* invErrorTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_Natural32Value i = 0; i < NEG_TABLE_SIZE; i++) {
    Arcadia_Natural32Value base = (i + mulTableSize - 1) / mulTableSize;
    Arcadia_Natural32Value base2 = base * mulTableSize;
    Arcadia_Natural32Value offset = base2 - i;
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, mul, offset);
    Arcadia_BigInteger_copy(thread, result, Arcadia_List_getBigIntegerValueAt(thread, smallInvTable, base));
    Arcadia_BigInteger_subtract(thread, result, ONE);
    Arcadia_BigInteger_multiply(thread, result, mul);
    Arcadia_Integer32Value shift = pow5Bits(base2) - pow5Bits(i);
    Arcadia_BigInteger_shiftRightInteger32(thread, result, shift);
    Arcadia_BigInteger_add(thread, result, ONE);
    Arcadia_BigInteger* x = invMultiplier(thread, i);
    Arcadia_BigInteger_subtract3(thread, result, x, result);
  #if _DEBUG
    Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, result));
    Arcadia_printNewline(thread);
  #endif
    if (Arcadia_BigInteger_isNegative(thread, result) || Arcadia_BigInteger_compareTo(thread, result, TWO) > 0) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }

    Arcadia_List_insertBack(thread, invErrorTable, Arcadia_Value_makeNatural32Value(Arcadia_BigInteger_toNatural32(thread, result)));
  }
  //
  Arcadia_List* errorTable = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_Natural32Value i = 0; i < POS_TABLE_SIZE; ++i) {
    Arcadia_Natural32Value base = i / mulTableSize;
    Arcadia_Natural32Value base2 = base * mulTableSize;
    Arcadia_Natural32Value offset = i - base2;
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, mul, offset);
    Arcadia_BigInteger_copy(thread, result, Arcadia_List_getBigIntegerValueAt(thread, smallTable, base));
    Arcadia_BigInteger_multiply(thread, result, mul);
    Arcadia_BigInteger_shiftRightInteger32(thread, result, pow5Bits(i) - pow5Bits(base2));
    Arcadia_BigInteger_subtract3(thread, result, multiplier(thread, i), result);

    if (Arcadia_BigInteger_isNegative(thread, result) || Arcadia_BigInteger_compareTo(thread, result, TWO) > 0) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_List_insertBackNatural32Value(thread, errorTable, Arcadia_BigInteger_toNatural32(thread, result));
  }

  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL64_POW5_INV_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_INV_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"#define REAL64_POW5_BITCOUNT ");
  Arcadia_StringBuilder_insertBack(thread, target, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, POW5_BITCOUNT)));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  printTable(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_INV_SPLIT2", sizeof(u8"REAL64_POW5_INV_SPLIT2") - 1), smallInvTable, 1);
  printOffsets(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_OFFSETS", sizeof(u8"REAL64_POW5_OFFSETS") - 1), invErrorTable, 6);
  printTable(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_SPLIT2", sizeof(u8"REAL64_POW5_SPLIT2") - 1), smallTable, 1);
  printOffsets(thread, target, Arcadia_RuntimeUTF8String_create(thread, u8"REAL64_POW5_OFFSETS", sizeof(u8"REAL64_POW5_OFFSETS") - 1), errorTable, 6);
}

void
Arcadia_Tools_RyuLookupTableGenerator_generateReal64Table
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue printLargeTables,
    Arcadia_StringBuilder* target
  )
{
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

  // (1 * 2^64) - 1
  a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_copy(thread, a, ONE);
  Arcadia_BigInteger_shiftLeftNatural8(thread, a, 64);
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, a));
#endif
  Arcadia_BigInteger_subtract(thread, a, ONE);
#if _DEBUG
  //Arcadia_printRuntimeUTF8String(thread, Arcadia_BigInteger_toDecimalString(thread, a));
#endif
  if (Arcadia_ARMS_addNotifyDestroy(a, NULL, &MASK64, (Arcadia_ARMS_NotifyDestroyCallback*)&onNotifyDestroy)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  MASK64 = a;

  if (printLargeTables) {
    doPrintLargeTables(thread, target);
    Arcadia_printStringBuffer(thread, target);
    //Arcadia_printNewline(thread);
  } else {
    doPrintSmallTables(thread, target);
    Arcadia_printStringBuffer(thread, target);
    //Arcadia_printNewline(thread);
  }
}
