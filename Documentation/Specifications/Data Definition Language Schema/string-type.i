<h3 id="string-type">3.9 String type</h3>
<p>A DDL map with <code>kind: "String"</code> describes the DDLS
<code>String</code> definition. Without an <code>enum</code> entry, it
accepts every DDL string value. The optional <code>enum</code> entry
restricts acceptance to an explicitly enumerated set of DDL strings.</p>

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
has the same rules. Schema reference resolution and cycle checks apply
to those definitions.</p>

<p>A <code>String</code> definition without an <code>enum</code> entry
accepts every DDL String value. When <code>enum</code> is present, it
restricts the accepted values to its members. A <code>String</code>
definition imposes no pattern, length, or other restriction.</p>

<h4 id="string-enumeration-cases">3.9.2 Conformance cases</h4>
<p>The following cases define acceptance and rejection.</p>
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
reader before DDLS schema translation. The
<a href="@{siteAddress}/specifications/data-definition-language/index.html#type-string">DDL String definition</a>
governs escape syntax. DDLS error categories do not prescribe exact
diagnostic message text or C API representations.</p>
