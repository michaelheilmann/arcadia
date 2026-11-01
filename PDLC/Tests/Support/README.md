# PDLC expected-diagnostic support

`Arcadia.PDLC.Tests.Support` is a test-only native C library. Production targets
must not depend on it. Its [header](Sources/Arcadia/PDLC/Tests/Include.h) exposes a
predicate for ordinary mismatches and an assertion that raises
`Arcadia_Status_TestFailed`.

An expectation requires all of the following:

- Exact observed status and exactly one diagnostic, with Error severity.
- Exact runtime diagnostic class, not merely any error or a base-class match.
- Exact normalized path suffix at a component boundary, including fixture name.
- Exact byte offset, or explicitly absent position (`NO_POSITION` is not a wildcard).
- Exact message text. For FileSystemDiagnostic, compare its reason field exactly
  and check its path separately, excluding configuration/session-specific prefixes.
- For ValueRaised, the raised object must actually be CompilationFailedException.

PDLC currently has no public numeric diagnostic-code API. Several distinct backend
errors share the private CallableDiagnostic class, so exact reason text is necessary
in addition to class/status/location. This support does not add compiler codes,
expose that private class, or change production diagnostic wording. Legacy wording
(including `confilcting`) is intentionally preserved in the contracts. Some native
diagnostics report offset zero or no file/position; tests check precisely that
existing contract rather than pretending finer locations exist.

Failure reports show the fixture, expected/actual status, diagnostic count, type,
severity, file, position and message. Reporting uses metadata directly and does not
load source files or line maps: an unavailable/wrong source must not turn an
expectation mismatch into a secondary I/O exception. Normal match/mismatch paths
restore their value-stack frame before returning or deliberately raising TestFailed.

Capture the observed status at the original jump boundary, then clear the thread
status before inspecting diagnostics/allocating assertion output. Retain the raised
value until validation, because ValueRaised expectations also check its actual
exception type. The test context/diagnostics must stay rooted throughout inspection.

## Fixture contracts and boundaries

- [SemanticalAnalysis/NegativeCases.i](../SemanticalAnalysis/Sources/Arcadia/PDLC/Tests/SemanticalAnalysis/NegativeCases.i)
  encodes 64 reviewed negative contracts. Each independent compilation fixture now
  has a named callback, its own original suite-local expected-error/process boundary,
  and a T04 private fixture copy. DuplicateModules compiles A/B together because the
  duplicate must be diagnosed in one context. NativeEnumerationValue remains a
  positive control. The former CallableErrors aggregate selector is retired, not
  retained as a duplicate workload. FileSystem still checks successful nested output
  creation; MissingSourceDirectory/BlockedOutputDirectory are independent negatives.
- [SyntacticalAnalysis/NegativeCases.i](../SyntacticalAnalysis/Sources/Arcadia/PDLC/Tests/SyntacticalAnalysis/NegativeCases.i)
  encodes all 12 parser negatives. The existing shared process, valid-input sequence
  and error-recovery/status-reset sequence remain one SyntacticalAnalysis callback.
  Each negative must actually jump with SyntacticalError and its exact diagnostic;
  stale status from a previous fixture is cleared before observing the next parser.
- PDLC Backend/AST negative entry-point tests already assert specific status values
  and retain those checks. LexicalAnalysis/LineMap and build generators are not
  negative-diagnostic oracles; their positive behavior checks remain unchanged.

The audit found ten pre-existing false positives. Eight callable fixtures contained
their intended declarations in Module.pdl but failed first on a comment-only
Library/Sources/Empty.pdl compilation unit. Their declarations now occupy that
source unit and Module.pdl retains only the module declaration. MissingModuleDefinition
and MultipleModuleDefinitions had no Library/Sources directory and failed on a
filesystem diagnostic; valid native fixture units now let the intended module
diagnostics be reached. No parser/compiler implementation or language policy was
changed to repair these inputs.

## Checker regression

`Arcadia.PDLC.Tests.Diagnostics --case ExpectedDiagnostics` exercises both the
predicate and actual raising assertion with wrong class, file/component boundary,
offset/presence, message, status, severity, missing/extra diagnostics and wrong
raised values. A correct observation must pass; each mismatch must raise TestFailed
without changing caller stack depth, including above 255 stack slots. Temporarily
removing either the type check or the byte-offset comparison makes this regression
fail; both guards are restored in the shipped source.
