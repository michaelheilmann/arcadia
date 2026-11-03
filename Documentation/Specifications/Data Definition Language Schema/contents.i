<h2 id="introduction" tabindex="-1">1 Introduction</h2>

<p>
This draft specifies the Data Definition Language Schema (DDLS), a declarative way to describe constraints on
values written in the <a href="@{siteAddress}/specifications/data-definition-language/index.html">Data Definition Language (DDL)</a>.
It is intended for readers of DDL who need to describe or validate structured data.
</p>

<p>
A DDLS schema is itself written as a DDL value. A DDLS reader interprets that value as a schema;
a validator then checks a separate DDL value against a named schema.
Read the <a href="@{siteAddress}/specifications/data-definition-language/index.html#semantical-translation">DDL semantical translation</a>
first for the DDL value model. This page describes DDLS values and validation, not a replacement for the DDL lexical or syntactical rules.
</p>

<p>For orientation, the reader accepts a top-level DDL map with <code>kind: "Schema"</code>, a schema <code>name</code>,
and a <code>definition</code> map. The <a href="#schema-type">schema translation</a> and the other
<a href="#semantics">type descriptions</a> explain how DDLS values are read; the
<a href="#validation">validation section</a> describes acceptance.</p>

<span id="notation" aria-hidden="true"></span>
<h2 id="notations">2 Notation</h2>

<p>This specification describes DDL values as defined by the
<a href="@{siteAddress}/specifications/data-definition-language/index.html#semantical-translation">DDL semantical translation</a>.
Names such as <code>kind</code> and <code>name</code> denote entries in a DDL map;
quoted words such as <code>"Map"</code> denote DDL string values. A DDLS value is the
interpretation of a DDL schema description, not the DDL value being validated.</p>

<p>DDLS definitions have these kinds: <code>Any</code>, <code>Boolean</code>, <code>Choice</code>,
<code>List</code>, <code>Map</code>, <code>Number</code>, <code>SchemaReference</code>,
<code>String</code>, and <code>Void</code>. The scalar kinds are
<code>Boolean</code>, <code>Number</code>, <code>String</code>, and <code>Void</code>;
<code>List</code> and <code>Map</code> are aggregate kinds.
A <code>Schema</code> names a definition; a <code>MapEntry</code> describes one entry
within a map definition. Neither is itself a definition kind.</p>

<h2 id="semantics">3 Semantics</h2>

<p>The following sections describe how a DDL map is interpreted as a DDLS schema
or definition. If a required field is missing, has the wrong kind, or a nested
definition cannot be read, translation fails. The draft names these error categories;
they describe language outcomes, not a promise that the current C API returns a
structured error value.</p>

<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="semantics">
<table class="syntax-directed-translation">
  <caption>DDLS error categories<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Error</th><th scope="col">Meaning</th></tr></thead>
  <tbody>
    <tr><td><code>InvalidDefinitionError</code></td><td>A schema description cannot be interpreted as a valid definition.</td></tr>
    <tr><td><code>SchemaNotFoundError</code></td><td>A referenced or starting schema is unavailable.</td></tr>
    <tr><td><code>SchemaAlreadySpecifiedError</code></td><td>A schema name is specified more than once.</td></tr>
    <tr><td><code>CyclicDefinitionError</code></td><td>A schema-reference cycle is reachable from the starting schema.</td></tr>
    <tr><td><code>ValidationError</code></td><td>A DDL value fails to satisfy a usable schema.</td></tr>
  </tbody>
</table>
</div>

@{include("./any-type.i")}
@{include("./boolean-type.i")}
@{include("./choice-type.i")}
@{include("./list-type.i")}
@{include("./map-type.i")}
@{include("./number-type.i")}
@{include("./schema-type.i")}
@{include("./schema-reference-type.i")}
@{include("./string-type.i")}
@{include("./void-type.i")}

<h2 id="validation">4 Validation</h2>
<p>A validation context contains named schemas. Each schema has a <code>name</code>
and a <code>definition</code>. Validation begins with one schema name and a DDL
value to check. The <code>elements</code> field in a <code>List</code> definition
describes each DDL list element; a <code>Map</code> definition has an
<code>entries</code> list of named <code>MapEntry</code> definitions.
Each map entry has a <code>type</code> and an <code>optional</code> flag.</p>

<h3 id="validation-ddl-value">4.1 DDL value validation</h3>
<p>Validation has one successful outcome or an error. A successful outcome means
that the DDL value satisfies the starting schema. An error means either that the
value does not satisfy the schema or that the schema cannot be used. The
<a href="#semantics">error categories</a> distinguish these cases.</p>
<p>Find the starting schema by name in the validation context. If none exists,
report <code>SchemaNotFoundError</code>. If the context contains two schemas with
the same name, report <code>SchemaAlreadySpecifiedError</code>. Otherwise validate
the DDL value against that schema's definition as described below.</p>
<p><strong>Example (accepted):</strong> Read this DDL value as a DDLS schema and
register it under the name <code>MySchema</code>:</p>
<pre><code>{ kind: "Schema", name: "MySchema", definition: { kind: "Any" } }</code></pre>
<p>With <code>MySchema</code> selected, each of the following separate DDL values
is accepted by <code>Any</code>:</p>
<pre><code>true
[1, true, "w"]</code></pre>
<p><strong>Example (invalid schema):</strong> Replacing the definition with
<code>{ kind: "Choice", choices: [] }</code> makes the schema description invalid
during reading; it is not a rejected DDL value. This is an
<code>InvalidDefinitionError</code> in the draft's language-level categories.</p>

<h3 id="validation-value">4.2 DDLS value validation</h3>
<p>Check the DDLS definition against the DDL value as follows. When a check
fails, report a <code>ValidationError</code> unless a more specific schema error
applies.</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="validation-value">
<table class="syntax-directed-translation">
  <caption>Validation by definition kind<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">DDLS definition</th><th scope="col">When the DDL value satisfies it</th></tr></thead>
  <tbody>
    <tr><td><code>Any</code></td><td>Any DDL value satisfies it.</td></tr>
    <tr><td><code>Boolean</code>, <code>Number</code>, <code>String</code>, <code>Void</code></td><td>The DDL value has the corresponding kind.</td></tr>
    <tr><td><code>List</code></td><td>It is a DDL list and its elements satisfy the <a href="#validation-list">list rule</a>.</td></tr>
    <tr><td><code>Map</code></td><td>It is a DDL map and its entries satisfy the <a href="#validation-map">map rule</a>.</td></tr>
    <tr><td><code>Choice</code></td><td>At least one alternative satisfies the DDL value.</td></tr>
    <tr><td><code>SchemaReference</code></td><td>The referenced schema's definition satisfies the DDL value; see <a href="#schema-reference-resolution">schema reference resolution</a>.</td></tr>
  </tbody>
</table>
</div>
<p>For a <code>Choice</code>, alternatives are considered in their listed order.
An ordinary mismatch allows the next alternative to be tried; the first
matching alternative accepts the value. If none matches, report a
<code>ValidationError</code>. A structural schema error is not an alternative
mismatch and stops validation, even if the erroneous alternative would not be
tried after an earlier match. An empty <code>choices</code> list is rejected
during <a href="#choice-type">schema translation</a>, before validation.</p>

<h3 id="validation-list">4.3 DDL list value validation</h3>
<p>A <code>List</code> definition applies only to a DDL list. Each list element
must satisfy the definition in <code>elements</code>. Check the elements in order
and stop at the first failure; an empty DDL list satisfies this rule.</p>

<h3 id="validation-map">4.4 DDL map value validation</h3>
<p>A <code>Map</code> definition applies only to a DDL map. Its
<code>entries</code> list must not describe the same name twice; a duplicate
name makes the schema definition invalid. The DDL map must not contain duplicate
names either.</p>
<ul>
  <li>Every DDL map entry must have a matching name in the schema's <code>entries</code> list. Unknown entries fail validation.</li>
  <li>Every schema entry whose <code>optional</code> flag is false must be present in the DDL map. An optional entry may be absent.</li>
  <li>Every present entry, including an optional one, must satisfy its <code>type</code> definition. Stop at the first failure.</li>
</ul>
<p>The <code>optional</code> flag defaults to false; see the
<a href="#map-type">MapEntry translation</a> for its DDL representation.</p>

<h3 id="schema-reference-resolution">4.5 Schema reference resolution</h3>
<p>A <code>SchemaReference</code> names a schema in the validation context.
Register the intended schemas before validating a value; this permits forward
references between their definitions. Then check every definition reachable
from the starting schema, including all choice alternatives, list element
definitions, and both required and optional map entry types. Unrelated schemas
that cannot be reached from the starting schema are not part of this check.
If any referenced schema name is unavailable, report
<code>SchemaNotFoundError</code>. If a schema-reference cycle is reachable,
report <code>CyclicDefinitionError</code>. Shared references reached along
different acyclic paths are not cycles. When these checks succeed, validate
each reference against its named schema's <code>definition</code>.</p>

<h3 id="acceptance-and-rejection">4.6 Acceptance and rejection</h3>
<p>A DDL value is accepted if validation succeeds. A
<code>ValidationError</code> rejects the value. An unavailable, duplicate, invalid,
or cyclic schema is an error in the validation context or schema description;
it does not produce an acceptance verdict.</p>
<p><strong>Implementation note:</strong> These errors are language-level categories,
not C API return types. The current validator signals failures with a
semantic-error status and diagnostics rather than returning DDLS error values.</p>
