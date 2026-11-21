<h3 id="string-type">3.9 String type</h3>
<p>A DDL map with <code>kind: "String"</code> describes the DDLS
<code>String</code> definition. Without an <code>enum</code> entry, it
accepts every DDL string value. The optional <code>enum</code> entry
restricts acceptance to an explicitly enumerated set of DDL strings.</p>

<p><strong>v0.3 String enumeration contract (issue #184; implementation on the
working branch, verification pending).</strong>
This subsection specifies an extension to the existing draft v0.2
language. It does not change the draft/version designation of this page.</p>

<h4 id="string-enumeration">3.9.1 String enumeration</h4>
<p>If present, <code>enum</code> must be a non-empty DDL list whose every
element is a DDL string. A malformed <code>enum</code> entry makes the
String definition invalid at schema translation time and must report
<code>InvalidDefinitionError</code>, not <code>ValidationError</code>.
A missing <code>enum</code> does not restrict the String definition.
No non-string value may be converted into a string to satisfy this rule.</p>

<p>All members of <code>enum</code> must be pairwise distinct. Equality
compares decoded DDL String values as exact sequences of Unicode scalar
values, case-sensitively. It does not compare source spellings or UTF-8
byte representations, and it performs no Unicode normalization, case
folding, locale-specific comparison or trimming. Consequently,
<code>"A"</code> and <code>"\u{41}"</code> denote the same member and
must be rejected as duplicates. The precomposed string
<code>"\u{E9}"</code> and the string <code>"e\u{301}"</code>
remain different members. Two copies of the same text, including the
empty string <code>""</code>, are duplicates regardless of their
positions in the list.</p>

<p>A valid enumerated String definition accepts a DDL value if and only if
that value has DDL kind String and its decoded value equals at least one
<code>enum</code> member under the preceding equality rule. Otherwise
validation must report <code>ValidationError</code>. Enumeration
members are not schema references or alternatives: they are literal
String values. Their order does not change acceptance or rejection.</p>

<p>Schema translation must reject an invalid enumeration even if the
definition occurs in a <code>Choice</code> alternative that would not
be selected for a particular input. An enumerated String definition
nested within a <code>List</code>, <code>MapEntry</code> type or named
schema, including a target reached through <code>SchemaReference</code>,
has the same rules. The existing rules for resolving references and
rejecting cyclic definitions are unchanged.</p>

<p><strong>Compatibility:</strong> Existing valid
<code>{ kind: "String" }</code> definitions remain unconstrained.
Adding <code>enum</code> opt-in narrows the accepted values. An
<code>enum</code> entry previously ignored by an implementation is
not evidence that it already supported enumerations; such schemas must
be reviewed on migration. This proposal does not specify patterns,
length limits, string conversion, or extra constraints.</p>

<h4 id="string-enumeration-cases">3.9.2 Conformance cases</h4>
<p>The following cases specify the required language behavior.
Runtime conformance remains subject to build and test verification.</p>
<pre><code>// Valid schema definition: accept "Nearest", "Linear", "Cubic" only.
{ kind: "Schema", name: "Filter", definition: {
  kind: "String", enum: ["Nearest", "Linear", "Cubic"]
} }

// Valid schema definition: unrestricted String.
{ kind: "Schema", name: "AnyString", definition: { kind: "String" } }

// Valid schema definition: an empty string is a permitted member.
{ kind: "String", enum: ["", "Other"] }

// Valid schema definition: canonically equivalent text is not normalized.
{ kind: "String", enum: ["\u{E9}", "e\u{301}"] }
</code></pre>
<p>With <code>Filter</code> selected, the separate DDL values
<code>"Nearest"</code> and <code>"Linear"</code> are accepted;
<code>"nearest"</code>, <code>"Unknown"</code>, <code>1</code>,
and <code>void</code> fail validation. With <code>AnyString</code>
selected, every DDL String value is accepted and a non-String value
fails validation.</p>
<pre><code>// Invalid definitions: InvalidDefinitionError at schema translation.
{ kind: "String", enum: [] }
{ kind: "String", enum: "Nearest" }
{ kind: "String", enum: ["Nearest", 1] }
{ kind: "String", enum: ["Nearest", "Nearest"] }
{ kind: "String", enum: ["A", "\u{41}"] }
{ kind: "String", enum: ["", ""] }
{ kind: "String", enum: void }
</code></pre>
<p>Invalid DDL lexical or syntactical input remains an error of the DDL
reader before DDLS schema translation; the extension does not redefine
DDL string escape syntax. Exact diagnostic message wording and the
representation of language-level errors in a C API are not specified
by this proposal.</p>
