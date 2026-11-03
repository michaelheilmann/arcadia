<h3 id="choice-type">3.3 Choice type</h3>
<p>A DDL map with <code>kind: "Choice"</code> and a <code>choices</code> entry
describes a DDLS choice definition. The <code>choices</code> value must be a
non-empty DDL list. Read each list element, in order, as a DDLS definition;
the resulting list is the choice's alternatives.</p>
<p>If the list is empty, or if an element cannot be read as a DDLS definition,
report <code>InvalidDefinitionError</code> during schema translation.</p>
