      <h2 id="text-format">10. Canonical text format, version 1</h2>
      <p>A <code>.vpl</code> file is UTF-8 without a byte-order mark. It contains one module form
      followed by end of input. Parenthesized forms make declarations, scopes, instruction operands,
      and boundaries explicit. Line endings are trivia, not instruction terminators. This format
      is independent of PDL and contains no nested arithmetic expressions.</p>

      <h3 id="text-lexical">10.1 Lexical selection and refinements</h3>
      <p>VPL selects Common Lexical Translations names, decimal numbers, quoted strings, Boolean
      and void words, parentheses, spaces/tabs, end-of-line sequences, comments, line continuations,
      and start/end conditions. Names are ASCII alphabetic names with the common underscore and
      digit suffix rules; matching is case-sensitive. Names <code>true</code>, <code>false</code>,
      and <code>void</code> are reserved. Other form-head words are contextual. Register identifiers
      refine names to <code>r</code> followed by decimal digits; their numeric IDs are function-local.</p>
      <p>Outside strings, spaces, tabs, common line endings, non-nesting solidus/asterisk comments,
      double-solidus line comments, and reverse-solidus immediately followed by a line ending are
      ignored. Comments separate words. Other punctuation is not a version-1 word alternative.
      Integer and real spelling overlap is resolved by operand/declaration context; a maximal
      number word must match the common decimal grammar in its entirety.</p>
      <p>Both quote styles are accepted. Raw C0 controls and line endings are forbidden inside
      strings. VPL explicitly refines the common escape spelling: reverse-solidus, either quote,
      <code>\b</code>, <code>\f</code>, <code>\n</code>, <code>\r</code>, and <code>\t</code> decode
      to reverse-solidus, that quote, U+0008, U+000C, U+000A, U+000D, and U+0009 respectively.
      <code>\uHHHH</code> uses exactly four hexadecimal digits (0-9, a-f, A-F). Surrogates are
      rejected rather than paired; non-BMP scalar characters can be written directly in UTF-8.
      These explicit refinements resolve the short-control/hexadecimal inconsistencies in the
      current common listing without inheriting another language's rules.</p>
      <p>Signed integer operands and locations accept an optional sign and decimal digits, within
      signed 32-bit range. Sizes and counts use unsigned decimal digits within Arcadia.Size range;
      register IDs use the digits following r. Decimal float operands accept a sign, decimal point,
      and/or exponent under the common number spelling, then round directly to binary32 using
      nearest-even conversion in a private C numeric locale. Underflow to signed zero is allowed;
      overflow to infinity is rejected. Invalid UTF-8, overlong encodings, encoded surrogates,
      unclosed strings/comments, and malformed numbers are lexical errors.</p>

      <h3 id="text-grammar">10.2 Grammar</h3>
      <p>In the following grammar, braces denote repetition and brackets optional syntax.
      Parentheses and words in quotes are literal. <code>id</code>, <code>reg</code>,
      <code>string</code>, <code>size</code>, <code>signedInteger</code>, and <code>number</code>
      denote the lexical categories/refinements above. Every count is exact unless repetition is shown.</p>
      <pre><code>module       : '(' 'module' version types uniforms vertex fragment ')'
version      : size /* its numeric value shall equal 1 */
types        : '(' 'types' {type} ')'
type         : '(' 'type' id scalarShape ')'
             | '(' 'type' id 'vector' elementKind size ')'
             | '(' 'type' id 'matrix' size size ')'
             | '(' 'type' id 'array' id size ')'
             | '(' 'type' id 'record' string {field} ')'
scalarShape  : 'void' | 'bool' | 'int' | 'float' | 'sampler2D'
elementKind  : 'bool' | 'int' | 'float'
field        : '(' 'field' string id ')'
uniforms     : '(' 'uniforms' {uniform} ')'
uniform      : '(' 'scalar' id string id ')'
             | '(' 'record' id string id ')'
             | '(' 'array' id string id size ')'
vertex       : '(' 'vertex' id interface functions ')'
fragment     : '(' 'fragment' id interface functions ')'
interface    : '(' 'interface' {io} ')'
io           : '(' direction id string id signedInteger semantic ')'
direction    : 'in' | 'out'
semantic     : 'none' | 'position' | 'color' | 'texcoord' | 'fragment_color'
functions    : '(' 'functions' {function} ')'
function     : '(' 'function' id string id parameters registers block {block} ')'
parameters   : '(' 'parameters' {parameter} ')'
parameter    : '(' 'parameter' id id ')'
registers    : '(' 'registers' {register} ')'
register     : '(' 'register' reg id ')'
block        : '(' 'block' id {instruction} terminator ')'

operand      : reg | '(' 'bool' boolean ')' | '(' 'int' signedInteger ')'
             | '(' 'float' number ')'
boolean      : 'true' | 'false'
reference    : '(' 'local' reg ')' | '(' 'parameter' id ')'
             | '(' 'uniform' id ')' | '(' 'stage' id ')'
             | '(' 'member_ref' reference string ')'
             | '(' 'index_ref' reference operand ')'

instruction  : '(' 'copy' reg operand ')'
             | '(' 'load' reg reference ')' | '(' 'store' reference operand ')'
             | '(' binary reg operand operand ')'
             | '(' 'construct' reg {operand} ')'
             | '(' 'call' result id {operand} ')'
             | '(' 'texture' reg operand operand ')'
             | '(' 'swizzle' reg operand string ')'
             | '(' 'member' reg operand string ')'
             | '(' 'index' reg operand operand ')'
binary       : 'add' | 'subtract' | 'multiply' | 'divide' | 'equal' | 'not_equal'
             | 'less' | 'less_equal' | 'greater' | 'greater_equal'
result       : reg | 'void'
terminator   : '(' 'branch' id ')' | '(' 'branch_if' operand id id ')'
             | '(' 'return' [operand] ')' | '(' 'discard' ')'</code></pre>
      <p>The module's second item is an unsigned integer value equal to 1; other versions are
      rejected. Matrix dimensions are rows then columns. Type IDs are module-wide; uniform IDs
      are module-wide; interface/function IDs are stage-local; parameter/register/block IDs are
      function-local. Display names and field names are quoted strings, distinct from binding IDs.</p>
      <p>Type, same-stage function, and same-function block forward references are permitted.
      Types must be acyclic, including unused declarations. Duplicate identifiers in each scope
      are rejected. Register spellings with the same numeric ID (for example r0 and r00) duplicate
      one register. The registers form creates the declaration prefix in the first block.</p>
      <p>References bind to explicitly declared slot types; field/element descriptors inherit the
      selected type. A construct uses its destination's declared type. This is binding of declared
      types, not source-expression type inference. Calls cannot name a function in another stage.
      A block has exactly one final terminator. Text reading always invokes canonical verification.</p>
      <p>Version 1 limits syntactic form/atom nesting and named-type dependency depth to 128.
      Canonical writing limits derived reference depth to 120 so emitted forms fit that reader
      bound. All counts, ranges, entry signatures, interfaces, operands, and output flows must also
      satisfy the semantic sections above.</p>

      <h3 id="text-example">10.3 Complete example</h3>
      <p>This exact source is also read and verified by <code>Arcadia.VPL.Tests.Text</code>:</p>
      <pre><code>@{include("./example.vpl")}</code></pre>

      <h2 id="publication">11. Text APIs and verified publication</h2>
      <p><code>Arcadia_VPL_Text_read</code> accepts bytes, byte count, diagnostics, and an optional
      source label. It returns a verified mutable module. Lexical, syntactic, and binding/verification
      failures raise LexicalError, SyntacticalError, or SemanticalError respectively. Structured
      diagnostics carry a source label, byte offset in <code>inputPosition</code>, and one-based
      Unicode-code-point line/column. Common CR/LF groups count as one line ending. Missing-object
      verifier diagnostics fall back to the module opening when no precise subject is available.</p>
      <p><code>Arcadia_VPL_Text_write</code> verifies a current module, emits deterministic text,
      and reads that result back through the same verifier before returning it. Type IDs are t0,
      t1, and so on in deterministic first-discovery order; uniforms, interface slots, functions,
      parameters, and blocks use u/s/f/p/b plus their list positions. Numeric register IDs are
      preserved. Display names, declaration/instruction/block order, nominal record relationships,
      and floating bits (including signed zero) are preserved. Unused type declarations, comments,
      trivia, and original binding-ID spellings are not preserved. The writer uses double quotes
      and Unicode escapes for controls, LF endings, and round-trippable finite decimal floats.</p>
      <p><code>Arcadia_VPL_VerifiedModule_create</code> publishes an opaque immutable canonical-text
      snapshot of a verified module. No mutable graph or cached-validity flag is exposed.
      <code>getText</code> returns immutable text; <code>materialize</code> returns a fresh, separately
      verified module. Mutating the original builder or one materialization cannot alter the saved
      snapshot or later materializations. The GLSL backend consumes this publication boundary.</p>
      <p>These APIs restore the value stack on success/failure and hold transient objects through
      a private text context. The scanner's generic forms are private parsing data, not a public
      expression AST or a higher-level convenience language.</p>
