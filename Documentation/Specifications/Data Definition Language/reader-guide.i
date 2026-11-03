<h2 id="first-document">Your first DDL document</h2>
<p>This fictional Stone Lantern record is a complete DDL document, verified by the
<code>FirstDocument</code> reader fixture. It is an example of DDL structure, not an ADL asset or a DDLS schema.</p>
<pre id="first-document-code" tabindex="0" aria-label="Complete Stone Lantern DDL document"><code>{
  name: "Stone Lantern",
  enabled: true,
  size: 1.5,
  layers: ["stone", "light",],
  details: {note: void,},
}</code></pre>
<p>The outer map has five entries in source order. Each name is followed by a colon and a value.
<code>layers</code> contains a list of two strings; <code>details</code> contains another map whose
<code>note</code> has the value <code>void</code>. The commas before closing brackets are permitted.
Map names are not quoted strings, and the keywords <code>true</code>, <code>false</code> and <code>void</code> cannot be used as names.</p>

<h2 id="type-reference">Six-type quick reference</h2>
<p>Examples below are covered by <code>DocumentationContract</code> and the existing semantic fixtures.
Each invalid example is a complete input, not just a value of a different type.</p>
<div class="ddl-table specification-scroll" role="region" aria-label="DDL value types" tabindex="0">
<table>
  <caption>Values, examples and exact formal rules<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Type and meaning</th><th scope="col">Valid example / edges</th><th scope="col">Invalid example / reason</th><th scope="col">Rules</th></tr></thead>
  <tbody>
    <tr><th scope="row">Boolean: truth value</th><td><code>true</code>, <code>false</code>; lowercase keywords</td><td><code>True</code>: a name is not a value</td><td><a href="#syntax-boolean">Syntax</a>, <a href="#type-boolean">semantics</a></td></tr>
    <tr><th scope="row">Number: numeric spelling</th><td><code>-1.0e-10</code>; <code>+1</code>, <code>01</code>, <code>1.</code>, <code>.1</code>, <code>1E+2</code> retain their spelling</td><td><code>1e</code>: exponent digits are missing</td><td><a href="#syntax-number">Syntax</a>, <a href="#type-number">semantics</a></td></tr>
    <tr><th scope="row">String: decoded text</th><td><code>"Stone Lantern"</code>, <code>""</code>, <code>"\u{41}\n"</code> (A and line feed), <code>"\u{1F3EE}"</code> (U+1F3EE)</td><td><code>"\q"</code>: unsupported escape; <code>"\u0041"</code>: braces required</td><td><a href="#syntax-string">Syntax</a>, <a href="#type-string">semantics</a></td></tr>
    <tr><th scope="row">Void: one distinguished value</th><td><code>void</code>; not an empty string or empty collection</td><td><code>null</code>: not a DDL value keyword</td><td><a href="#syntax-void">Syntax</a>, <a href="#type-void">semantics</a></td></tr>
    <tr><th scope="row">List: ordered values</th><td><code>[]</code>, <code>[1,]</code>; nesting and mixed types allowed</td><td><code>[1 2]</code>: missing comma</td><td><a href="#syntax-list">Syntax</a>, <a href="#semantical-translation-list-type">semantics</a></td></tr>
    <tr><th scope="row">Map: ordered named entries</th><td><code>{}</code>, <code>{x:1,}</code>; names are case-sensitive and unique within each map</td><td><code>{x 1}</code>: missing colon; <code>{x:void, x:1}</code>: duplicate name (semantic error)</td><td><a href="#syntax-map">Syntax</a>, <a href="#semantical-translation-map-type">semantics</a></td></tr>
  </tbody>
</table>
</div>
