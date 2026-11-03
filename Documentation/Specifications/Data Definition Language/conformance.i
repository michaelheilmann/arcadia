<h2 id="errors">8 Errors</h2>
<p>Failure categories describe where translation stops; they do not prescribe diagnostic wording.</p>
<dl>
  <dt>Lexical error</dt><dd>The input cannot be read as words: for example <code>+</code>, <code>-</code>, <code>+e2</code>, <code>1e</code>, <code>"\q"</code>,
  <code>"\u{G}"</code> (a non-hexadecimal escape digit), <code>"\u0041"</code> (missing braces), an unterminated string or an unterminated block comment.
  The tested reader status is <code>LexicalError</code>.</dd>
  <dt>Syntactical error</dt><dd>The words cannot form a complete sentence: for example <code>{x 1}</code> (missing colon),
  <code>[1 2]</code> (missing comma), an empty document, <code>true false</code> (a second value), or <code>true,</code> (a top-level trailing comma).
  The tested reader status is <code>SyntacticalError</code>.</dd>
  <dt>Semantical error</dt><dd>A parsed sentence violates a value constraint: <code>{x:void, x:1}</code> repeats a map name.
  Explicit semantic analysis reports <code>SemanticalError</code>; parsing alone does not reject this sentence.</dd>
</dl>

<h2 id="conformance">9 Conformance and reader edges</h2>
<p>The regression fixtures exercise the scanner/parser and explicit semantic analysis separately.
These observations apply to the current implementation, not to every possible DDL implementation.
The approved DDL-specific rules and reader boundaries are summarized below.</p>
<p>DDL strings use double quotes only. <code>"text"</code> and <code>"It's ready"</code> are valid;
<code>'text'</code>, <code>'a'</code> and <code>''</code> fail with <code>LexicalError</code>.
There is no single-quoted character-literal form. This is an intentional DDL restriction of the shared string grammar,
not a reader/specification mismatch. The writer also emits double-quoted strings.</p>
<p>Map-entry names follow <code>[A-Za-z_][A-Za-z0-9_]*</code>, including <code>_</code>, <code>__</code> and <code>_1</code>.
They are case-sensitive ASCII names; <code>true</code>, <code>false</code> and <code>void</code> are reserved.
Digit-leading and quoted keys are invalid. This is the intended DDL name profile, not an unresolved difference from the shared grammar.</p>
<p>A document contains one top-level value, followed only by whitespace or comments and then end-of-input.
Top-level trailing commas such as <code>true,</code> and <code>[1,],</code> fail with <code>SyntacticalError</code>.
Trailing commas inside collections remain valid: <code>[1,]</code> and <code>{x:1,}</code>.</p>
<p><strong>Migration:</strong> remove a comma after the complete document value. Do not remove collection trailing commas unless desired for style.</p>
<p>Unicode escapes use only <code>\u{...}</code>, with one to six hexadecimal digits inside braces.
For example, <code>"\u{41}"</code> decodes to <code>"A"</code>; <code>"\u{1F3EE}"</code> has the same decoded UTF-8 text as a literal U+1F3EE.
Empty braces, more than six digits, non-hexadecimal digits, values above U+10FFFF and surrogate values U+D800 through U+DFFF fail with <code>LexicalError</code>.</p>
<p><strong>Migration:</strong> the former unbraced <code>\uXXXX</code> syntax is rejected, not retained as an alias.
Replace <code>\u0041</code> with <code>\u{0041}</code> (or <code>\u{41}</code>).
Do not wrap surrogate halves separately; use the complete scalar value instead. Other escape forms such as <code>\n</code> remain unchanged.</p>
<p>A numeric word requires at least one mantissa digit, and an exponent, when present, requires at least one digit.
The scanner rejects <code>+</code>, <code>-</code>, <code>+e2</code> and incomplete exponents such as <code>1e+</code> with <code>LexicalError</code>.
Valid forms such as <code>+1</code>, <code>.1</code>, <code>1.</code> and <code>1e2</code> retain their exact spelling.
This error-classification rule does not change the accepted numeric forms or the validation contract of direct Number-node API calls.</p>
<p>The short escapes follow the
<a href="@{siteAddress}/specifications/common-lexical-translations/index.html#productions-string-literal">shared string translation rules</a>:
<code>\b</code> denotes U+0008 (backspace), <code>\f</code> U+000C, <code>\t</code> U+0009,
<code>\v</code> U+000B and <code>\0</code> U+0000 (embedded NUL).
Braced escapes decode to Unicode scalar values; UTF-16 surrogate-pair escapes and normalization are not supported.</p>
<p>The repository's contract matrix and verification handoff are in
<code>Documentation/Specifications/Data Definition Language/Verification.md</code>.
The maintained cases are discoverable through the test executable's <code>--list</code> or CTest.</p>
