<h3 id="list-type">3.4 List type</h3>
<p>A DDL map with <code>kind: "List"</code> and an <code>elements</code> entry
describes a DDLS list definition. The <code>elements</code> value must be a DDL
map describing a DDLS definition; it specifies the type of every element in the
DDL list being validated. If that nested description cannot be read as a DDLS
definition, report <code>InvalidDefinitionError</code>.</p>
