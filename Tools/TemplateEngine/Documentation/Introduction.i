<section id="Introduction">
  <h2>Introduction</h2>
  <p>Build the Arcadia Template Engine from the repository root using CMake.
  The command-line executable is the target <code>Arcadia.TemplateEngine.CLI</code>:</p>
  <pre><code>cmake -S . -B build
cmake --build build --target Arcadia.TemplateEngine.CLI</code></pre>
  <p>The executable's location depends on the generator and configuration.
  The examples below assume it is on <code>PATH</code>. For multi-configuration
  generators, pass <code>--config Debug</code> or the selected configuration.</p>

  <h3>First template</h3>
  <p>Create a UTF-8 file named <code>hello.te</code> containing:</p>
  <pre><code>&lt;h1&gt;Hello, @@{project}!&lt;/h1&gt;</code></pre>
  <p>Create an environment file named <code>example.env</code>, using the
  Arcadia Data Definition Language (DDL) map syntax:</p>
  <pre><code>{
  project : "Arcadia",
}</code></pre>
  <p>Invoke the CLI with source, target, dependency output, and environment
  paths. The following is a POSIX-shell example: enclosing each argument
  in single quotes passes its inner double quotes through to Arcadia's
  command-line parser. Adapt argument quoting for other shells.</p>
  <pre><code>Arcadia.TemplateEngine.CLI '--source="hello.te"' '--target="hello.html"' '--dependencies="hello.d"' '--environment="example.env"'</code></pre>
  <p>The file <code>hello.html</code> then contains:</p>
  <pre><code>&lt;h1&gt;Hello, Arcadia!&lt;/h1&gt;</code></pre>
  <p><code>hello.d</code> records the template inputs for incremental builds.
  The destination directory must exist; an existing regular target file is
  overwritten. All paths in this example are relative to the process's
  working directory. To render a template without user-defined variables,
  omit <code>--environment</code>.</p>

  <h3>Run the tests and build the documentation</h3>
  <pre><code>ctest --test-dir build -R TemplateEngine --output-on-failure
cmake --build build --target Arcadia.TemplateEngine.BuildDocs</code></pre>
  <p>When building with a multi-configuration generator, use the matching
  CTest configuration (for example <code>ctest -C Debug</code>). Generated
  documentation is written to <code>.Website/Arcadia/TemplateEngine/index.html</code>.
  The page sources in <code>Tools/TemplateEngine/Documentation</code> are
  authoritative; do not edit generated HTML as the primary source.</p>
</section>
