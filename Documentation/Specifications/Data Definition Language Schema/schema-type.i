<h3 id="schema-type">3.7 Schema type</h3>
<p>A DDL map with <code>kind: "Schema"</code>, a string <code>name</code>, and
a <code>definition</code> map describes a named DDLS schema. The definition map
must describe one of the DDLS definition kinds in this section. If the name or
definition cannot be read, or the definition does not describe a DDLS value,
report <code>InvalidDefinitionError</code> during schema translation. A malformed
schema description is not a validation failure of a DDL value.</p>
