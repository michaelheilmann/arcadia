#if !defined(ARMS_TAG_H_INCLUDED)
#define ARMS_TAG_H_INCLUDED

#include "Arcadia/ARMS/Internal/Common.h"
#include <stdbool.h> // bool, true, false
#include <inttypes.h> // uint8_t
typedef struct ARMS_Type ARMS_Type; // Forward declaration.

#define Arcadia_ARMS_TagFlags_White (1)
#define Arcadia_ARMS_TagFlags_Black (2)
#define Arcadia_ARMS_TagFlags_Gray (Arcadia_ARMS_TagFlags_White|Arcadia_ARMS_TagFlags_Black)

typedef struct Arcadia_ARMS_Tag Arcadia_ARMS_Tag;

#if Arcadia_ARMS_Configuration_InstructionSetArchitecture_X64 == Arcadia_ARMS_Configuration_InstructionSetArchitecture

  // We must make sure that this thing is 64 bit aligned.
  #if Arcadia_ARMS_Configuration_CompilerC == Arcadia_ARMS_Configuration_CompilerC_Msvc
    #pragma pack(push, 8)
  #endif

#elif Arcadia_ARMS_Configuration_InstructionSetArchitecture_X32 == Arcadia_ARMS_Configuration_InstructionSetArchitecture

  // We must make sure that this thing is 32 bit aligned.
  #if Arcadia_ARMS_Configuration_CompilerC == Arcadia_ARMS_Configuration_CompilerC_Msvc
    #pragma pack(push, 4)
  #endif

#endif

struct Arcadia_ARMS_Tag {
  uint8_t flags;
  ARMS_Type* type;
  Arcadia_ARMS_Tag* allNext;
  Arcadia_ARMS_Tag* grayNext;
}
#if Arcadia_ARMS_Configuration_InstructionSetArchitecture_X64 == Arcadia_ARMS_Configuration_InstructionSetArchitecture
  // We must make sure that this thing is 64 bit aligned.
  #if Arcadia_ARMS_Configuration_CompilerC == Arcadia_ARMS_Configuration_CompilerC_Gcc
    __attribute__((aligned(16)))
  #endif
#elif Arcadia_ARMS_Configuration_InstructionSetArchitecture_X32 == Arcadia_ARMS_Configuration_InstructionSetArchitecture
  // We must make sure that this thing is 32 bit aligned.
  #if Arcadia_ARMS_Configuration_CompilerC == Arcadia_ARMS_Configuration_CompilerC_Gcc
    __attribute__((aligned(16)))
  #endif
#endif
;

#if Arcadia_ARMS_Configuration_CompilerC == Arcadia_ARMS_Configuration_CompilerC_Msvc
  #pragma pack(pop)
#endif

#if Arcadia_ARMS_Configuration_InstructionSetArchitecture_X64 == Arcadia_ARMS_Configuration_InstructionSetArchitecture
  Cxx_staticAssert(sizeof(Arcadia_ARMS_Tag) % 8 == 0, "Arcadia_ARMS_Tag size not 8 Byte aligned");
#elif Arcadia_ARMS_Configuration_InstructionSetArchitecture_X32 == Arcadia_ARMS_Configuration_InstructionSetArchitecture
  Cxx_staticAssert(sizeof(Arcadia_ARMS_Tag) % 4 == 0, "Arcadia_ARMS_Tag size not 4 Byte aligned");
#endif

static inline bool
Arcadia_ARMS_Tag_isWhite
  (
    Arcadia_ARMS_Tag const* tag
  )
{ return Arcadia_ARMS_TagFlags_White == (tag->flags & Arcadia_ARMS_TagFlags_Gray); }

static inline void
Arcadia_ARMS_Tag_setWhite
  (
    Arcadia_ARMS_Tag* tag
  )
{ tag->flags = (tag->flags & ~Arcadia_ARMS_TagFlags_Black) | Arcadia_ARMS_TagFlags_White;  }

static inline bool
Arcadia_ARMS_Tag_isGray
  (
    Arcadia_ARMS_Tag const* tag
  )
{ return Arcadia_ARMS_TagFlags_Gray == (tag->flags & Arcadia_ARMS_TagFlags_Gray); }

static inline void
Arcadia_ARMS_Tag_setGray
  (
    Arcadia_ARMS_Tag* tag
  )
{ tag->flags |= Arcadia_ARMS_TagFlags_Gray; }

static inline bool
Arcadia_ARMS_Tag_isBlack
  (
    Arcadia_ARMS_Tag const* tag
  )
{ return Arcadia_ARMS_TagFlags_Black == (tag->flags & Arcadia_ARMS_TagFlags_Gray); }

static inline void
Arcadia_ARMS_Tag_setBlack
  (
    Arcadia_ARMS_Tag* tag
  )
{ tag->flags = (tag->flags & ~Arcadia_ARMS_TagFlags_White) | Arcadia_ARMS_TagFlags_Black; }

#endif // ARMS_TAG_H_INCLUDED
