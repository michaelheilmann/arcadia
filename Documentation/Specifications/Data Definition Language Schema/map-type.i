<h3 id="map-type">3.5 Map type</h3>
<p>A DDL map with <code>kind: "Map"</code> and an <code>entries</code> entry
describes a DDLS map definition. The <code>entries</code> value must be a DDL
list of map-entry descriptions. Read each list element as a
<code>MapEntry</code>, retaining its order. If an element is not a valid
<code>MapEntry</code>, report <code>InvalidDefinitionError</code>. Two entries
with the same <code>name</code> do not form a valid map definition.</p>

<h4 id="map-entry">3.5.1 Map entry</h4>
<p>Each map-entry description is a DDL map containing
<code>kind: "MapEntry"</code>, a string <code>name</code>, and a
<code>type</code> map that describes a DDLS definition. The entry may also
contain <code>optional</code>. If present, <code>optional</code> must be a DDL
Boolean value, expressed by the unquoted keyword <code>true</code> or
<code>false</code>, as defined by the
<a href="@{siteAddress}/specifications/data-definition-language/index.html#type-boolean">DDL Boolean semantical translation</a>.
It must not be a DDL String, Number, Void, List or Map value. If absent,
<code>optional</code> defaults to <code>false</code>. Entry fields are
matched by name, regardless of their order in the DDL map.</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="map-type">
<table class="syntax-directed-translation">
  <caption>Map entry translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Optional field</th><th scope="col">Result</th></tr></thead>
  <tbody>
    <tr><td>Absent or <code>false</code></td><td>A required entry with the given <code>name</code> and <code>type</code>.</td></tr>
    <tr><td><code>true</code></td><td>An optional entry with the given <code>name</code> and <code>type</code>.</td></tr>
    <tr><td>Any non-Boolean value, including <code>"true"</code> and <code>"false"</code></td><td>The entry description is invalid; report <code>InvalidDefinitionError</code> during schema translation.</td></tr>
  </tbody>
</table>
</div>
<p>The <code>type</code> map must itself be a valid DDLS definition. An
invalid nested definition makes the map-entry description invalid.</p>
<p>For a valid <code>MapEntry</code>, the flag controls only whether the
named entry may be absent from a DDL map being validated. It does not
change the entry's <code>type</code> or supply a default value. Every
present entry must satisfy its declared type, whether optional or required.
In particular, a present DDL <code>void</code> value satisfies an entry
of type <code>Void</code> but not an entry of type <code>String</code>.
The <a href="#validation-map">DDL map value validation</a> rules continue
to reject unknown entries and missing required entries.</p>
<p><strong>Examples (schema translation):</strong> The following
<code>MapEntry</code> descriptions are valid:</p>
<pre><code>{ kind: "MapEntry", name: "label", type: { kind: "String" }, optional: true }
{ kind: "MapEntry", name: "label", type: { kind: "String" }, optional: false }
{ kind: "MapEntry", name: "label", type: { kind: "String" } }</code></pre>
<p><strong>Examples (invalid schema descriptions):</strong> Each of the
following <code>optional</code> fields makes its containing
<code>MapEntry</code> invalid during schema translation, not a data
validation failure:</p>
<pre><code>optional: "true"
optional: "false"
optional: 1
optional: void
optional: []
optional: {}</code></pre>
<p><strong>Examples (validation):</strong> Consider the following
<code>Map</code> definition:</p>
<pre><code>{
  kind: "Map",
  entries: [
    { kind: "MapEntry", name: "label", type: { kind: "String" }, optional: true },
    { kind: "MapEntry", name: "present", type: { kind: "Void" } }
  ]
}</code></pre>
<p>The DDL value <code>{ present: void }</code> is accepted because
<code>label</code> may be absent and <code>present</code> matches
<code>Void</code>. The value
<code>{ label: "x", present: void }</code> is also accepted. The value
<code>{}</code> is rejected because the required entry
<code>present</code> is absent. The values
<code>{ label: void, present: void }</code> and
<code>{ present: "x" }</code> are rejected because present entries must
match their declared types. The value <code>{ present: void, extra: 1 }</code>
is rejected because unknown map entries are not permitted.</p>
<p><strong>Draft v0.2 compatibility correction:</strong> Schema descriptions
that encoded <code>optional</code> as the strings <code>"true"</code>
or <code>"false"</code> must be migrated to the corresponding unquoted
Boolean values. This is an intentional breaking correction of draft v0.2
schema syntax. A conforming DDLS reader must not accept those strings as
compatibility aliases. No change to the DDLS draft/version designation or
to other schema constructs is implied.</p>
