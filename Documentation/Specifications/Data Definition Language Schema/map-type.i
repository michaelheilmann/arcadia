<h3 id="map-type">3.5 Map type</h3>
<p>A DDL map with <code>kind: "Map"</code> and an <code>entries</code> entry
describes a DDLS map definition. The <code>entries</code> value must be a DDL
list of map-entry descriptions. Read each list element as a
<code>MapEntry</code>, retaining its order. If an element is not a valid
<code>MapEntry</code>, report <code>InvalidDefinitionError</code>. Two entries
with the same <code>name</code> do not form a valid map definition.</p>

<h4>Map entry</h4>
<p>Each map-entry description is a DDL map containing
<code>kind: "MapEntry"</code>, a string <code>name</code>, and a
<code>type</code> map that describes a DDLS definition. The entry may also
contain <code>optional</code>. This field is a DDL <em>string</em>, not a DDL
boolean: only <code>"true"</code> and <code>"false"</code> are allowed. If
absent, it defaults to false. Entry fields are matched by name, regardless
of their order in the DDL map.</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="map-type">
<table class="syntax-directed-translation">
  <caption>Map entry translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Optional field</th><th scope="col">Result</th></tr></thead>
  <tbody>
    <tr><td>Absent or <code>"false"</code></td><td>A required entry with the given <code>name</code> and <code>type</code>.</td></tr>
    <tr><td><code>"true"</code></td><td>An optional entry with the given <code>name</code> and <code>type</code>.</td></tr>
    <tr><td>Any other value</td><td>The entry description is invalid.</td></tr>
  </tbody>
</table>
</div>
<p>The <code>type</code> map must itself be a valid DDLS definition. An
invalid nested definition makes the map-entry description invalid.</p>
