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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_ATOMS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_ATOMS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Configure.h"
#include "Arcadia/ARMS/Include.h"
#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Size.h"

/// @brief
/// Atoms are UTF-8 strings which can be compared by address for equality and inequality.
/// @remarks
/// Atoms are kept in a data structure which allows efficient lookup of an atom by a sequence of Bytes.
/// Atoms maintain an age. Atoms below a certain age are marked as "life" during premark phase (unless purge cache is specified).
/// If an atom is dead, then in the finalize callback, its ADDRESS and its HASH value are used to effieciently remove the atom from the set of atoms.
typedef struct Arcadia_Atom Arcadia_Atom;

/// @brief Type of a pointer to an atom.
typedef Arcadia_Atom* Arcadia_AtomValue;

/// @brief
/// Get or create an atom.
/// @param flags
/// The flags. Must be Arcadia_AtomKind_Name.
/// @param bytes
/// A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes
/// The number of Bytes in the array pointed to by @a numberOfBytes.
/// @return
/// A pointer to the atom.
Arcadia_AtomValue
Arcadia_Atoms_getOrCreateAtom
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  );

/// @brief
/// Visit this Arcadia_Atom object.
/// @param thread
/// A pointer to the Arcadia_Thread object.
/// @param self
/// A pointer to this Arcadia_Atom object.
void
Arcadia_Atom_visit
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  );

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

/// @brief
/// Ensure this Arcadia_Atom object is gray.
/// @param thread
/// A pointer to this Arcadia_Thread object.
/// @param self
/// A pointer to this Arcadia_Atom object.
void
Arcadia_Atom_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  );

#endif

/// @brief
/// Get a pointer to the Bytes of this atom.
/// @param self
/// A pointer to this atom.
/// @return
/// A pointer to the Bytes of this atom.
const void*
Arcadia_Atom_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  );

/// @brief
/// Get the number of Bytes of this atom.
/// @param self
/// A pointer to this atom.
/// @return
/// The number of Bytes of this atom.
Arcadia_SizeValue
Arcadia_Atom_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  );

/// @brief
/// Get the hash of this atom.
/// @param self
/// A pointer to this atom.
/// @return
/// The hash of this atom.
Arcadia_SizeValue
Arcadia_Atom_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self
  );

/// @brief
/// Get if this atom is equal to another atom.
/// @param self
/// A pointer to this atom.
/// @param other
/// A pointer to the other atom.
/// @return
/// #Arcadia_BooleanValue_True if this atom is equal to the other atom.
/// #Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_Atom_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue self,
    Arcadia_AtomValue other
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_ATOMS_H_INCLUDED
