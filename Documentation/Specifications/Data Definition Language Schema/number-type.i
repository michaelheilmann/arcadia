<h3 id="number-type">3.6 Number type</h3>
<p>A DDL map with the required entry <code>kind: "Number"</code> describes the DDLS
<code>Number</code> definition. It accepts DDL number values that satisfy every
specified numeric bound.</p>

<h4 id="number-bounds">3.6.1 Numeric bounds</h4>
<p>A <code>Number</code> definition is a DDL map with required
<code>kind: "Number"</code> and at most one entry of each of these optional fields:
<code>minimum</code>, <code>exclusiveMinimum</code>, <code>maximum</code> and
<code>exclusiveMaximum</code>. Every present bound must be a DDL
<code>Number</code> value. DDL's existing duplicate-map-name restriction applies.
The handling of unrelated definition fields is unchanged.</p>
<pre><code>{ kind: "Number", minimum: 0, exclusiveMinimum: 5, maximum: 10, exclusiveMaximum: 10 }</code></pre>

<p>For numeric comparison, let <code>N(x)</code> be the exact mathematical finite
decimal denoted by the DDL Number spelling <code>x</code>, equivalently its signed
integral significand multiplied by a power of ten. DDL Number values retain
their original text, according to
<a href="@{siteAddress}/specifications/data-definition-language/index.html#type-number">DDL Number semantical translation</a>.
Numeric order must be exact, with no lexical string ordering or loss of digits
through conversion to machine floating point. Consequently
<code>N(+01) = N(1.0) = N(1e0)</code> and
<code>N(-0) = N(0)</code>. Values of other DDL kinds are never coerced to Number.</p>

<p>A DDL value satisfies a bounded <code>Number</code> definition exactly when
it is of DDL Number kind and <em>all</em> present bound conditions hold:</p>
<ul>
  <li><code>minimum: a</code>: <code>N(value) &gt;= N(a)</code>.</li>
  <li><code>exclusiveMinimum: a</code>: <code>N(value) &gt; N(a)</code>.</li>
  <li><code>maximum: b</code>: <code>N(value) &lt;= N(b)</code>.</li>
  <li><code>exclusiveMaximum: b</code>: <code>N(value) &lt; N(b)</code>.</li>
</ul>
<p>Inclusive and exclusive bounds may coexist on the same side and remain
conjunctive. An absent bound imposes no condition. The
<code>{ kind: "Number" }</code> form accepts every DDL Number value. A DDL
value outside a valid bound intersection fails with <code>ValidationError</code>.</p>

<p>During schema translation, report <code>InvalidDefinitionError</code> if any bound
is not a DDL Number or if the conjunction of bounds defines an empty interval.
For example, <code>minimum: 2, maximum: 1</code> and
<code>minimum: 1, exclusiveMaximum: 1</code> are invalid; the single-point
inclusive interval <code>minimum: 1, maximum: 1</code> is valid.
All supplied bounds must participate in the emptiness check.</p>

<p><strong>Conformance examples:</strong></p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="number-bounds">
<table class="syntax-directed-translation">
  <caption>Number bounds acceptance<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Definition</th><th scope="col">Accepted values</th><th scope="col">Rejected values</th></tr></thead>
  <tbody>
    <tr><td><code>{ kind: "Number" }</code></td><td><code>-1</code>, <code>1e1000</code></td><td><code>"1"</code>, <code>true</code></td></tr>
    <tr><td><code>{ kind: "Number", minimum: 0 }</code></td><td><code>0</code>, <code>+00.0</code></td><td><code>-0.1</code></td></tr>
    <tr><td><code>{ kind: "Number", exclusiveMinimum: 0 }</code></td><td><code>0.0001</code></td><td><code>0</code>, <code>-0</code></td></tr>
    <tr><td><code>{ kind: "Number", maximum: 10 }</code></td><td><code>10</code>, <code>-3</code></td><td><code>10.0001</code></td></tr>
    <tr><td><code>{ kind: "Number", exclusiveMaximum: 10 }</code></td><td><code>9.999</code></td><td><code>10</code></td></tr>
    <tr><td><code>{ kind: "Number", minimum: 0, exclusiveMinimum: 5, maximum: 10, exclusiveMaximum: 10 }</code></td><td><code>6</code>, <code>9.999</code></td><td><code>5</code>, <code>10</code></td></tr>
    <tr><td><code>{ kind: "Number", minimum: 1, maximum: 1 }</code></td><td><code>+01.00e0</code></td><td><code>1.0001</code></td></tr>
    <tr><td><code>{ kind: "Number", minimum: 9007199254740993 }</code></td><td><code>9007199254740993</code></td><td><code>9007199254740992</code></td></tr>
  </tbody>
</table>
</div>

<p><strong>Invalid-schema examples:</strong>
<code>{ kind: "Number", minimum: "0" }</code>,
<code>{ kind: "Number", maximum: false }</code>,
<code>{ kind: "Number", minimum: 2, maximum: 1 }</code>, and
<code>{ kind: "Number", exclusiveMinimum: 1, maximum: 1 }</code>.
An illegal DDL number spelling is rejected at the DDL lexical/syntactical
stage, not reclassified as a DDLS range violation.</p>

<p>The same rule applies to bounded Number definitions nested in
<code>Schema.definition</code>, <code>List.elements</code>,
<code>MapEntry.type</code>, or <code>Choice.choices</code>, and through
<code>SchemaReference</code>. It does not otherwise alter containing-construct,
choice or reference-resolution semantics.</p>

<p>A <code>Number</code> definition imposes no integer-only rule,
<code>multipleOf</code> condition, or other numerical restriction unless specified
by its four numeric bounds. The DDL Number grammar does not admit NaN or Infinity
as numeric literals. Numeric magnitude and exponent have no fixed machine-width
limits. Resource exhaustion produces an implementation resource error rather than
an approximate or incorrect comparison.</p>
