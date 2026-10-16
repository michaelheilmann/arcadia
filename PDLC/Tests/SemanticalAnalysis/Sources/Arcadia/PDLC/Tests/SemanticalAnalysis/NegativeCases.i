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

// Exact diagnostic contracts, not a transcript-derived runtime oracle. Offsets
// are byte positions; zero/absent positions preserve the compiler's current API.
// Select sections when expanding callback definitions or the ordered case table.
#if defined(PDLC_INCLUDE_LEGACY_CASES)
NEGATIVE(InterfaceNotAnInterface, "Arcadia.PDLC.Diagnostics.SymbolIsNoInterfaceDiagnostic", "Assets/InterfaceNotAnInterface/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: the symbol `MyModule.NotAnInterface` is no interface\n")
NEGATIVE(InterfaceCycle, "Arcadia.PDLC.Diagnostics.CyclicInterfaceInheritanceDiagnostic", "Assets/InterfaceCycle/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: the interface `MyModule.A` cannot be its own ancestor interface\n")
NEGATIVE(InterfaceOperationBody, "Arcadia.PDLC.Diagnostics.InterfaceOperationHasBodyDiagnostic", "Assets/InterfaceOperationBody/Library/Sources/MyModule/Types.pdl", 33,
  "error: interface operation `greet` must not have a body\n")
NEGATIVE(MissingModuleDefinition, "Arcadia.PDLC.Diagnostics.MissingModuleDefinitionDiagnostic", "Assets/MissingModuleDefinition/Module.pdl", ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: module definition file not found\n")
NEGATIVE(MultipleModuleDefinitions, "Arcadia.PDLC.Diagnostics.MultipleModuleDefinitionsDiagnostic", "Assets/MultipleModuleDefinitions/Module.pdl", 0,
  "error: confilcting module definition\n")
NEGATIVE(ParameterTypeNotDefined, "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/ParameterTypeNotDefined/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: symbol `Arcadia.Nonexistent` not defined\n")
NEGATIVE(ParameterTypeNotQualified, "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/ParameterTypeNotQualified/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: symbol `Greeter` not defined\n")
NEGATIVE(ProcedureParameterTypeNotDefined, "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/ProcedureParameterTypeNotDefined/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: symbol `Arcadia.Nonexistent` not defined\n")
NEGATIVE(ReturnTypeNotDefined, "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/ReturnTypeNotDefined/Library/Sources/MyModule/Types.pdl", 0,
  "semantical error: symbol `Arcadia.Nonexistent` not defined\n")
#endif

#if defined(PDLC_INCLUDE_CALLABLE_CASES)
PAIR_NEGATIVE(DuplicateModules, "DuplicateModules/A", "DuplicateModules/B", "Arcadia.PDLC.Diagnostics.SymbolIsAlreadyDefinedDiagnostic", "Assets/DuplicateModules/B/Module.pdl", 0,
  "semantical error: symbol `Test.DuplicateModule` already defined\n")
NEGATIVE(DuplicateLocal, "Arcadia.PDLC.CallableDiagnostic", "Assets/DuplicateLocal/Library/Sources/Empty.pdl", 47,
  "duplicate local variable or parameter")
NEGATIVE(UnknownVariable, "Arcadia.PDLC.CallableDiagnostic", "Assets/UnknownVariable/Library/Sources/Empty.pdl", 39,
  "undefined local variable or parameter `missing`")
NEGATIVE(UnknownLabel, "Arcadia.PDLC.CallableDiagnostic", "Assets/UnknownLabel/Library/Sources/Empty.pdl", 34,
  "undefined jump label")
NEGATIVE(BadCallCount, "Arcadia.PDLC.CallableDiagnostic", "Assets/BadCallCount/Library/Sources/Empty.pdl", 96,
  "procedure argument count mismatch")
NEGATIVE(NativeBody, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeBody/Library/Sources/Empty.pdl", 0,
  "native definitions must not have a PDL body")
NEGATIVE(BadOverride, "Arcadia.PDLC.CallableDiagnostic", "Assets/BadOverride/Library/Sources/Empty.pdl", 0,
  "incompatible method override")
NEGATIVE(ReturnValueVoid, "Arcadia.PDLC.CallableDiagnostic", "Assets/ReturnValueVoid/Library/Sources/Empty.pdl", 34,
  "void callable cannot return a value")
NEGATIVE(LiteralTarget, "Arcadia.PDLC.CallableDiagnostic", "Assets/LiteralTarget/Library/Sources/Empty.pdl", 34,
  "instruction target must be a register or local variable")
NEGATIVE(AbstractAllocation, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractAllocation/Library/Sources/Types.pdl", 69,
  "cannot instantiate an abstract class")
NEGATIVE(AbstractDynamicAllocation, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractDynamicAllocation/Library/Sources/Types.pdl", 105,
  "cannot instantiate an abstract class")
NEGATIVE(ReservedMember, "Arcadia.PDLC.CallableDiagnostic", "Assets/ReservedMember/Library/Sources/Types.pdl", 0,
  "member name is reserved by C/C++")
NEGATIVE(ReservedDispatch, "Arcadia.PDLC.CallableDiagnostic", "Assets/ReservedDispatch/Library/Sources/Types.pdl", 0,
  "member name is reserved by C/C++")
NEGATIVE(NativeEntryConflict, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeEntryConflict/Library/Sources/Types.pdl", 0,
  "native target conflicts with its generated entry point")
NEGATIVE(NativeAdapterConflict, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeAdapterConflict/Library/Sources/Types.pdl", 0,
  "native target conflicts with its generated entry point")
NEGATIVE(DuplicateRecordField, "Arcadia.PDLC.Diagnostics.SymbolIsAlreadyDefinedDiagnostic", "Assets/DuplicateRecordField/Library/Sources/Types.pdl", 100,
  "semantical error: symbol `width` already defined\n")
NEGATIVE(InvalidOwnership, "Arcadia.PDLC.CallableDiagnostic", "Assets/InvalidOwnership/Library/Sources/Types.pdl", 0,
  "owned storage requires an external reference")
NEGATIVE(InvalidReadOnly, "Arcadia.PDLC.CallableDiagnostic", "Assets/InvalidReadOnly/Library/Sources/Types.pdl", 0,
  "readonly requires an object view or external reference")
NEGATIVE(InvalidOutput, "Arcadia.PDLC.CallableDiagnostic", "Assets/InvalidOutput/Library/Sources/Types.pdl", 0,
  "output requires a writable external-reference parameter")
NEGATIVE(NonvirtualOverride, "Arcadia.PDLC.CallableDiagnostic", "Assets/NonvirtualOverride/Library/Sources/Types.pdl", 0,
  "nonvirtual members cannot participate in overrides")
NEGATIVE(QualifiedOverride, "Arcadia.PDLC.CallableDiagnostic", "Assets/QualifiedOverride/Library/Sources/Types.pdl", 0,
  "incompatible override parameter type")
NEGATIVE(ConstantRange, "Arcadia.PDLC.Diagnostics.IntegerLiteralOutOfRangeDiagnostic", "Assets/ConstantRange/Library/Sources/Types.pdl", 29,
  "error: value `2147483648` of initializer of constant `Value` is out of range\n")
NEGATIVE(MissingLifetimeProtocol, "Arcadia.PDLC.CallableDiagnostic", "Assets/MissingLifetimeProtocol/Library/Sources/Types.pdl", 0,
  "retained types require nonvirtual ref/unref lifetime operations")
NEGATIVE(DuplicateNativeHeaders, "Arcadia.PDLC.CallableDiagnostic", "Assets/DuplicateNativeHeaders/Library/Sources/Types.pdl", 0,
  "multiple declarations own the same generated header")
NEGATIVE(NativeAggregateHeader, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeAggregateHeader/Library/Sources/Types.pdl", 0,
  "a generated declaration cannot replace its module aggregate header")
NEGATIVE(OpaqueNativeHeader, "Arcadia.PDLC.CallableDiagnostic", "Assets/OpaqueNativeHeader/Library/Sources/Types.pdl", 0,
  "a generated header cannot replace an opaque native definition")
NEGATIVE(NativeHeaderExtension, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeHeaderExtension/Library/Sources/Types.pdl", 0,
  "native implementation declarations require an .h output path")
NEGATIVE(AbstractMethodConcreteOwner, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractMethodConcreteOwner/Library/Sources/Types.pdl", 0,
  "abstract methods require an abstract declaring class")
NEGATIVE(AbstractMethodBody, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractMethodBody/Library/Sources/Types.pdl", 0,
  "abstract methods cannot be nonvirtual or have a body or native target")
NEGATIVE(AbstractMethodNative, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractMethodNative/Library/Sources/Types.pdl", 0,
  "abstract methods cannot be nonvirtual or have a body or native target")
NEGATIVE(AbstractMethodNonvirtual, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractMethodNonvirtual/Library/Sources/Types.pdl", 0,
  "abstract methods cannot be nonvirtual or have a body or native target")
NEGATIVE(MissingAbstractMethod, "Arcadia.PDLC.CallableDiagnostic", "Assets/MissingAbstractMethod/Library/Sources/Types.pdl", 0,
  "concrete class `MyModule.Concrete` does not implement abstract method `read`")
NEGATIVE(PartialAbstractMethods, "Arcadia.PDLC.CallableDiagnostic", "Assets/PartialAbstractMethods/Library/Sources/Types.pdl", 0,
  "concrete class `MyModule.Concrete` does not implement abstract method `write`")
NEGATIVE(MissingReabstractedMethod, "Arcadia.PDLC.CallableDiagnostic", "Assets/MissingReabstractedMethod/Library/Sources/Types.pdl", 0,
  "concrete class `MyModule.Concrete` does not implement abstract method `read`")
NEGATIVE(AbstractOverrideSignature, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractOverrideSignature/Library/Sources/Types.pdl", 0,
  "incompatible override parameter type")
NEGATIVE(AbstractReferenceValue, "Arcadia.PDLC.CallableDiagnostic", "Assets/AbstractReferenceValue/Library/Sources/Types.pdl", 196,
  "external references and record values require a runtime value representation for executable code")
NEGATIVE(MappedClassPath, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `../Outside.h` is not a valid path\n")
NEGATIVE(NativeParentAllocation, "Arcadia.PDLC.CallableDiagnostic", "Assets/NativeParentAllocation/Library/Sources/Types.pdl", 160,
  "native class construction has no generated allocation entry point")
NEGATIVE(MappedEnumPath, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `../Outside.h` is not a valid path\n")
NEGATIVE(MappedEnumExtension, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `Test/Invalid.hpp` is not a valid path\n")
NEGATIVE(MappedEnumAggregate, "Arcadia.PDLC.CallableDiagnostic", "Assets/MappedEnumAggregate/Library/Sources/Types.pdl", 0,
  "a generated declaration cannot replace its module aggregate header")
NEGATIVE(MappedEnumDuplicate, "Arcadia.PDLC.CallableDiagnostic", "Assets/MappedEnumDuplicate/Library/Sources/Types.pdl", 0,
  "multiple declarations own the same generated header")
NEGATIVE(MappedEnumOpaque, "Arcadia.PDLC.CallableDiagnostic", "Assets/MappedEnumOpaque/Library/Sources/Types.pdl", 0,
  "a generated header cannot replace an opaque native definition")
NEGATIVE(MappedEnumNative, "Arcadia.PDLC.CallableDiagnostic", "Assets/MappedEnumNative/Library/Sources/Types.pdl", 0,
  "a generated header cannot replace an opaque native definition")
NEGATIVE(MappedEnumDoublePath, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/MappedEnumDoublePath/Library/Sources/Types.pdl", 78,
  "syntactical error: received <end of input>\n")
NEGATIVE(UnknownEnumConstant, "Arcadia.PDLC.CallableDiagnostic", "Assets/UnknownEnumConstant/Library/Sources/Types.pdl", 77,
  "undefined local variable or parameter `Test.Kind.Missing`")
NEGATIVE(EnumConstantTarget, "Arcadia.PDLC.CallableDiagnostic", "Assets/EnumConstantTarget/Library/Sources/Types.pdl", 81,
  "undefined local variable or parameter `Test.Kind.Present`")
NEGATIVE(ProcedureAggregateHeader, "Arcadia.PDLC.CallableDiagnostic", "Assets/ProcedureAggregateHeader/Library/Sources/Types.pdl", 0,
  "a generated declaration cannot replace its module aggregate header")
NEGATIVE(ProcedureTypeHeaderCollision, "Arcadia.PDLC.CallableDiagnostic", "Assets/ProcedureTypeHeaderCollision/Library/Sources/Types.pdl", 0,
  "multiple declarations own the same generated header")
NEGATIVE(QualifiedNativeEntryConflict, "Arcadia.PDLC.CallableDiagnostic", "Assets/QualifiedNativeEntryConflict/Library/Sources/Types.pdl", 0,
  "native target conflicts with its generated entry point")
NEGATIVE(QualifiedUnknownProcedure, "Arcadia.PDLC.CallableDiagnostic", "Assets/QualifiedUnknownProcedure/Library/Sources/Types.pdl", 37,
  "undefined procedure")
NEGATIVE(ModuleAsType, "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/ModuleAsType/Library/Sources/Types.pdl", 0,
  "semantical error: symbol `Test.ModuleOnly` not defined\n")
NEGATIVE(DuplicateTopLevelType, "Arcadia.PDLC.Diagnostics.SymbolIsAlreadyDefinedDiagnostic", "Assets/DuplicateTopLevelType/Library/Sources/Types.pdl", 0,
  "semantical error: symbol `Test.Duplicate` already defined\n")
#endif

#if defined(PDLC_INCLUDE_FILESYSTEM_CASES)
NEGATIVE(MissingSourceDirectory, "Arcadia.PDLC.Diagnostics.FileSystemDiagnostic", "Assets/MissingSourceDirectory/Library/Sources", ARCADIA_PDLC_TESTS_NO_POSITION,
  "source directory does not exist or is not a directory")
NEGATIVE(BlockedOutputDirectory, "Arcadia.PDLC.Diagnostics.FileSystemDiagnostic", "Assets/BlockedOutputDirectory/Library/Sources/MyModule/Blocked.c.g", ARCADIA_PDLC_TESTS_NO_POSITION,
  "cannot write generated file or create its parent directory")
#endif
