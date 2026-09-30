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

#if !defined(ARCADIA_RING1_CONFIGURE_H_INCLUDED)
#define ARCADIA_RING1_CONFIGURE_H_INCLUDED



#define Arcadia_Configuration_InstructionSetArchitecture_Unknown @Arcadia.Ring1_InstructionSetArchitecture_Unknown@
#define Arcadia_Configuration_InstructionSetArchitecture_X86 @Arcadia.Ring1_InstructionSetArchitecture_X86@
#define Arcadia_Configuration_InstructionSetArchitecture_X64 @Arcadia.Ring1_InstructionSetArchitecture_X64@

#define Arcadia_Configuration_InstructionSetArchitecture @Arcadia.Ring1_InstructionSetArchitecture@



#define Arcadia_Configuration_OperatingSystem_Unknown @Arcadia.Ring1_OperatingSystem_Unknown@
#define Arcadia_Configuration_OperatingSystem_Cygwin @Arcadia.Ring1_OperatingSystem_Cygwin@
#define Arcadia_Configuration_OperatingSystem_Ios @Arcadia.Ring1_OperatingSystem_IOS@
#define Arcadia_Configuration_OperatingSystem_IosSimulator @Arcadia.Ring1_OperatingSystem_IOSSimulator@
#define Arcadia_Configuration_OperatingSystem_Linux @Arcadia.Ring1_OperatingSystem_Linux@
#define Arcadia_Configuration_OperatingSystem_Macos @Arcadia.Ring1_OperatingSystem_MacOS@
#define Arcadia_Configuration_OperatingSystem_Mingw @Arcadia.Ring1_OperatingSystem_MinGW@
#define Arcadia_Configuration_OperatingSystem_Msys @Arcadia.Ring1_OperatingSystem_MSYS@
#define Arcadia_Configuration_OperatingSystem_Unix @Arcadia.Ring1_OperatingSystem_Unix@
#define Arcadia_Configuration_OperatingSystem_Windows @Arcadia.Ring1_OperatingSystem_Windows@

#define Arcadia_Configuration_OperatingSystem @Arcadia.Ring1_OperatingSystem@



#define Arcadia_Configuration_CompilerC_Unknown @Arcadia.Ring1_Compiler_C_Unknown@
#define Arcadia_Configuration_CompilerC_Clang @Arcadia.Ring1_Compiler_C_Clang@
#define Arcadia_Configuration_CompilerC_Gcc @Arcadia.Ring1_Compiler_C_GCC@
#define Arcadia_Configuration_CompilerC_Msvc @Arcadia.Ring1_Compiler_C_MSVC@

#define Arcadia_Configuration_CompilerC @Arcadia.Ring1_Compiler_C@



#define Arcadia_Configuration_Version_Major 0
#define Arcadia_Configuration_Version_Minor 1



#define Arcadia_Configuration_ByteOrder_Unknown @Arcadia.Ring1_ByteOrder_Unknown@
#define Arcadia_Configuration_ByteOrder_BigEndian @Arcadia.Ring1_ByteOrder_BigEndian@
#define Arcadia_Configuration_ByteOrder_LittleEndian @Arcadia.Ring1_ByteOrder_LittleEndian@

#define Arcadia_Configuration_ByteOrder @Arcadia.Ring1_ByteOrder@



// If the C language supports binary literals.
#define Arcadia_Configuration_C_HasBinaryLiterals (1)



// The size, in Bytes, of a limp.
#define Arcadia_Configuration_BigInteger_LimpSize 4
// Least significand at lowest array index, most significand at highest array index.
#define Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian (1)
// Most significand at lowest array index, least significand at highest array index.
#define Arcadia_Configuration_BigInteger_LimpOrder_BigEndian (2)

// The limp order used by the big integer implementation is little endian.
#define Arcadia_Configuration_BigInteger_LimpOrder Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian



#endif // ARCADIA_RING1_CONFIGURE_H_INCLUDED
