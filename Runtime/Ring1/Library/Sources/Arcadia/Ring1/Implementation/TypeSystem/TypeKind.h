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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPEKIND_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPEKIND_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

/// An enumeration of the type kinds.
typedef enum Arcadia_TypeKind {

  /// The "enumeration" type kind.
  Arcadia_TypeKind_Enumeration,

  /// The "interface" type kind.
  Arcadia_TypeKind_Interface,

  /// The "internal" type kind.
  /// Examples of this type kind are
  /// - <code>Arcadia.Atom</code>
  /// - <code>Arcadia.ImmutableByteArray</code>
  /// - <code>Arcadia.Memory</code>
  /// - <code>Arcadia.Type</code>
  Arcadia_TypeKind_Internal,

  /// The "object" type kind.
  Arcadia_TypeKind_Object,

  /// The "scalar" type kind.
  /// Examples of this type are:
  /// - <code>Arcadia.Boolean</code>
  /// - <code>Arcadia.ForeignProcedure</code>
  /// - <code>Arcadia.Integer(8|16|32|64)</code>
  /// - <code>Arcadia.Natural(8|16|32|64)</code>
  /// - <code>Arcadia.Real(32|64)</code>
  /// - <code>Arcadia.Size</code>
  /// - <code>Arcadia.Void</code>
  Arcadia_TypeKind_Scalar,

} Arcadia_TypeKind;

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPEKIND_H_INCLUDED
