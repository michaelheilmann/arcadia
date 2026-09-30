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

#if !defined(ARCADIA_ARMS_CONFIGURE_H_INCLUDED)
#define ARCADIA_ARMS_CONFIGURE_H_INCLUDED



// Define to 1 to enable the "locks" module.
#define Arcadia_ARMS_Configuration_WithLocks (1)

// Define to 1 to enable the "barriers" module.
#define Arcadia_ARMS_Configuration_WithBarriers (1)

// Define to 1 to enable the "notify destroy" module.
#define Arcadia_ARMS_Configuration_WithNotifyDestroy (1)

// Define to 1 to enable the "names" module.
#define Arcadia_ARMS_Configuration_WithNames (1)



#define Arcadia_ARMS_Configuration_InstructionSetArchitecture_Unknown @Arcadia.ARMS_InstructionSetArchitecture_Unknown@
#define Arcadia_ARMS_Configuration_InstructionSetArchitecture_X86 @Arcadia.ARMS_InstructionSetArchitecture_X86@
#define Arcadia_ARMS_Configuration_InstructionSetArchitecture_X64 @Arcadia.ARMS_InstructionSetArchitecture_X64@

#define Arcadia_ARMS_Configuration_InstructionSetArchitecture @Arcadia.ARMS_InstructionSetArchitecture@



#define Arcadia_ARMS_Configuration_OperatingSystem_Unknown @Arcadia.ARMS_OperatingSystem_Unknown@
#define Arcadia_ARMS_Configuration_OperatingSystem_Cygwin @Arcadia.ARMS_OperatingSystem_Cygwin@
#define Arcadia_ARMS_Configuration_OperatingSystem_Ios @Arcadia.ARMS_OperatingSystem_IOS@
#define Arcadia_ARMS_Configuration_OperatingSystem_IosSimulator @Arcadia.ARMS_OperatingSystem_IOSSimulator@
#define Arcadia_ARMS_Configuration_OperatingSystem_Linux @Arcadia.ARMS_OperatingSystem_Linux@
#define Arcadia_ARMS_Configuration_OperatingSystem_Macos @Arcadia.ARMS_OperatingSystem_Macos@
#define Arcadia_ARMS_Configuration_OperatingSystem_Mingw @Arcadia.ARMS_OperatingSystem_Mingw@
#define Arcadia_ARMS_Configuration_OperatingSystem_Msys @Arcadia.ARMS_OperatingSystem_MSYS@
#define Arcadia_ARMS_Configuration_OperatingSystem_Unix @Arcadia.ARMS_OperatingSystem_Unix@
#define Arcadia_ARMS_Configuration_OperatingSystem_Windows @Arcadia.ARMS_OperatingSystem_Windows@

#define Arcadia_ARMS_Configuration_OperatingSystem @Arcadia.ARMS_OperatingSystem@



#define Arcadia_ARMS_Configuration_CompilerC_Unknown @Arcadia.ARMS_Compiler_C_Unknown@
#define Arcadia_ARMS_Configuration_CompilerC_Clang @Arcadia.ARMS_Compiler_C_Clang@
#define Arcadia_ARMS_Configuration_CompilerC_Gcc @Arcadia.ARMS_Compiler_C_GCC@
#define Arcadia_ARMS_Configuration_CompilerC_Msvc @Arcadia.ARMS_Compiler_C_MSVC@

#define Arcadia_ARMS_Configuration_CompilerC @Arcadia.ARMS_Compiler_C@



#define Arcadia_ARMS_Configuration_ByteOrder_Unknown @Arcadia.ARMS_ByteOrder_Unknown@
#define Arcadia_ARMS_Configuration_ByteOrder_BigEndian @Arcadia.ARMS_ByteOrder_BigEndian@
#define Arcadia_ARMS_Configuration_ByteOrder_LittleEndian @Arcadia.ARMS_ByteOrder_LittleEndian@

#define Arcadia_ARMS_Configuration_ByteOrder @Arcadia.ARMS_ByteOrder@



#endif // ARCADIA_ARMS_CONFIGURE_H_INCLUDED
