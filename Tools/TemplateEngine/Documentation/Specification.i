<section id="Specification">
  <h2>Specification</h2>
  <p>This section describes the behavior implemented by the current scanner,
  parser, evaluator, and CLI. Constructs identified as future work are not
  part of the current language.</p>

  <h3 id="TemplateLanguage">Template Language</h3>
  <p>A template is a sequence of ordinary UTF-8 text and directives. Ordinary
  text passes to the output unchanged. The current directives are:</p>
  <table>
    <thead><tr><th>Source spelling</th><th>Effect</th></tr></thead>
    <tbody>
      <tr><td><code>@@@@</code></td><td>Emit one literal <code>@@</code> character.</td></tr>
      <tr><td><code>@@{name}</code></td><td>Load the named environment variable and emit its string value.</td></tr>
      <tr><td><code>@@{procedure()}</code></td><td>Invoke a no-argument procedure and emit its result.</td></tr>
      <tr><td><code>@@{procedure("text")}</code></td><td>Invoke a procedure with one string-literal argument and emit its result.</td></tr>
    </tbody>
  </table>
  <p>In the invocation form, the parser accepts zero or more string-literal
  arguments separated by commas. A name without parentheses denotes a
  variable load; a name followed by parentheses denotes an invocation.
  Numeric arguments and user-authored procedure definitions are not currently
  supported by the parser. Whitespace and quoting must follow the accepted
  scanner grammar.</p>
  <p>An undefined variable, an attempt to invoke a non-procedure, a syntax
  error, or an invalid procedure argument causes evaluation to fail. Template
  content is evaluated from left to right; a failure prevents successful
  completion of the invocation.</p>

  <h3>Environment and name resolution</h3>
  <p>An optional environment file is parsed as an
  <a href="@{siteAddress}/specifications/data-definition-language">Arcadia DDL</a>
  value of type <em>Map</em>. Every entry value must be a DDL string.
  An absent environment file yields no user variables; built-in procedures
  remain available. Names in the supplied environment shadow names in its
  enclosing default environment, including built-in procedures.</p>
  <pre><code>{
  title : "Arcadia",
  description : "Template example",
}</code></pre>
  <p>Use <code>@@{title}</code> to insert the value of <code>title</code>.
  Invoking that string as <code>@@{title()}</code> is an error.</p>

  <h3>Built-in procedures</h3>
  <p>All date/time procedures below take no arguments and use <strong>UTC</strong>.
  Values are evaluated when the procedure runs, not frozen when processing
  starts.</p>
  <table class="invocation-directives">
    <thead><tr><th>Procedure</th><th>Result</th></tr></thead>
    <tbody>
      <tr><td><code>include("path")</code></td><td>Evaluate and insert a file; one string argument, resolved relative to the process working directory.</td></tr>
      <tr><td><code>dayOfWeek()</code></td><td>Weekday number, Monday = 1 through Sunday = 7 (UTC).</td></tr>
      <tr><td><code>dayOfWeekName()</code></td><td>English weekday name (UTC).</td></tr>
      <tr><td><code>dayOfMonth()</code></td><td>Day number, 1 through 31 (UTC).</td></tr>
      <tr><td><code>month()</code></td><td>Month number, 1 through 12 (UTC).</td></tr>
      <tr><td><code>monthName()</code></td><td>English month name (UTC).</td></tr>
      <tr><td><code>year()</code></td><td>Four-digit year in the current Gregorian calendar (UTC).</td></tr>
    </tbody>
  </table>
  <p><code>include</code> is handled by the evaluator rather than registered as
  an ordinary environment procedure. Recursive includes, including indirect
  cycles, fail rather than producing infinite output. Example:</p>
  <pre><code>&lt;main&gt;
  @@{include("section.i")}
&lt;/main&gt;</code></pre>

  <h3>Output, dependencies, and failure behavior</h3>
  <p>The required dependency file describes the generated target as dependent
  on the source file and every transitively included file. It is designed for
  CMake dependency tracking (including Ninja depfiles); included templates
  are inputs, not additional generated targets. A change to a nested include
  therefore requires regeneration of the final output.</p>
  <p>On a successful invocation the CLI writes the rendered target file and
  the dependency file. The target is overwritten if it already exists as a
  regular file. The CLI requires parent directories to exist and does not
  guarantee atomic replacement of the output. Read, parse, evaluate, and
  write failures cause an unsuccessful invocation; no rollback of output
  files is promised.</p>
  <p>The CLI returns <code>EXIT_SUCCESS</code> on success and
  <code>EXIT_FAILURE</code> on failure. No stable numeric exit-code taxonomy
  or machine-readable diagnostics format is currently exposed. The present
  <code>--help</code> implementation prints usage but also returns failure.
  Do not interpret a nonzero status from <code>--help</code> as a render error.</p>

  <h3>Not currently supported</h3>
  <p>Loop directives, user-defined statements, general expressions, and
  user-defined procedures are possible future language extensions, not
  available features of this implementation.</p>
</section>
