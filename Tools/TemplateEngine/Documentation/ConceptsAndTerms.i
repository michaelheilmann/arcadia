<section id="ConceptsAndTerms">
  <h2>Concepts and Terms</h2>
  <dl>
    <dt>Template</dt>
    <dd>A UTF-8 text file containing ordinary text and embedded directives.
    Ordinary text is copied to the output in order.</dd>
    <dt>Directive</dt>
    <dd>A construct beginning with <code>@@</code> in template source.
    In template <em>source</em>, <code>@@{name}</code> loads a variable and
    <code>@@{procedure("argument")}</code> invokes a procedure.</dd>
    <dt>Environment</dt>
    <dd>A collection of named values. An optional DDL map file supplies
    string-valued variables; built-in procedures come from the default
    environment. User-defined names can shadow built-in names.</dd>
    <dt>Include</dt>
    <dd>A directive that inserts the evaluated content of another template.
    Paths are interpreted relative to the Template Engine process working
    directory, not relative to the including template.</dd>
    <dt>Target</dt>
    <dd>The text file receiving the rendered output of an invocation.</dd>
    <dt>Dependency file</dt>
    <dd>The build dependency description identifying the target and all
    directly and transitively read template files. It enables incremental
    rebuilds and must not be confused with rendered output.</dd>
  </dl>
  <p>The CLI processes one top-level source template per invocation. Included
  templates share the invocation's environment and contribute to the same
  output file. See <a href="#TemplateLanguage">Template Language</a> for
  the supported constructs and <a href="#ModuleInterface">Module Interface</a>
  for the command-line and build integration.</p>
</section>
